/*
 *	File: vtkGraphToPolyDataFilter.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkGraphToPolyDataFilter.h"
#include "vtkGraph.h"

vtkCxxRevisionMacro(vtkGraphToPolyDataFilter, "1.0");

//-------------------------------------------------------

void vtkGraphToPolyDataFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//-------------------------------------------------------

// Specify the input data or filter.
void vtkGraphToPolyDataFilter::SetInput(vtkGraph *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

vtkGraph *vtkGraphToPolyDataFilter::GetInput()
{
  if (this->NumberOfInputs < 1)
    {
    return NULL;
    }
  
  return (vtkGraph *)(this->Inputs[0]);
}

void vtkGraphToPolyDataFilter::ComputeInputUpdateExtents( 
	vtkDataObject *output)
{
  this->vtkPolyDataSource::ComputeInputUpdateExtents(output);

  if (!this->GetInput())
    {
    return;
    }
  // assume that we cannot handle more than the requested extent.
  this->GetInput()->RequestExactExtentOn();
}

