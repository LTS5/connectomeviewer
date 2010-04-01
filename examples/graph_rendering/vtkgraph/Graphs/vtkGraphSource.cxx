/*
 *	File: vtkGraphSource.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkGraphSource.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkGraphSource, "1.0");

//------------------------------------------------------

vtkGraphSource::vtkGraphSource()
{
  this->vtkSource::SetNthOutput(0,vtkGraph::New());
  // Releasing data for pipeline parallism.
  // Filters will know it is empty. 
  this->Outputs[0]->ReleaseData();
  this->Outputs[0]->Delete();
}

void vtkGraphSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//------------------------------------------------------

vtkGraph *vtkGraphSource::GetOutput()
{
  if (this->NumberOfOutputs < 1)
    {
    return NULL;
    }
  
  return (vtkGraph *)(this->Outputs[0]);
}

vtkGraph *vtkGraphSource::GetOutput(int idx)
{
  return (vtkGraph *)(this->vtkSource::GetOutput(idx)); 
}

void vtkGraphSource::SetOutput(vtkGraph *output)
{
  this->vtkSource::SetNthOutput(0, output);
}

