/*
 *	File: vtkGraphStringMapper.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.
 *
 *	This implementation is based on vtkLabeledDataMapper.
 */

#include "vtkGraphStringMapper.h"

#include "vtkActor2D.h"
#include "vtkGraph.h"
#include "vtkObjectFactory.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"
#include "vtkStringTable.h"
#include "vtkIdTypeArray.h"

vtkCxxRevisionMacro(vtkGraphStringMapper, "1.0");
vtkStandardNewMacro(vtkGraphStringMapper);

vtkCxxSetObjectMacro(vtkGraphStringMapper,LabelTextProperty,vtkTextProperty);


//----------------------------------------------------------

vtkGraphStringMapper::vtkGraphStringMapper()
{
  this->Input = NULL;

  this->LabelFormat = new char[20]; 
  strcpy(this->LabelFormat,"%s");
	this->ArrayName = NULL;
 
  this->NumberOfLabels = 0;
  this->NumberOfLabelsAllocated = 50;

  this->TextMappers = new vtkTextMapper * [this->NumberOfLabelsAllocated];
  for (int i=0; i<this->NumberOfLabelsAllocated; i++)
    {
    this->TextMappers[i] = vtkTextMapper::New();
    }

  this->LabelTextProperty = vtkTextProperty::New();
  this->LabelTextProperty->SetFontSize(12);
  this->LabelTextProperty->SetBold(1);
  this->LabelTextProperty->SetItalic(1);
  this->LabelTextProperty->SetShadow(1);
  this->LabelTextProperty->SetFontFamilyToArial();
}

vtkGraphStringMapper::~vtkGraphStringMapper()
{
  if (this->LabelFormat)
    {
    delete [] this->LabelFormat;
    }
	if (this->ArrayName)
		{
		delete [] this->ArrayName;
		}
		
  if (this->TextMappers != NULL )
    {
    for (int i=0; i < this->NumberOfLabelsAllocated; i++)
      {
      this->TextMappers[i]->Delete();
      }
    delete [] this->TextMappers;
    }
  
  this->SetInput(NULL);
  this->SetLabelTextProperty(NULL);
}



void vtkGraphStringMapper::ReleaseGraphicsResources(vtkWindow *win)
{
  if (this->TextMappers != NULL )
    {
    for (int i=0; i < this->NumberOfLabelsAllocated; i++)
      {
      this->TextMappers[i]->ReleaseGraphicsResources(win);
      }
    }
}


//----------------------------------------------------------

void vtkGraphStringMapper::RenderOverlay(vtkViewport *viewport, 
                                         vtkActor2D *actor)
{
  int i;
  double x[3];
  vtkGraph *input=this->GetInput();

  if ( ! input )
    {
    vtkErrorMacro(<<"Need input graph to render strings");
    return;
    }
  for (i=0; i<this->NumberOfLabels; i++)
    {
    this->Input->GetLayout()->GetPoint(i,x);
    actor->GetPositionCoordinate()->SetCoordinateSystemToWorld();
    actor->GetPositionCoordinate()->SetValue(x);
    this->TextMappers[i]->RenderOverlay(viewport, actor);
    }
}


void vtkGraphStringMapper::RenderOpaqueGeometry(vtkViewport *viewport, 
                                                vtkActor2D *actor)
{
  int i;
  char string[4096];
  double x[3], srcp[3], dstp[3];
	vtkIdType src, dst;
  vtkIdTypeArray *indices;
	vtkStringTable *strings;

  vtkGraph *input = this->GetInput();

	vtkDebugMacro(<<"GraphStringMapper running.");
  if ( ! input )
    {
    vtkErrorMacro(<<"Need input data to render labels");
    return;
    }
  vtkTextProperty *tprop = this->LabelTextProperty;
  if (!tprop)
    {
    vtkErrorMacro(<<"Need text property to render labels");
    return;
    }
		
  input->Update();
	strings = input->GetStrings();
	if (!strings)
		{
		vtkErrorMacro(<<"No strings in input graph.");
		return;
		}
	if (!this->ArrayName)
		{
		vtkErrorMacro(<<"No array name given");
		return;
		}
	if (this->Location == NODE_DATA)
		{
		indices = vtkIdTypeArray::SafeDownCast(
			input->GetNodeData()->GetArray(this->ArrayName));
		this->NumberOfLabels = input->GetNumberOfNodes();
		}
	else
		{
		indices = vtkIdTypeArray::SafeDownCast(
			input->GetEdgeData()->GetArray(this->ArrayName));
		this->NumberOfLabels = input->GetNumberOfEdges();
		}
	if (!indices)
		{
		vtkErrorMacro(<<"No array of string indices");
		return;
		}
		
  // Check to see whether we have to rebuild everything
  if ( this->GetMTime() > this->BuildTime || 
       input->GetMTime() > this->BuildTime ||
       tprop->GetMTime() > this->BuildTime)
    {
    vtkDebugMacro(<<"Rebuilding labels");
    if (this->NumberOfLabels > this->NumberOfLabelsAllocated )
      {
      // delete old stuff
      for (i=0; i < this->NumberOfLabelsAllocated; i++)
        {
        this->TextMappers[i]->Delete();
        }
      delete [] this->TextMappers;

      this->NumberOfLabelsAllocated = this->NumberOfLabels;
      this->TextMappers = new vtkTextMapper * [this->NumberOfLabelsAllocated];
      for (i=0; i<this->NumberOfLabelsAllocated; i++)
        {
        this->TextMappers[i] = vtkTextMapper::New();
        }
      }//if we have to allocate new text mappers
    
    for (i=0; i < this->NumberOfLabels; i++)
      {
      sprintf(
					string, 
					this->LabelFormat, 
          strings->GetString(indices->GetValue(i)));
      this->TextMappers[i]->SetInput(string);
      this->TextMappers[i]->SetTextProperty(tprop);
      }
    this->BuildTime.Modified();
    }

  for (i=0; i<this->NumberOfLabels; i++)
    {
		if (this->Location == NODE_DATA)
			{
    	input->GetLayout()->GetPoint(i,x);
			}
		else
			{
			input->GetEdge(input->GetEdgeId(i), src, dst);
			input->GetLayout()->GetPoint(src, srcp);
			input->GetLayout()->GetPoint(dst, dstp);
			x[0] = (srcp[0] + dstp[0])/2.0;
			x[1] = (srcp[1] + dstp[1])/2.0;
			x[2] = (srcp[2] + dstp[2])/2.0;
			}
    actor->GetPositionCoordinate()->SetCoordinateSystemToWorld();
    actor->GetPositionCoordinate()->SetValue(x);
    this->TextMappers[i]->RenderOpaqueGeometry(viewport, actor);
    }
	vtkDebugMacro(<<"GraphStringMapper done.");
}


//----------------------------------------------------------

void vtkGraphStringMapper::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  if ( this->Input )
    {
    os << indent << "Input: (" << this->Input << ")\n";
    }
  else
    {
    os << indent << "Input: (none)\n";
    }

  if (this->LabelTextProperty)
    {
    os << indent << "Label Text Property:\n";
    this->LabelTextProperty->PrintSelf(os,indent.GetNextIndent());
    }
  else
    {
    os << indent << "Label Text Property: (none)\n";
    }
	os << indent << "String array: ";
	if (this->ArrayName)
		{
		os << this->ArrayName << "\n";
		}
	else
		{
		os << "(not set)\n";
		}
  os << indent << "Label Format: " << this->LabelFormat << "\n";
}

