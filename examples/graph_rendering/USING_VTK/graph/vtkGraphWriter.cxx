/*
 *	File: vtkGraphWriter.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkGraphWriter.h"
#include "vtkObjectFactory.h"
#include "vtkGraph.h"
#include "vtkStringTable.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkXMLUnstructuredGridWriter.h"
#include "vtkGraphToUnstructuredGrid.h"
#include "vtkIntArray.h"
#include "vtkErrorCode.h"

#include <ctype.h>
#include <sys/stat.h>

vtkStandardNewMacro(vtkGraphWriter);
vtkCxxRevisionMacro(vtkGraphWriter, "1.0");



//-----------------------------------------------------------
// Constructors and destructors

vtkGraphWriter::vtkGraphWriter()
{
  this->BaseName = NULL;
	this->XMLFormat = 0;
	this->OS = NULL;
}

vtkGraphWriter::~vtkGraphWriter()
{ 
  if (this->BaseName)
    {
    delete [] this->BaseName;
    this->BaseName = NULL;
    }
	if (this->OS)
		{
		delete this->OS;
		}
}

void vtkGraphWriter::PrintSelf(ostream& os, vtkIndent indent)
{
	this->vtkWriter::PrintSelf(os, indent);
	os << indent << "Basename: ";
	os << ((this->BaseName) ? this->BaseName : "(none)") << "\n";
	os << indent << "XML? ";
	os << (this->XMLFormat ? "yes" : "no") << "\n";
}

//-----------------------------------------------------------

void vtkGraphWriter::SetInput(vtkGraph *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

vtkGraph *vtkGraphWriter::GetInput()
{
  if (this->NumberOfInputs < 1)
    {
    return NULL;
    }
  
  return (vtkGraph *)(this->Inputs[0]);
}

//-----------------------------------------------------------


void vtkGraphWriter::WriteData()
{
  vtkGraph *graph = this->GetInput();

	vtkGraphToUnstructuredGrid *converter;
	vtkUnstructuredGridWriter *writer = NULL;
  vtkXMLUnstructuredGridWriter *xmlWriter = NULL;

	vtkIdType num, i;
	vtkDataArray *array;
	vtkStringTable *strings;
	char *string;
	
	char *graphFileName;
	char *stringFileName;

	vtkDebugMacro(<<"GraphWriter starting.");

	if (!this->BaseName)
		{
		vtkErrorMacro(<<"No base name for input.");
		return;
		}
		
	graphFileName = new char[strlen(this->BaseName)+5];
	stringFileName = new char[strlen(this->BaseName)+5];
	strcpy(graphFileName,this->BaseName);
	strcpy(stringFileName,this->BaseName);
	
	converter = vtkGraphToUnstructuredGrid::New();
	converter->SetInput(graph);
	if (this->XMLFormat)
		{
		strcat(graphFileName,".vtu");
		xmlWriter = vtkXMLUnstructuredGridWriter::New();
		xmlWriter->SetFileName(graphFileName);
		xmlWriter->SetInput(converter->GetOutput());
		xmlWriter->Write();
		}
	else
		{
		strcat(graphFileName,".vtk");
		writer = vtkUnstructuredGridWriter::New();
		writer->SetFileName(graphFileName);
		writer->SetInput(converter->GetOutput());
		writer->Write();
		}
	
	strcat(stringFileName, ".str");
  this->OS = new ofstream(stringFileName, ios::out);
  if (this->OS->fail())
    {
    vtkErrorMacro(<< "Unable to create file: "<< stringFileName);
    delete this->OS;
    this->OS = NULL;
    this->SetErrorCode( vtkErrorCode::CannotOpenFileError );
    }

	if (this->OS)
		{
		// Read in the string information.
		// Skip a header line, can use this in future to check format.
		*this->OS << "# VTK StringTable v1.0\n";

		// Count number of node string arrays
		num = 0;
		for (i = 0; i < graph->GetNodeData()->GetNumberOfArrays(); i++)
			{
			array = graph->GetNodeData()->GetArray(i);
			if (graph->IsNodeDataStringArray(array->GetName()))
				{
				num++;
				}
			}
		// Output count, and the arrays.
		*this->OS << "node_arrays " << num << "\n";
		for (i = 0; i < graph->GetNodeData()->GetNumberOfArrays(); i++)
			{
			array = graph->GetNodeData()->GetArray(i);
			if (graph->IsNodeDataStringArray(array->GetName()))
				{
				*this->OS << array->GetName() << "\n";
				}
			}
		
		// Count number of edge string arrays
		num = 0;
		for (i = 0; i < graph->GetEdgeData()->GetNumberOfArrays(); i++)
			{
			array = graph->GetEdgeData()->GetArray(i);
			if (graph->IsEdgeDataStringArray(array->GetName()))
				{
				num++;
				}
			}
		// Output count, and the arrays.
		*this->OS << "edge_arrays " << num << "\n";
		for (i = 0; i < graph->GetEdgeData()->GetNumberOfArrays(); i++)
			{
			array = graph->GetEdgeData()->GetArray(i);
			if (graph->IsEdgeDataStringArray(array->GetName()))
				{
				*this->OS << array->GetName() << "\n";
				}
			}

		*this->OS << "strings ";
		if (strings = graph->GetStrings())
			{
			// Count number of actual strings;
			num = 0;
			for (vtkIdType n = 0; n < strings->GetNextId(); n++)
				{
				if ((string = strings->GetString(n)))
					{
					num++;
					}
				}
			*this->OS << num << "\n";
			// Write the strings and indices.
			for (vtkIdType p = 0; p < strings->GetNextId(); p++)
				{
				if ((string = strings->GetString(p)))
					{
					*this->OS << p << "\t" << "\"" << string << "\"\n";
					}
				}
			}
		else
			{
			// No string table ... so no strings!
			*this->OS << "0\n";
			}
		}

	delete [] graphFileName;
	delete [] stringFileName;
	
	converter->SetInput(NULL);
	converter->Delete();
	
	if (xmlWriter)
		{
		xmlWriter->Delete();
		}
	if (writer)
		{
		writer->Delete();
		}
	if (this->OS)
		{
		delete this->OS;
		this->OS = NULL;
		}
		
	vtkDebugMacro(<<"GraphWriter done.");
}


