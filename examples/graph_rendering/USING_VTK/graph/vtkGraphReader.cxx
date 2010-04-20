/*
 *	File: vtkGraphReader.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkGraphReader.h"
#include "vtkObjectFactory.h"
#include "vtkGraph.h"
#include "vtkStringTable.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkXMLUnstructuredGridReader.h"
#include "vtkUnstructuredGridToGraph.h"
#include "vtkIntArray.h"
#include "vtkErrorCode.h"

#include <ctype.h>
#include <sys/stat.h>

vtkStandardNewMacro(vtkGraphReader);
vtkCxxRevisionMacro(vtkGraphReader, "1.0");



//-----------------------------------------------------------
// Constructors and destructors

vtkGraphReader::vtkGraphReader()
{
  this->BaseName = NULL;
	this->XMLFormat = 0;
	this->IS = NULL;
}

vtkGraphReader::~vtkGraphReader()
{ 
  if (this->BaseName)
    {
    delete [] this->BaseName;
    this->BaseName = NULL;
    }
	if (this->IS)
		{
		delete this->IS;
		}
}

void vtkGraphReader::PrintSelf(ostream& os, vtkIndent indent)
{
	this->vtkGraphSource::PrintSelf(os, indent);
	os << indent << "Basename: ";
	os << ((this->BaseName) ? this->BaseName : "(none)") << "\n";
	os << indent << "XML? ";
	os << (this->XMLFormat ? "yes" : "no") << "\n";
}

int vtkGraphReader::ReadNumber()
{
	int num;
	
	if (this->AbortParse) return 0;
	
	*this->IS >> num;
	if (this->IS->fail())
		{
		vtkErrorMacro(<<"Error reading number.");
		this->AbortParse = 1;
		}
	return num;
}

void vtkGraphReader::ReadQuotedString()
{
  char c;
  int i;

	if (this->AbortParse) return;
	
  this->IS->width(MAX_STRING_LENGTH);
  do
    {
    *this->IS >> c;
    }
  while (c != '\"'); 
  *this->IS >> c;
  for (i = 0; c != '\"'; i++)
    {
    this->String[i] = c;
    *this->IS >> c;
    }
  this->String[i] = 0;
  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unexpected error on reading string.\n");
		this->AbortParse = 1;
    }
}

void vtkGraphReader::ReadString()
{
	if (this->AbortParse) return;
	
  this->IS->width(MAX_STRING_LENGTH);
  *this->IS >> this->String;

  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unexpected error on reading string.\n");
		this->AbortParse = 1;
    }
}

void vtkGraphReader::Expect(const char *token)
{
	this->ReadString();

	if (this->AbortParse) return;

	if (strcmp(token, this->String))
		{
		vtkErrorMacro(<< "Unexpected input: "<<this->String<<" in file\n");
		this->AbortParse = 1;
		}
}

//-------------------------------------------------------------------


void vtkGraphReader::Execute()
{
  vtkGraph *graph = this->GetOutput();

	vtkUnstructuredGridToGraph *converter;
	vtkUnstructuredGridReader *reader = NULL;
  vtkXMLUnstructuredGridReader *xmlReader = NULL;
	vtkStringTable *strings;
	
	char *graphFileName;
	char *stringFileName;
	int num, i, strId;

	vtkDebugMacro(<<"GraphReader starting.");

	if (!this->BaseName)
		{
		vtkErrorMacro(<<"No base name for input.");
		return;
		}
	graphFileName = new char[strlen(this->BaseName)+5];
	stringFileName = new char[strlen(this->BaseName)+5];
	strcpy(graphFileName, this->BaseName);
	strcpy(stringFileName, this->BaseName);

	converter = vtkUnstructuredGridToGraph::New();

	if (this->XMLFormat)
		{
		strcat(graphFileName,".vtu");
		xmlReader = vtkXMLUnstructuredGridReader::New();
		xmlReader->SetFileName(graphFileName);
		converter->SetInput(xmlReader->GetOutput());
		}
	else
		{
		strcat(graphFileName,".vtk");
		reader = vtkUnstructuredGridReader::New();
		reader->SetFileName(graphFileName);
		converter->SetInput(reader->GetOutput());
		}

	converter->Update();
	graph->ShallowCopy(converter->GetOutput());

	strcat(stringFileName, ".str");
	
	this->AbortParse = 0;
  // first make sure the file exists, this prevents an empty file from
  // being created on older compilers
  struct stat fs;
  if (stat(stringFileName, &fs) != 0) 
    {
    vtkErrorMacro(<< "Unable to open file: "<< stringFileName);
    this->SetErrorCode( vtkErrorCode::CannotOpenFileError );
		this->AbortParse = 1;
    }
  this->IS = new ifstream(stringFileName, ios::in);
  if (this->IS->fail())
    {
    vtkErrorMacro(<< "Unable to open file: "<< stringFileName);
    delete this->IS;
    this->IS = NULL;
    this->SetErrorCode( vtkErrorCode::CannotOpenFileError );
		this->AbortParse = 1;
    }
		
	if (!this->AbortParse)
		{
		// Read in the string information.
		// Skip a header line, can use this in future to check format.
		this->IS->getline(this->String, MAX_STRING_LENGTH);
		if (this->IS->fail())
			{
			vtkErrorMacro(<<"Could not read header line");
			this->AbortParse = 1;
			}
			
		// Read number of node string arrays, and their names
		this->Expect("node_arrays");
		num = this->ReadNumber();
		for (i = 0; i < num && !this->AbortParse; i++)
			{
			this->ReadString();;
			graph->MarkAsNodeDataStringArray(this->String);
			}
		
		// Read number of edge string arrays, and their names
		this->Expect("edge_arrays");
		num = this->ReadNumber();
		for (i = 0; i < num && !this->AbortParse; i++)
			{
			this->ReadString();
			graph->MarkAsNodeDataStringArray(this->String);
			}

		// Read number of strings, and then the indices and strings
		this->Expect("strings");
		num = this->ReadNumber();

		if (num > 0)
			{
			strings = vtkStringTable::New();
			for (i = 0; i < num && !this->AbortParse; i++)
				{
				strId = this->ReadNumber();
				this->ReadQuotedString();
				strings->SetString(this->String, strId);
				}
			graph->SetStrings(strings);
			}
		}
		
	delete [] graphFileName;
	delete [] stringFileName;
	
	converter->Delete();
	if (xmlReader)
		{
		xmlReader->Delete();
		}
	if (reader)
		{
		reader->Delete();
		}
	vtkDebugMacro(<<"GraphReader done.");
}


