/*
 *	File: vtkGMLWriter.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.
 *
 */

#include "vtkGMLWriter.h"
#include "vtkObjectFactory.h"
#include "vtkFieldData.h"
#include "vtkDataArray.h"
#include "vtkErrorCode.h"

#include "vtkBitArray.h"
#include "vtkCharArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkIntArray.h"
#include "vtkUnsignedIntArray.h"
#include "vtkShortArray.h"
#include "vtkUnsignedShortArray.h"
#include "vtkLongArray.h"
#include "vtkUnsignedLongArray.h"
#include "vtkIdTypeArray.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"

vtkCxxRevisionMacro(vtkGMLWriter, "1.0");
vtkStandardNewMacro(vtkGMLWriter);

vtkGMLWriter::vtkGMLWriter()
{
  this->FileName = NULL;
}

vtkGMLWriter::~vtkGMLWriter()
{
  if (this->FileName)
		{
		delete [] this->FileName;
		}
}


//----------------------------------------------------------------------------

void vtkGMLWriter::SetInput(vtkGraph *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

vtkGraph *vtkGMLWriter::GetInput()
{
  if (this->NumberOfInputs < 1)
    {
    return NULL;
    }
  
  return (vtkGraph *)(this->Inputs[0]);
}

// Open a GML file for writing. Returns NULL if error.
ostream *vtkGMLWriter::OpenGMLFile()
{
  ostream *fptr;
  
  if (!this->FileName)
    {
    vtkErrorMacro(<< "No FileName specified! Can't write!");
    this->SetErrorCode(vtkErrorCode::NoFileNameError);
    return NULL;
    }
  
  vtkDebugMacro(<<"Opening file for writing...");

  fptr = new ofstream(this->FileName, ios::out);

  if (fptr->fail())
    {
    vtkErrorMacro(<< "Unable to open file: "<< this->FileName);
    this->SetErrorCode(vtkErrorCode::CannotOpenFileError);
    delete fptr;
    return NULL;
    }

  return fptr;
}

// Close GML file.
void vtkGMLWriter::CloseGMLFile(ostream *fp)
{
  vtkDebugMacro(<<"Closing file\n");
  
  if ( fp != NULL )
    {
    delete fp;
    fp = NULL;
    }
}

void vtkGMLWriter::WriteDeclarations(
	ostream *fp, 
	int location,
	vtkFieldData *fd)
{
	vtkDataArray *array;
	
	for (int i = 0; i < fd->GetNumberOfArrays(); i++)
		{
		array = fd->GetArray(i);
		*fp << "  ";
		*fp << ((location == NODE_DATA) ? "node_data" : "edge_data");
		*fp << " " << array->GetName() << " ";
		switch (array->GetDataType())
			{
			case VTK_BIT:
				*fp << "bit";
				break;
    	case VTK_CHAR:
				*fp << "char";
				break;
			case VTK_UNSIGNED_CHAR:
				*fp << "unsigned_char";
				break;
			case VTK_SHORT:
				*fp << "short";
				break;
			case VTK_UNSIGNED_SHORT:
				*fp << "unsigned_short";
				break;
			case VTK_INT:
				*fp << "int";
				break;
			case VTK_UNSIGNED_INT:
				*fp << "unsigned_int";
				break;
			case VTK_LONG:
				*fp << "long";
				break;
    	case VTK_UNSIGNED_LONG:
				*fp << "unsigned_long";
				break;
    	case VTK_FLOAT:
				*fp << "float";
				break;
    	case VTK_DOUBLE:
				*fp << "double";
				break;
    	case VTK_ID_TYPE:
				if (this->GetInput()->IsDataStringArray(location, array->GetName()))
					{
					*fp << "string";
					}
				else
					{
					*fp << "int";
					}
				break;
			default:
				vtkErrorMacro(<<"Unsupported data type.");
				break;
			}
		*fp << "\n";
		}
}

// Write out data to file specified.
void vtkGMLWriter::WriteValue(
	vtkDataArray *data,
	ostream *fp, 
	vtkIdType idx)
{
  int dataType = data->GetDataType();
	int i;
  char c;
	short s;
	long l;
	vtkIdType id;
	float f;
	double d;

	switch (dataType)
    {
    case VTK_BIT:
      i = ((vtkBitArray *)data)->GetValue(idx);
      *fp << i;
    	break;
    case VTK_CHAR:
      c = ((vtkCharArray *)data)->GetValue(idx);
      *fp << c;
    	break;
    case VTK_UNSIGNED_CHAR:
      c = ((vtkUnsignedCharArray *)data)->GetValue(idx);
      *fp << c;
    	break;
    case VTK_SHORT:
      s = ((vtkShortArray *)data)->GetValue(idx);
      *fp << s;
    	break;
    case VTK_UNSIGNED_SHORT:
      s = ((vtkUnsignedShortArray *)data)->GetValue(idx);
      *fp << s;
    	break;
    case VTK_INT:
      i = ((vtkShortArray *)data)->GetValue(idx);
      *fp << i;
    	break;
    case VTK_UNSIGNED_INT:
      i = ((vtkUnsignedIntArray *)data)->GetValue(idx);
      *fp << i;
    	break;
    case VTK_LONG:
      l = ((vtkLongArray *)data)->GetValue(idx);
      *fp << l;
    	break;
    case VTK_UNSIGNED_LONG:
      l = ((vtkUnsignedLongArray *)data)->GetValue(idx);
      *fp << l;
    	break;
    case VTK_FLOAT:
      f = ((vtkFloatArray *)data)->GetValue(idx);
      *fp << f;
    	break;
    case VTK_DOUBLE:
      d = ((vtkDoubleArray *)data)->GetValue(idx);
      *fp << d;
    	break;
    case VTK_ID_TYPE:
      id = ((vtkIdTypeArray *)data)->GetValue(idx);
      *fp << id;
    	break;
    default:
      vtkErrorMacro(<<"Type currently not supported");
    }

  fp->flush();
  if (fp->fail())
    {
    this->SetErrorCode(vtkErrorCode::OutOfDiskSpaceError);
    }
}



// Write FieldData data to file
void vtkGMLWriter::WriteData()
{
  ostream *fp;
  vtkGraph *input = this->GetInput();
	vtkIdIterator *it;
	vtkFieldData *data;
	vtkDataArray *array;
	vtkIdType node, edge, index;
	int i;
	vtkStringTable *strings;
	const char *name;
	vtkPoints *layout;
	double point[3];
	vtkIdType *bends;

  vtkDebugMacro(<<"Writing Graph to GML file ...");

	if (!input)
		{
		return;
		}
  if ( !(fp=this->OpenGMLFile()))
    {
    return;
    }
		
	strings = input->GetStrings();
	layout = input->GetLayout();

	*fp << "graph [\n";
	*fp << "  node_count " << input->GetNumberOfNodes() << "\n";
	*fp << "  edge_count " << input->GetNumberOfEdges() << "\n";
	*fp << "\n";

	this->WriteDeclarations(fp, NODE_DATA, input->GetNodeData());
	this->WriteDeclarations(fp, EDGE_DATA, input->GetEdgeData());
	*fp << "\n";
		
	data = input->GetNodeData();
	it = input->GetNodes();
	while (it->HasNext())
		{
		node = it->GetNext();
		index = input->GetNodeIndex(node);
		*fp << "  node [\n";
		*fp << "    id " << node << "\n";
		*fp << "    degree " << input->GetDegree(node) << "\n";
		// If the node has a position, output it.
		if (index < layout->GetNumberOfPoints())
			{
			layout->GetPoint(index, point);
      *fp << "    point ";
			*fp << point[0] << " " << point[1] << " " << point[2] << "\n";
			}
		for (i = 0; i < data->GetNumberOfArrays(); i++)
			{
			array = data->GetArray(i);
			name = array->GetName();
			*fp << "    " << array->GetName() << " ";
			if (input->IsNodeDataStringArray(name) && strings)
				{
				*fp << "\"";
				*fp << strings->GetString(
					((vtkIdTypeArray*)array)->GetValue(index));
				*fp << "\"";
				}
			else
				{
				this->WriteValue(array, fp, index);
				}
			*fp << "\n";
			}
		*fp << "  ]\n";
		}
  it->Delete();

	data = input->GetEdgeData();
	it = input->GetEdges();
	while (it->HasNext())
		{
		edge = it->GetNext();
		index = input->GetEdgeIndex(edge);
		*fp << "  edge [\n";
		*fp << "    id " << edge << "\n";
		*fp << "    source " << input->GetSource(edge) << "\n";
		*fp << "    target " << input->GetTarget(edge) << "\n";
		for (i = 0; i < data->GetNumberOfArrays(); i++)
			{
			array = data->GetArray(i);
			name = array->GetName();
			*fp << "    " << name << " ";
			if (input->IsEdgeDataStringArray(name) && strings)
				{
				*fp << "\"";
				*fp << strings->GetString(
						((vtkIdTypeArray*)array)->GetValue(index));
				*fp << "\"";
				}
			else
				{
				this->WriteValue(array, fp, index);
				}
			*fp << "\n";
			}
		// If the edge has a bend, output the bend points
		if (input->GetNumberOfBends(edge) > 0)
			{
			*fp << "    line [\n";
			bends = input->GetBendArray(edge);
			for (int b = 0; b < input->GetNumberOfBends(edge); b++)
				{
				input->GetBendPosition(bends[b], point);
				*fp << "      point ";
				*fp << point[0] << " " << point[1] << " " << point[2] << "\n";
				}
			*fp << "    ]\n";
			}
		*fp << "  ]\n";
		}
  it->Delete();

	*fp << "]\n";

  this->CloseGMLFile(fp);  

	vtkDebugMacro(<<"GMLWriter done");
}

void vtkGMLWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "File Name: " 
     << (this->FileName ? this->FileName : "(none)") << "\n";
}

