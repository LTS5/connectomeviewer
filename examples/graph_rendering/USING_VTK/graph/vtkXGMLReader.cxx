/*
 *	File: vtkXGMLReader.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkXGMLReader.h"
#include "vtkObjectFactory.h"
#include "vtkErrorCode.h"
#include "vtkGraph.h"

#include "vtkIdTypeArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkCharArray.h"
#include "vtkFieldData.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

vtkStandardNewMacro(vtkXGMLReader);
vtkCxxRevisionMacro(vtkXGMLReader, "1.0");

char *vtkXGMLReader::DataAttributeTypeNames[2] 
= { "NODE_DATA",
		"EDGE_DATA" };

char *vtkXGMLReader::DataAttributeOptionNames[2]
= { "OPTIONAL",
		"MANDATORY" };

vtkXGMLReader::_vtkDataType_s vtkXGMLReader::DataTypes[]
= { { "VTK_BIT",						VTK_BIT,						"%d"	},
		{ "VTK_CHAR", 					VTK_CHAR, 					"'%c'"  },
		{ "VTK_UNSIGNED_CHAR",	VTK_UNSIGNED_CHAR,	"%c"  },
		{ "VTK_SHORT",					VTK_SHORT,					"%d"  },
		{ "VTK_UNSIGNED_SHORT", VTK_UNSIGNED_SHORT, "%d"  },
		{ "VTK_INT",						VTK_INT,						"%d"  },
		{ "VTK_UNSIGNED_INT", 	VTK_UNSIGNED_INT, 	"%d"  },
		{ "VTK_LONG", 					VTK_LONG, 					"%ld" },
		{ "VTK_UNSIGNED_LONG",	VTK_UNSIGNED_LONG,	"%ld" },
		{ "VTK_FLOAT",					VTK_FLOAT,					"%f"  },
		{ "VTK_ID_TYPE",				VTK_ID_TYPE,				"%ld" },
		{ "VTK_STRING", 				VTK_STRING, 				"%s" 	}, 
		{ NULL, 			 					-1, 								NULL  }};

//----------------------------------------------------------------------------

vtkXGMLReader::vtkXGMLReader()
{
  this->FileName = NULL;
  this->IS = NULL;
	this->Strings = vtkStringTable::New();
	
	this->AttributeArraySize = 0;
	this->NumberOfAttributes = 0;
	this->Attributes = NULL;
	
	// Declare standard GML attributes. 
	this->DeclareAttribute("x",			VTK_FLOAT,	0.0,	NULL, NODE_DATA, XGML_OPTIONAL);
	this->DeclareAttribute("y",			VTK_FLOAT,	0.0,	NULL, NODE_DATA, XGML_OPTIONAL);
	this->DeclareAttribute("z",			VTK_FLOAT,	0.0,	NULL, NODE_DATA, XGML_OPTIONAL);
	this->DeclareAttribute("s",			VTK_FLOAT,	1.0,	NULL, NODE_DATA, XGML_MANDATORY);
	this->DeclareAttribute("label", VTK_STRING, 0,		"",		NODE_DATA, XGML_OPTIONAL);
}

//----------------------------------------------------------------------------

vtkXGMLReader::~vtkXGMLReader()
{ 
  if (this->IS)
    {
    delete this->IS;
    this->IS = NULL;
    }
  
  if (this->FileName)
    {
    delete [] this->FileName;
    this->FileName = NULL;
    }
		
	if (this->Attributes)
		{
		for (int i = 0; i < this->NumberOfAttributes; i++)
			{
			delete [] this->Attributes[i].name;
			if (this->Attributes[i].defaultString)
				{
				delete [] this->Attributes[i].defaultString;
				}
			}
		delete [] this->Attributes;
		}
}

void vtkXGMLReader::ResizeAttributeArray(int size)
{
	_vtkXGMLAttribute_s *array;

	array = new _vtkXGMLAttribute_s[size];
	if (this->Attributes)
		{
		for (int i = 0; i < this->NumberOfAttributes; i++)
			{
			array[i] = this->Attributes[i];
			}
		delete [] this->Attributes;
		}
	this->Attributes = array;
	this->AttributeArraySize = size;
}

void vtkXGMLReader::DeclareAttribute(	
	char *attributeName, 
	int valueType, 
	float defaultValue,
	char *defaultString,
	int dataAttributeType,
	XGMLOptionMode optionalFlag)
{
	_vtkXGMLAttribute_s *entry;
	int position = 0;

	while (position < this->NumberOfAttributes 
		&& ((this->Attributes[position].attributeType != dataAttributeType)
				|| 
				strcmp(attributeName, this->Attributes[position].name)))
		{
		position++;
		}
	if (position < this->NumberOfAttributes)
		{
		vtkErrorMacro(<<"Redeclaration of attribute "<<attributeName);
		return;
		}
	if (position == this->AttributeArraySize)
		{
		// Extend array, increment size is arbitrary.
		this->ResizeAttributeArray(this->AttributeArraySize+5);
		}
	entry = this->Attributes + position;

	entry->name = new char[strlen(attributeName)+1];
	strcpy(entry->name, attributeName);
	
	entry->typeInfo = -1;
	for (int i = 0; DataTypes[i].name; i++)
		{
		if (DataTypes[i].typeId == valueType)
			{
			entry->typeInfo = i;
			break;
			}
		}
	if (entry->typeInfo < 0)
		{
		vtkErrorMacro("Unrecognized type id\n");
		}
	
	// The default value is treated as a pointer to a char
	// array, and the string is copied into the default
	// string field.  It is NOT allocated yet into the string
	// table, as this may change for each execution of the
	// reader.
	if (valueType == VTK_STRING)
		{
		entry->defaultString = new char[strlen(defaultString)+1];
		strcpy(entry->defaultString, defaultString);
		}
	else
		{
		entry->defaultValue  = defaultValue;
		entry->defaultString = NULL;
		}
	entry->attributeType = dataAttributeType;
	entry->optional      = (optionalFlag == XGML_OPTIONAL);
	
	// Find the format to use to convert from strings to values
	// of this type.
	position = 0;
	while (DataTypes[position].format)
		{
		if (DataTypes[position].typeId == valueType)
			{
			entry->format = DataTypes[position].format;
			break;
			}
		else
			{
			position++;
			}
		}

	this->NumberOfAttributes++;
}


// Wrapper to allow specification of parameters as strings.
// Checks each parameter for an appropriate value, and
// then passes the unpacked values to the above form of this
// fuction, which manages the actual declaration.

void vtkXGMLReader::DeclareAttribute(
	char *attributeName, 
	char *valueType, 
	char *defaultString,
	char *dataAttributeType,
	char *optionalFlag)
{
	int type;
	float defvalue;
  char charValue;
	int attributeType = NODE_DATA;
	XGMLOptionMode optional = 1;
	
	int i;

	// Decode the kind of attribute, NODE_DATA or EDGE_DATA
	for (i = 0; i < 2; i++)
		{
		if (!strcmp(dataAttributeType, DataAttributeTypeNames[i]))
			{
			attributeType = i;
			break;
			}
		}
	if (i == 2)
		{
		vtkErrorMacro(<<"Unrecognized attribute type: "<<dataAttributeType);
		return;
		}
		
	// Decode optionality, either OPTIONAL or MANDATORY
	for (i = 0; i < 2; i++)
		{
		if (!strcmp(optionalFlag, DataAttributeOptionNames[i]))
			{
			optional = (i == 0) ? XGML_OPTIONAL : XGML_MANDATORY;
			break;
			}
		}
	if (i == 2)
		{
		vtkErrorMacro(<<"Unrecognized option type: "<<optionalFlag);
		return;
		}

	// Discover type name.
	i = 0;
	while (DataTypes[i].name 
			&& strcmp(DataTypes[i].name, valueType))
		{
		i++;
		}
	if (DataTypes[i].name)
		{
		type = DataTypes[i].typeId;
		}
	else
		{
		vtkErrorMacro("Unrecognized type: "<<valueType);
		return;
		}
		
	// Decode the default value.  
	if (type == VTK_CHAR)
		{
		sscanf(defaultString, DataTypes[i].format, &charValue);
		defvalue = (float)charValue;
		}
	else
		{
		if (type == VTK_STRING)
			{
			defvalue = 0;
			}
		else
			{
			sscanf(defaultString, DataTypes[i].format, &defvalue);
			}
		}
		
	// Declare the attribute.
	this->DeclareAttribute(
		attributeName,
		type,
		defvalue,
		defaultString,
		attributeType,
		optional);
}


//-------------------------------------------------------------------

void vtkXGMLReader::PrintSelf(ostream &os, vtkIndent indent)
{
  os << indent << "File: ";
  os << (this->FileName ? this->FileName : "(none)") << "\n";
	if (this->NumberOfAttributes > 0)
		{
		os << indent << "Declared Attributes:\n";
		for (vtkIdType i = 0; i < this->NumberOfAttributes; i++)
			{
			os << indent.GetNextIndent();
			os << this->Attributes[i].name;
			os << "\t" << DataTypes[this->Attributes[i].typeInfo].name;
			os << "\t" << DataAttributeTypeNames[this->Attributes[i].attributeType];
			if (!this->Attributes[i].optional)
				{
				os << " *";
				}
			os << "\n";
			}
		}
}

//-------------------------------------------------------------------

int vtkXGMLReader::OpenFile()
{
  if ( !this->FileName || (strlen(this->FileName) == 0))
    {
    vtkErrorMacro(<< "No file specified!");
    this->SetErrorCode( vtkErrorCode::NoFileNameError );
    return 1;
    }
  this->IS = new ifstream(this->FileName, ios::in);
  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unable to open file: "<< this->FileName);
    delete this->IS;
    this->IS = NULL;
    this->SetErrorCode( vtkErrorCode::CannotOpenFileError );
    return 1;
    }
  return 0;
}

void vtkXGMLReader::ReadString(char result[4096])
{
  this->IS->width(4096);
  *this->IS >> result;
  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unexpected error on reading string.\n");
    }
}

void vtkXGMLReader::ReadQuotedString(char result[256])
{
  char c;
  int i;

  do
    {
    *this->IS >> c;
    }
  while (c != '\"'); 
  *this->IS >> c;
  for (i = 0; c != '\"'; i++)
    {
    result[i] = c;
    *this->IS >> c;
    }
  result[i] = 0;
  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unexpected error on reading string.\n");
    }
}

//-------------------------------------------------------------------
// Expect a token ...

int vtkXGMLReader::Expect(const char *expected)
{
  char token[4096];

  ReadString(token);
  return strcmp(token, expected);
}


//-------------------------------------------------------------------

#define AbortFlagMacro(M)    vtkErrorMacro( << M << "\n"); abortParse=1;
#define AbortMacro(M)    AbortFlagMacro(M); break;
#define ExpectMacro(T)  if (this->Expect(T)) { AbortMacro( "Expected "<<T ); }

void vtkXGMLReader::Execute()
{
  char token[4096];    // string read from input stream;

  char decAttrName[80]; 
  char decDataType[30];
  char decAttrType[30];
  char decAttrDefault[80];
  char decAttrMode[20];

  int level;          // number of enclosing '['s.
  int abortParse;
  int inNode, inEdge;
	
	_vtkXGMLAttribute_s *attribute;
	
  vtkIdType source = -1; 
  vtkIdType target = -1;  	// Id of source / dest node of edge
  vtkIdType nodeId  = -1;   // Most recent node-id seen.
	vtkIdType edgeId  = -1;
	vtkIdType index;
  vtkIdType nextFreeNodeId = 0;
	vtkIdType nextFreeEdgeId = 0;
	int edgeIdSeen = 0;
	int nodeIdSeen = 0;
	int dataType;
	int i;
	vtkIdType num;
	vtkIdType edgeCount = 0;
	int edgeCountSeen = 0;

	char cval;

  vtkDebugMacro(<< "XGML Reader Executing.\n");

  if (!this->FileName)
    {
    vtkErrorMacro(<<"FileName must be specified.");
    return;
    }
  if (this->OpenFile())
		{
		return;
		}
	this->ReadString(token);

	// Look for any attribute declarations
  if (!strcmp(token, "declare"))
		{
		this->Expect("[");
		this->ReadString(token);
		while (strcmp(token,"]"))
      {
		  strcpy(decAttrName, token);
			this->ReadString(decDataType);
			this->ReadString(decAttrDefault);
			this->ReadString(decAttrType);
			this->ReadString(decAttrMode);
			this->DeclareAttribute(
				decAttrName,
				decDataType,
				decAttrDefault,
				decAttrType,
				decAttrMode);
			this->ReadString(token);
			}
		this->ReadString(token);
		}

	// Parse body of graph
  if (strcmp(token,"graph") || this->Expect("["))
    {
    vtkErrorMacro("Expected a graph in XGML format");
		return;
    }

	if (!this->Strings)
		{
		vtkErrorMacro("No string table.");
		return;
		}
		
	
  vtkGraph *graph = this->GetOutput();

	// Create a data array for each declared attribute,
	// and mark each attribute as unseen.  For string-valued
	// attributes, also add the default string to the string
	// table and store the id of that string to be used as 
	// the default value.
	// this->Strings->Initialize();
	for (i = 0; i < this->NumberOfAttributes; i++)
		{
		attribute = this->Attributes + i;
		dataType = DataTypes[attribute->typeInfo].typeId;
		attribute->seen = false;
		if (dataType == VTK_STRING)
			{
			// The data array holds the index of the string in the table.
			attribute->array = vtkDataArray::CreateDataArray(VTK_ID_TYPE);
			attribute->defaultValue 
				= this->Strings->AddString(attribute->defaultString);
			}
		else
			{
			attribute->array = vtkDataArray::CreateDataArray(dataType);
			}
		}

	  
  inNode = inEdge = 0;	// are we in a node or an edge context?
  abortParse = 0;
  level = 1;						// number of currently unmatched '['s.
	nodeId = nextFreeNodeId;
	edgeId = nextFreeEdgeId;

  while (level > 0 && !abortParse)
    {
    this->ReadString(token);
  	vtkDebugMacro(<< "token: "<<token);


		// Test token, first checking for special cases, then dealing
		// with general attributes.
    if (!strcmp(token,"node"))
      {
      if (inNode || inEdge)
        {
        AbortMacro( "Node opened before previous node/edge closed." );
        }

			edgeCountSeen = 0;
			this->ReadString(token);
			if (strcmp(token, "["))
				{
				sscanf(token, "%ld", &edgeCount);
				edgeCountSeen = 1;
				this->ReadString(token);
			}
      if (strcmp(token, "[" ))
				{
				vtkErrorMacro(<<"Expected [");
				}
      level++;
      inNode = 1;
			nodeIdSeen = 0;
			// Allocate a defaut nodeId for use if an explicit id
			// is not given by an "id" declaration.
			nodeId = nextFreeNodeId;
			// Initialize node attributes.
			for (i = 0; i < this->NumberOfAttributes; i++)
				{
				attribute = this->Attributes + i;
				if (attribute->attributeType == NODE_DATA)
					{
					attribute->value = attribute->defaultValue;
					}
				}
      continue;
      } // ------ END OF 'node' CASE ---------------------


    if (!strcmp(token,"edge"))
      {
      if (inNode || inEdge)
        {
        AbortMacro( "Edge opened before previous node/edge closed." );
        }
      ExpectMacro( "[" )
      level++;
      inEdge = 1;
			edgeIdSeen = 0;
			// Allocate a defaut edgeId for use if an explicit id
			// is not given by an "id" declaration.
			edgeId = nextFreeEdgeId;
			// Initialize edge attributes.
			for (i = 0; i < this->NumberOfAttributes; i++)
				{
				attribute = this->Attributes + i;
				if (attribute->attributeType == EDGE_DATA)
					{
					attribute->value = attribute->defaultValue;
					}
				}
      continue;
      } // ------ END OF 'edge' CASE ---------------------
	
    if (!strcmp(token,"graphics"))
      {
      ExpectMacro( "[" )
      level++;
			continue;
			} // ------ END OF 'graphics' CASE -----------------
			
			
    if (!strcmp(token,"Line"))
      {
      ExpectMacro( "[" )
      level++;
			continue;
			} // ------ END OF 'Line' CASE ---------------------
			
			
		// Closing ']' may be the end of a node or edge block, and if
		// so triggers updating of data structures.  Node or edge blocks
		// are at level 2 in the file.
    if (!strcmp(token,"]"))
      {
      if (level == 2 && inEdge)
        {
				graph->CreateEdge(edgeId, source, target);
				index = graph->GetEdgeIndex(edgeId);
				if (!edgeIdSeen)
					{
					++nextFreeEdgeId;
					}
				// Copy edge attribute values into each data, storing
				// the values at the index allocated to this node.
				for (i = 0; i < this->NumberOfAttributes; i++)
					{
					attribute = this->Attributes + i;
					if (attribute->attributeType == EDGE_DATA)
						{
						attribute->array->InsertTuple1(index, attribute->value);
						}
					}
        vtkDebugMacro(<<"Created edge "<<edgeId<<": "<<source<<"-"<<target);
        inEdge = 0;
				}
				
			if (level == 2 && inNode)
				{
				graph->CreateNode(nodeId);
				index = graph->GetNodeIndex(nodeId);
        vtkDebugMacro(<<"Created node "<<nodeId);
				if (!nodeIdSeen) 
					{
					++nextFreeNodeId;
					}
				if (edgeCountSeen)
					{
					graph->SetEdgeArraySize(nodeId, edgeCount);
					}
				// Copy node attribute values into each data, storing
				// the values at the index allocated to this node.
				for (i = 0; i < this->NumberOfAttributes; i++)
					{
					attribute = this->Attributes + i;
					if (attribute->attributeType == NODE_DATA)
						{
						attribute->array->InsertTuple1(index, attribute->value);
						}
					}
				inNode = 0;
				}
				
      level--;
      continue;
      } // ------ END OF ']' CASE ------------------------


    if (!strcmp(token,"id"))
      {
      this->ReadString(token);
      if (inEdge)
        {
				sscanf(token, "%ld", &edgeId);
				edgeIdSeen = 1;
				// Make sure that any subsequent id allocated by default
				// will not collide with this explicitly given id by
				// moving the next id to be allocated past this one.
				if (edgeId > nextFreeEdgeId)
					{
					nextFreeEdgeId = edgeId + 1;
					}
        continue;
        } 
      if (inNode)
        {
        sscanf(token, "%ld", &nodeId);
				nodeIdSeen = 1;
				if (nodeId > nextFreeNodeId)
					{
					nextFreeNodeId = nodeId + 1;
					}
        continue;
        }
      // if not in an edge or a node, this token is unexpected
      // and should be treated as an error.
      } // ------ END OF 'id' CASE ----------------------
			

    if (!strcmp(token,"node_count"))
      {
      this->ReadString(token);
      sscanf(token, "%ld", &num);
			graph->SetInitialNodeSize(num);
      continue;
      } // ------ END OF 'node_count' CASE -----------------

    if (!strcmp(token,"edge_count"))
      {
      this->ReadString(token);
      sscanf(token, "%ld", &num);
			graph->SetInitialEdgeSize(num);
      continue;
      } // ------ END OF 'node_count' CASE -----------------


		// edge source and target nodes could be treated as edge
		// attributes, but are handled specially as edges are
		// created by calling a specific method in the graph interface.
    if (!strcmp(token,"source"))
      {
      this->ReadString(token);
      sscanf(token, "%ld", &source);
      continue;
      } // ------ END OF 'source' CASE -----------------
			
    if (!strcmp(token,"target"))
      {
      this->ReadString(token);
      sscanf(token, "%ld", &target);
			continue;
      } // ------ END OF 'target' CASE -----------------

		// GML declarations that are silently ignored
		// DIRECTED flag
		// VERSION number
    if (!strcmp(token,"directed"))
      {
      this->ReadString(token);
			continue;
      } 
    if (!strcmp(token,"version"))
      {
      this->ReadString(token);
			continue;
      } 

		// Is the token an attribute?  Check whether the token
		// matches an attribute name appropriate to the context
		// (in a node, or in an edge), and if so record the
		// corresponding value.
		for (i = 0; i < this->NumberOfAttributes; i++)
			{
			attribute = this->Attributes + i;
			if (((inNode && attribute->attributeType == NODE_DATA) ||
			     (inEdge && attribute->attributeType == EDGE_DATA))
				&& !strcmp(token, attribute->name))
				{
				attribute->seen = true;
				if (DataTypes[attribute->typeInfo].typeId == VTK_STRING)
					{
					// Handle a string.  Create an entry in the string
					// table, and take the index of the entry as an id
					// type value.
					this->ReadQuotedString(token);
					attribute->value = (float)(this->Strings->AddString(token));
					break;
					}
				if (DataTypes[attribute->typeInfo].typeId == VTK_CHAR)
					{
					// Skip to a "'", read the following char, then expect
					// a closing "'".
					do 
						{
						*this->IS >> cval; 
						}
					while (cval != '\'');
					*this->IS >> cval; 
					attribute->value = (float)cval;
					*this->IS >> cval; 
					if (cval != '\'')
						{
						vtkWarningMacro(<<"Missing ' after character.");
						}
					break;
					}
				// Otherwise use conversion format to scan next token.
				this->ReadString(token);
				//sscanf(token, attribute->format, &(attribute->value));
				sscanf(token, "%f", &(attribute->value));

				// Attribute handled, do not consider any others.
				break;
				} /* matched attribute */
			} /* for each possible attribute */
		if (i == this->NumberOfAttributes)
			{
    	// Any token not matched is an error; 
			// skip it and try to proceed.
			vtkWarningMacro(<<"Skipping "<<token);
			}
		// ------ END OF general token CASE ----------------------
		
  } // while tokens remain.

	// ---------------------------------------------------------
	
	// File has been read.
	// Transfer created attributes to output graph.
	for (i = 0; i < this->NumberOfAttributes; i++)
		{
		attribute = this->Attributes + i;
		// A declared attribute might not have been present in the
		// dataset.  Only attach the attribute array to the data
		// if some instance of the attribute has been encountered.
		if (!attribute->optional || attribute->seen)
			{
			attribute->array->SetName(attribute->name);
			if (attribute->attributeType == NODE_DATA)
				{
				this->GetOutput()->GetNodeData()->AddArray(attribute->array);
				if (DataTypes[attribute->typeInfo].typeId == VTK_STRING)
					{
					this->GetOutput()->MarkAsNodeDataStringArray(attribute->name);
					}
				}
			else
				{
				this->GetOutput()->GetEdgeData()->AddArray(attribute->array);
				if (DataTypes[attribute->typeInfo].typeId == VTK_STRING)
					{
					this->GetOutput()->MarkAsEdgeDataStringArray(attribute->name);
					}
				}
			attribute->array->UnRegister(this);
			attribute->array = NULL;
			}
		}
	this->GetOutput()->SetStrings(this->Strings);
	vtkDebugMacro(<<"XGMLReader done.");
}


