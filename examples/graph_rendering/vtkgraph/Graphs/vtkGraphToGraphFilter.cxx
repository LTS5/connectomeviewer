/*
 *	File: vtkGraphToGraphFilter.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkGraphToGraphFilter.h"
#include "vtkGraph.h"

vtkCxxRevisionMacro(vtkGraphToGraphFilter, "1.0");

//--------------------------------------------------------

void vtkGraphToGraphFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//--------------------------------------------------------

void vtkGraphToGraphFilter::SetInput(vtkGraph *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

// Specify the input data or filter.
vtkGraph *vtkGraphToGraphFilter::GetInput()
{
  if (this->NumberOfInputs < 1)
    {
    return NULL;
    }
  
  return (vtkGraph *)(this->Inputs[0]);
}


void vtkGraphToGraphFilter::ComputeInputUpdateExtents(
	vtkDataObject *output)
{
  this->vtkGraphSource::ComputeInputUpdateExtents(output);

  if (!this->GetInput())
    {
    return;
    }
  // assume that we cannot handle more than the requested extent.
  this->GetInput()->RequestExactExtentOn();
}

