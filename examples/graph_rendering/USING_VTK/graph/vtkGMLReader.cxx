/*
 *	File: vtkGMLReader.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkGMLReader.h"
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

vtkStandardNewMacro(vtkGMLReader);
vtkCxxRevisionMacro(vtkGMLReader, "1.0");


#define TOK_NOTFOUND		-1

#define	TOK_GRAPH				-1000
#define TOK_GRAPHICS		-1010
#define	TOK_DECLARE			-1020
#define TOK_VERSION			-1030
#define TOK_DIRECTED		-1040
#define TOK_NODE				-1050
#define TOK_EDGE				-1060
#define TOK_NODE_DATA		-1070
#define TOK_EDGE_DATA		-1080
#define TOK_NODE_COUNT	-1090
#define TOK_EDGE_COUNT	-1100
#define TOK_DEGREE			-1105
#define TOK_SOURCE			-1110
#define TOK_TARGET			-1120
#define TOK_ID					-1130
#define TOK_X						-1140
#define TOK_Y						-1150
#define TOK_Z						-1160
#define TOK_LINE				-1170
#define TOK_POINT				-1172
#define TOK_OPEN				-1180
#define TOK_CLOSE				-1190
#define TOK_OPTIONAL		-1200
#define TOK_MANDATORY		-1210

// Token values below the following are attributes, and the
// value encodes the index of the attribute in the table.
#define TOK_ATTRIBUTE		-5000


//-----------------------------------------------------------
// Constructors and destructors

vtkGMLReader::vtkGMLReader()
{
	int i;

  this->FileName = NULL;
  this->IS = NULL;
	this->Strings = vtkStringTable::New();
	
	this->AttributeArraySize = 0;
	this->NumberOfAttributes = 0;
	this->Attributes = NULL;
	
	// Intialize table for token lookup.
  for (i = 0; i < TOKEN_TABLE_SIZE; i++)
		{
    this->TokenTable[i] = NULL;
		}
	
	this->AddToken( "VTK_BIT",						VTK_BIT							);
	this->AddToken( "VTK_CHAR", 					VTK_CHAR	 					);
	this->AddToken( "VTK_FLOAT",					VTK_FLOAT						);
	this->AddToken( "VTK_ID_TYPE",				VTK_ID_TYPE					);
	this->AddToken( "VTK_INT",						VTK_INT							);
	this->AddToken( "VTK_LONG", 					VTK_LONG	 					);
	this->AddToken( "VTK_SHORT",					VTK_SHORT						);
	this->AddToken( "VTK_STRING", 				VTK_STRING	 				); 
	this->AddToken( "VTK_UNSIGNED_CHAR",	VTK_UNSIGNED_CHAR		);
	this->AddToken( "VTK_UNSIGNED_INT", 	VTK_UNSIGNED_INT	 	);
	this->AddToken( "VTK_UNSIGNED_LONG",	VTK_UNSIGNED_LONG		);
	this->AddToken( "VTK_UNSIGNED_SHORT", VTK_UNSIGNED_SHORT	);
	this->AddToken( "unsigned_char",			VTK_UNSIGNED_CHAR		);
	this->AddToken( "unsigned_int", 			VTK_UNSIGNED_INT	 	);
	this->AddToken( "unsigned_long",			VTK_UNSIGNED_LONG		);
	this->AddToken( "unsigned_short",			VTK_UNSIGNED_SHORT	);
	this->AddToken( "bit",								VTK_BIT							);
	this->AddToken( "float",							VTK_FLOAT						);
	this->AddToken( "double",							VTK_DOUBLE					);
	this->AddToken( "int",								VTK_INT							);
	this->AddToken( "long", 							VTK_LONG	 					);
	this->AddToken( "char", 							VTK_CHAR	 					);
	this->AddToken( "short",							VTK_SHORT						);
	this->AddToken( "string", 						VTK_STRING	 				); 

	this->AddToken( "OPTIONAL",						TOK_OPTIONAL				);
	this->AddToken( "MANDATORY",					TOK_MANDATORY				);
	this->AddToken( "declare",						TOK_DECLARE					);
	this->AddToken( "directed",						TOK_DIRECTED				);
	this->AddToken( "edge",								TOK_EDGE						);
	this->AddToken( "edge_count",					TOK_EDGE_COUNT			);
	this->AddToken( "edge_data",					TOK_EDGE_DATA				);
	this->AddToken( "graph",							TOK_GRAPH						);
	this->AddToken( "id",									TOK_ID							);
	this->AddToken( "node",								TOK_NODE						);
	this->AddToken( "node_count",					TOK_NODE_COUNT			);
	this->AddToken( "node_data",					TOK_NODE_DATA				);
	this->AddToken( "degree",							TOK_DEGREE					);
	this->AddToken( "source",							TOK_SOURCE					);
	this->AddToken( "target",							TOK_TARGET					);
	this->AddToken( "version",						TOK_VERSION					);
	this->AddToken( "line",								TOK_LINE						);
	this->AddToken( "point",							TOK_POINT						);
	this->AddToken( "graphics",						TOK_GRAPHICS				);
	this->AddToken( "x",									TOK_X								);
	this->AddToken( "y",									TOK_Y								);
	this->AddToken( "z",									TOK_Z								);

	this->AddToken( "[",									TOK_OPEN						);
	this->AddToken( "]",									TOK_CLOSE						);
}

vtkGMLReader::~vtkGMLReader()
{ 
  struct _vtkGMLToken_s *bucket;

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
		
  for (int i = 0; i < TOKEN_TABLE_SIZE; i++)
		{
		while (bucket = this->TokenTable[i]) 
			{
			this->TokenTable[i] = bucket->next;
			// no need to delete name; either its a token name and
			// in static storage, or an attribute name held in the
			// attribute table, and will be freed next.
			delete bucket;
			}
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

//-------------------------------------------------------------------

int vtkGMLReader::OpenFile()
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

//-------------------------------------------------------------------
// Stream input

// Family of functions for reading values.  A templated
// function would be better, if all compilers agreed
// on what they would accept ...

int vtkGMLReader::ReadValue(int* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}
int vtkGMLReader::ReadValue(unsigned int* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}
int vtkGMLReader::ReadValue(short* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}
int vtkGMLReader::ReadValue(unsigned short* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}
int vtkGMLReader::ReadValue(long* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}
int vtkGMLReader::ReadValue(unsigned long* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}
int vtkGMLReader::ReadValue(float* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}
int vtkGMLReader::ReadValue(double* result)
{
	*this->IS >> *result;
	return !(this->IS->fail());
}

// Read one, single-quote-enclosed, character.
float vtkGMLReader::ReadChar()
{
	char c, tmp;

	do 
		{
		*this->IS >> tmp; 
		}
	while (tmp != '\'');
	*this->IS >> c; 
	*this->IS >> tmp; 
	if (tmp != '\'')
		{
		vtkWarningMacro(<<"Missing ' after character.");
		}
	return static_cast<float>(c);
}

void vtkGMLReader::ReadString(char buffer[TOKEN_STRING_LENGTH])
{
  this->IS->width(TOKEN_STRING_LENGTH);
  *this->IS >> buffer;
  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unexpected error on reading string.\n");
		this->AbortParse = 1;
    }
}

// Read a double-quote-enclosed string of characters.
void vtkGMLReader::ReadQuotedString()
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
    this->TokenString[i] = c;
    *this->IS >> c;
    }
  this->TokenString[i] = 0;
  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unexpected error on reading string.\n");
    }
}

int vtkGMLReader::ReadToken()
{ 
	char inLowerCase[TOKEN_STRING_LENGTH];
	int token, i;

	this->ReadString(this->TokenString);
	// See if the string is present in its given case.
	token = this->Lookup(this->TokenString);
	if (token != TOK_NOTFOUND)
		{
		return token;
		}
	else
		{
		// Check whether string is present as lower-case.
		// Only non-terminals are case-insensitive, attribute
		// names should have matched already.
		for (i = 0; this->TokenString[i]; i++)
			{
			inLowerCase[i] = tolower(this->TokenString[i]);
			}
		inLowerCase[i] = 0;
		token = this->Lookup(inLowerCase);
		return (token < TOK_ATTRIBUTE) ? token : TOK_NOTFOUND;
		}
}

int vtkGMLReader::Expect(int token)
{
  return (this->ReadToken() == token);
}

void vtkGMLReader::ParseError(char *message)
{
	vtkErrorMacro(<<message<<": at or near "<<this->TokenString);
	this->AbortParse = 1;
}

//-------------------------------------------------------------------
// Attribute management.

void vtkGMLReader::ResizeAttributeArray(int size)
{
	_vtkGMLAttribute_s *array;

	array = new _vtkGMLAttribute_s[size];
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

int vtkGMLReader::DeclareAttribute(	
	char *name, 
	int type, 
	float defaultValue,
	char *defaultString,
	int location)
{
	_vtkGMLAttribute_s *entry;
	int i;

	// Perform basic check on values
	if (type < VTK_BIT || type > VTK_STRING)
		{
		this->ParseError("Unrecognized type");
		return TOK_NOTFOUND;
		}
	if (location != NODE_DATA && location != EDGE_DATA)
		{
		this->ParseError("Unrecognized location");
		return TOK_NOTFOUND;
		}

	// Check for existing entry; 
	for (i = 0; i < this->NumberOfAttributes; i++)
		{
		if (!strcmp(name, this->Attributes[i].name)) break;
		}
	if (i < this->NumberOfAttributes)
		{
		vtkWarningMacro(<<"Redeclaration of attribute "<<name<<" ignored.");
		return i;
		}
	if (i == this->AttributeArraySize)
		{
		// Extend array, increment size is arbitrary.
		this->ResizeAttributeArray(this->AttributeArraySize+5);
		}

	entry = this->Attributes + i;
	entry->name = new char[strlen(name)+1];
	strcpy(entry->name, name);
	entry->type = type;
	entry->token = TOK_ATTRIBUTE-(this->NumberOfAttributes);
	// The default value is treated as a pointer to a char
	// array, and the string is copied into the default
	// string field.  It will be copied into the string
	// table of the output graph.
	if (type == VTK_STRING)
		{
		entry->defaultString = new char[strlen(defaultString)+1];
		strcpy(entry->defaultString, defaultString);
		}
	else
		{
		entry->defaultValue  = defaultValue;
		entry->defaultString = NULL;
		}
	entry->location = location;
	this->NumberOfAttributes++;
	return i;
}


// Wrapper to allow specification of parameters as strings.
// Checks each parameter for an appropriate value, and
// then passes the unpacked values to the above form of this
// fuction, which manages the actual declaration.
int vtkGMLReader::DeclareAttribute(
	char *nameStr,
	char *typeStr,
	char *defaultStr,
	char *locationStr)
{
	int type;
	float defvalue = 0.0;
  istream *stream;

	// The token representing a data type name is the type id 
	// for that type.  If the name isn't found, the error will 
	// be picked up when the attribute is processed further.
	type = this->Lookup(typeStr);

	// Decode the default value if appropriate.
	if (type != VTK_STRING)
		{
		stream = this->IS;
		this->IS = new istrstream(defaultStr, strlen(defaultStr));
		if (type == VTK_CHAR)
			{
			defvalue = this->ReadChar();
			}
		else
			{
			this->ReadValue(&defvalue);
			}
		delete this->IS;
		this->IS = stream;
		}

	// Decode the location of the attribute.
	if (!strcmp(locationStr, "NODE_DATA"))
		{
		return this->DeclareAttribute(
				nameStr, type, defvalue, defaultStr, NODE_DATA);
		}
	if (!strcmp(locationStr, "EDGE_DATA"))
		{
		return this->DeclareAttribute(
				nameStr, type, defvalue, defaultStr, EDGE_DATA);
		}
	vtkErrorMacro(<<"Unknown attribute location");
	return TOK_NOTFOUND;
}

// Deprecated version: option string is ignored.
int vtkGMLReader::DeclareAttribute(
	char *nameStr,
	char *typeStr,
	char *defaultStr,
	char *locationStr,
	char *optionStr)
{
	vtkWarningMacro(<<"Optional/Mandatory deprecated, ignored.  See docs.");
	return this->DeclareAttribute(nameStr, typeStr, defaultStr, locationStr);
}

void vtkGMLReader::EnableAttribute(int i)
{
	_vtkGMLAttribute_s *attribute;

	attribute = this->Attributes + i;
	if (attribute->type == VTK_STRING)
		{
		// The data array holds the index of the string in the table.
		attribute->array = vtkDataArray::CreateDataArray(VTK_ID_TYPE);
		attribute->defaultValue 
			= this->Strings->AddString(attribute->defaultString);
		}
	else
		{
		attribute->array = vtkDataArray::CreateDataArray(attribute->type);
		}
	// Add the attribute name to the token table, so that 
	// it can be found by lookup during parsing.  The token
	// value encodes the index of the token in the table.
	this->AddToken(attribute->name, attribute->token);
}

//-------------------------------------------------------------------

void vtkGMLReader::PrintSelf(ostream &os, vtkIndent indent)
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
			os << "\n";
			}
		}
}

//-------------------------------------------------------------------
// Deprecated functions for setting/getting string table.
// Will be removed in a future release.

void vtkGMLReader::SetStrings(vtkStringTable *table)
{
	vtkWarningMacro(<<"Deprecated: set the table in the output.");
	if (this->Strings)
		{
		this->Strings->UnRegister(this);
		}
	this->Strings = table;
	if (this->Strings)
		{
		this->Strings->Register(this);
		}
}

vtkStringTable *vtkGMLReader::GetStrings()
{
	vtkWarningMacro(<<"Deprecated: get the table from the output.");
	return this->Strings;
}


//-------------------------------------------------------------------
// Management of attribute values.

void vtkGMLReader::ResetAttributeValues()
{
	_vtkGMLAttribute_s *attribute;

	// Initialize attribute values to defaults.
	for (int i = 0; i < this->NumberOfAttributes; i++)
		{
		attribute = this->Attributes + i;
		attribute->value = attribute->defaultValue;
		}
}

void vtkGMLReader::StoreAttributeValues(vtkIdType index, int kind)
{
	_vtkGMLAttribute_s *attribute;

	for (int i = 0; i < this->NumberOfAttributes; i++)
		{
		attribute = this->Attributes + i;
		if (attribute->location == kind)
			{
			attribute->array->InsertTuple1(index, attribute->value);
			}
		}
}


//-------------------------------------------------------------------
// Parse a GML File

void vtkGMLReader::Execute()
{
  vtkGraph *graph = this->GetOutput();

	int token;
  int level;          // number of enclosing '['s.
  int inNode, inEdge;

	_vtkGMLAttribute_s *attribute;

	// Holders for declaration components.
  char attrName[TOKEN_STRING_LENGTH]; 
  char defValue[TOKEN_STRING_LENGTH];
	char dataType[TOKEN_STRING_LENGTH];
	char attrType[TOKEN_STRING_LENGTH];
	int typeToken;

  vtkIdType source = -1; 
  vtkIdType target = -1;  	// Id of source / dest node of edge
	vtkIdType id = -1;
	vtkIdType index;
	int degree = 0;
	int idSeen = 0;
	int srcSeen = 0;
	int dstSeen = 0;
	int i;
	double coord[3];

	// Recording of edge bend data in "line [...]" block
	int bendSeen = 0;
	vtkIdType bend;
	vtkPoints *bends;


  vtkDebugMacro(<< "GML Reader Executing.\n");

  if (!this->FileName)
    {
    vtkErrorMacro(<<"FileName must be specified.");
    return;
    }
  if (this->OpenFile())
		{
		return;
		}

	// Check existence of a string table. If the graph already 
	// has one, that one is used.  If the graph doesn't, but the 
	// reader has one, that one is used.  Otherwise a new table
	// is created.  
	if (!graph->GetStrings())
		{
		if (!this->Strings)
			{
			this->Strings = vtkStringTable::New();
			}
		graph->SetStrings(this->Strings);
		}
	else
		{
		if (this->Strings)
			{
			this->Strings->UnRegister(this);
			}
		this->Strings = graph->GetStrings();
		this->Strings->Register(this);
		}

	// Begin parsing file ....
  this->AbortParse = 0;
	token = this->ReadToken();

	// Look for any "old"-style attribute declarations
  if (token == TOK_DECLARE)
		{
		vtkWarningMacro(<<"declare [...] deprecated, see documentation.");
		this->Expect(TOK_OPEN);
		token = this->ReadToken();
		while (!this->AbortParse && token != TOK_CLOSE)
      {
		  strcpy(attrName, this->TokenString);
			this->ReadString(dataType);
			this->ReadString(defValue);
			this->ReadString(attrType);
			this->ReadToken();						// consume "mode" - ignored.
			this->DeclareAttribute(attrName, dataType, defValue, attrType);
			token = this->ReadToken();		// look at next token
			}
		token = this->ReadToken();			// get token following ']'
		}

	// Look for start of graph.
  if (token != TOK_GRAPH || !this->Expect(TOK_OPEN))
    {
    vtkErrorMacro("Expected a graph in GML format");
		return;
    }

	// Enable each declared attribute, creating a suitable
	// data array for each.
	for (i = 0; i < this->NumberOfAttributes; i++)
		{
		this->EnableAttribute(i);
		}

  inNode = inEdge = 0;	// are we in a node or an edge context?
  level = 1;						// number of currently unmatched '['s.
	bends = vtkPoints::New();

  while (level > 0 && !this->AbortParse)
    {
		token = this->ReadToken();
		switch(token)
			{
			case TOK_NODE:
      	if (inNode || inEdge)
       		{
      	  this->ParseError("Already in node/edge" );
					break;
      	  }
				this->Expect(TOK_OPEN);
				// Set flags and defaults for node information
      	inNode = 1;
				degree = 0;
				idSeen = 0;
				coord[0] = coord[1] = coord[2] = 0.0;
				this->ResetAttributeValues();
      	level++;
      	break;

			case TOK_EDGE:
     		if (inNode || inEdge)
       		{
      	  this->ParseError("Already in node/edge" );
					break;
        	}
      	this->Expect(TOK_OPEN);
				// Set flags and defaults for edge information
      	level++;
      	inEdge   = 1;	// working on an edge
				idSeen   = 0; // no id seen yet
				srcSeen  = 0;	// no source ...
				dstSeen  = 0; // no target ...
				bendSeen = 0; // no bends ...
				this->ResetAttributeValues();
      	break;
	
			// The "graphics [ ... ]" directive is ignored
    	case TOK_GRAPHICS:
      	this->Expect(TOK_OPEN);
      	level++;
				break;

			// read a sequence of points representing a bend.
			case TOK_LINE:
				this->Expect(TOK_OPEN);
				while (!this->AbortParse && (token = this->ReadToken()) == TOK_POINT)
					{
					this->ReadValue(coord);
					this->ReadValue(coord+1);
					this->ReadValue(coord+2);
					bends->InsertNextPoint(coord);
					bendSeen = 1;
					}
				if (token != TOK_CLOSE)
					{
					vtkErrorMacro(<<"Expected ']' to end 'line ['");
					this->AbortParse = 1;
					}
				break;
			
    	case TOK_NODE_DATA:
				this->ReadString(attrName);
				typeToken = this->ReadToken();
				i = this->DeclareAttribute(attrName, typeToken, 0, "", NODE_DATA);
				this->EnableAttribute(i);
				if (this->NrNodes > 0)
					{
					this->Attributes[i].array->Resize(this->NrNodes);
					}			
				break;
			
    	case TOK_EDGE_DATA:
				this->ReadString(attrName);
				typeToken = this->ReadToken();
				i = this->DeclareAttribute(attrName, typeToken, 0, "", EDGE_DATA);
				this->EnableAttribute(i);
				if (this->NrEdges > 0)
					{
					this->Attributes[i].array->Resize(this->NrEdges);
					}			
				break;
    	
			case TOK_CLOSE:
      	level--;
      	if (level == 1 && inEdge)
        	{
					if (!srcSeen || !dstSeen)
						{
						this->ParseError("Missing source or target node");
						break;
						}
					if (idSeen)
						{
						graph->CreateEdge(id, source, target);
						}
					else
						{
						id = graph->CreateEdge(source, target);
						}
					index = graph->GetEdgeIndex(id);
					// Copy edge attribute values into each data, storing
					// the values at the index allocated to this node.
					this->StoreAttributeValues(index, EDGE_DATA);
					// If the edge bends, transfer the bend information to
					// the graph.
					if (bendSeen)
						{
						for (int b = 0; b < bends->GetNumberOfPoints(); b++)
							{
							bend = graph->CreateBend();
							graph->InsertBend(id, b, bend);
							graph->SetBendPosition(bend, bends->GetPoint(b));
							}
						// reset points for next bend.
						bends->Initialize();
						}
        	inEdge = 0;
					break;
					}
				if (level == 1 && inNode)
					{
					if (!idSeen)
						{
						this->ParseError("Missing nodeId");
						break;
						}
					graph->CreateNode(id);
					index = graph->GetNodeIndex(id);
					if (degree > 0)
						{
						graph->SetEdgeArraySize(id, degree);
						}
					this->StoreAttributeValues(index, NODE_DATA);
					graph->SetPosition(id, coord);
					inNode = 0;
					break;
					}
				break;

    	case TOK_ID:
				this->ReadValue(&id);
				idSeen = 1;
				break;

    	case TOK_NODE_COUNT:
				this->ReadValue(&(this->NrNodes));
				graph->SetInitialNodeSize(this->NrNodes);
				graph->GetLayout()->SetNumberOfPoints(this->NrNodes);
				for (i = 0; i < this->NumberOfAttributes; i++)
					{
					if (this->Attributes[i].location == NODE_DATA)
						{
						this->Attributes[i].array->Resize(this->NrNodes);
						}
					}
				break;

    	case TOK_EDGE_COUNT:
				this->ReadValue(&(this->NrEdges));
				graph->SetInitialEdgeSize(this->NrEdges);
				for (i = 0; i < this->NumberOfAttributes; i++)
					{
					if (this->Attributes[i].location == EDGE_DATA)
						{
						this->Attributes[i].array->Resize(this->NrEdges);
						}
					}
				break;

    	case TOK_DEGREE:
				this->ReadValue(&degree);
				break;

    	case TOK_SOURCE:
				this->ReadValue(&source);
				srcSeen = 1;
				break;

      case TOK_TARGET:
      	this->ReadValue(&target);
				dstSeen = 1;
				break;

			case TOK_X:
				this->ReadValue(coord);
				break;

			case TOK_Y:
				this->ReadValue(coord+1);
				break;

			case TOK_Z:
				this->ReadValue(coord+2);
				break;

			case TOK_POINT:
				this->ReadValue(coord);
				this->ReadValue(coord+1);
				this->ReadValue(coord+2);
				break;

    	case TOK_DIRECTED:
			case TOK_VERSION:
				// Ignore these directives.
      	this->ReadToken();
				break;

			case TOK_NOTFOUND:
				// Unexpected input: signal a warning and abort parse.
				this->ParseError("Unexpected token");
				break;

			default:
				// The remaining case is where the token is an attribute.
				// The token value encodes the offset of the attribute
				// in the table.
				attribute = this->Attributes + (TOK_ATTRIBUTE - token);
				if (attribute->type == VTK_STRING)
					{
					// Handle a string.  Create an entry in the string
					// table, and take the index of the entry as an id
					// type value.
					this->ReadQuotedString();
					attribute->value = 
						(float)(graph->GetStrings()->AddString(this->TokenString));
					break;
					}
				if (attribute->type == VTK_CHAR)
					{
					// Skip to a "'", read the following char, then expect
					// a closing "'".
					attribute->value = this->ReadChar();
					break;
					}
				// Otherwise read the value as a float
				this->ReadValue(&(attribute->value));
				break;

			} // switch(token)
  	} // while tokens remain.

	if (this->AbortParse)
		{
		graph->Initialize();
		}
	else
		{
		// File has been read successfully.
		// Transfer created attributes to output graph.
		for (i = 0; i < this->NumberOfAttributes; i++)
			{
			attribute = this->Attributes + i;
			attribute->array->SetName(attribute->name);
			if (attribute->location == NODE_DATA)
				{
				graph->GetNodeData()->AddArray(attribute->array);
				if (attribute->type == VTK_STRING)
					{
					graph->MarkAsNodeDataStringArray(attribute->name);
					}
				}
			else
				{
				graph->GetEdgeData()->AddArray(attribute->array);
				if (attribute->type == VTK_STRING)
					{
					graph->MarkAsEdgeDataStringArray(attribute->name);
					}
				}
			attribute->array->UnRegister(this);
			attribute->array = NULL;
			}
		}

	// Remove each attribute from the token table, and
	// clear out the attribute table.
	for (i = 0; i < this->NumberOfAttributes; i++)
		{
		this->RemoveToken(this->Attributes[i].name);
		delete [] this->Attributes[i].name;
		if (this->Attributes[i].defaultString)
			{
			delete [] this->Attributes[i].defaultString;
			}
		}
	this->NumberOfAttributes = 0;
	
	vtkDebugMacro(<<"GMLReader done.");
}


//--------------------------------------------------------------
// Token lookup functions.

void vtkGMLReader::AddToken(char *name, int token)
{
  int slot = this->Hash(name);
  struct _vtkGMLToken_s *bucket = this->TokenTable[slot];
	
  while (bucket && strcmp(name, bucket->name))
		{
		bucket = bucket->next;
		} 
	if (bucket)
		{
		vtkWarningMacro(<<"Overriding token.");
		}
	else
		{
		bucket = new (struct _vtkGMLToken_s);
		bucket->name = new char[strlen(name) + 1];
		strcpy(bucket->name, name);
		bucket->next = this->TokenTable[slot];
		this->TokenTable[slot] = bucket;
		}
	bucket->token = token;
}


void vtkGMLReader::RemoveToken(char *name)
{
  int slot = this->Hash(name);
  struct _vtkGMLToken_s *prev;
  struct _vtkGMLToken_s *bucket;
  
	prev = NULL;
	bucket = this->TokenTable[slot];
  while (bucket && strcmp(name, bucket->name))
    {
		prev = bucket;
		bucket = bucket->next;
		}
	if (!bucket)
		{
		vtkErrorMacro(<<"RemoveToken: "<<name<<" not found.");
		return;
		}
	if (prev)
		{
		prev->next = bucket->next;
		}
	else
		{
		this->TokenTable[slot] = bucket->next;
		}
	delete [] bucket->name;
	delete bucket;
}

int vtkGMLReader::Lookup(char *name)
{
  int slot = this->Hash(name);
  struct _vtkGMLToken_s *bucket = this->TokenTable[slot];
  
  while (bucket && strcmp(name, bucket->name))
		{
		bucket = bucket->next;
		} 
	return (bucket != NULL) ? bucket->token : TOK_NOTFOUND;
}
	
// Hashing function, following Figure 5.4 in:
//   Data Structures and Algorithm Analysis in C++,
//   Mark Allen Weiss, Addison Wesley, 1999

int vtkGMLReader::Hash(char *key)
{
  int result = 0;

  while (*key)
		{
    result = 37*result + *key++;
		}
	result %= TOKEN_TABLE_SIZE;
	
  if (result < 0)
		{
		result += TOKEN_TABLE_SIZE;
		}
	return result;
}


