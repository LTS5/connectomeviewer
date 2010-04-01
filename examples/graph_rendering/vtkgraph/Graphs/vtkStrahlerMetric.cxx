/*
 *	File: vtkStrahlerMetric.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 *
 *	Revision history:
 *	2003/10/30 changed member MaxStrahler to float, and imade it
 *		accessible to clients via a "get" method.
 */

#include "vtkStrahlerMetric.h"
#include "vtkObjectFactory.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkStrahlerMetric);
vtkCxxRevisionMacro(vtkStrahlerMetric,"1.01");

vtkStrahlerMetric::vtkStrahlerMetric()
{
  this->MaxStrahler = 0;
	this->Normalize = 0;
}

//----------------------------------------------------------------------------

float vtkStrahlerMetric::CalculateStrahler(
		vtkIdType root, 
		vtkFloatArray *metric,
		vtkGraph *graph)
 {
	float strahler, maxStrahler, *childStrahler;
  bool same;
	vtkIdIterator *children;
  vtkIdType nrChildren = graph->GetNumberOfChildren(root);
	
	// A leaf node has a Strahler value of 0.
  if (nrChildren == 0)
    {
    strahler = 0.0;
    }
  else
    {
		// Non-leaf node: find the Strahler values of the children.
    childStrahler = new float[nrChildren];
		children = graph->GetChildren(root);
    for (vtkIdType i = 0; i < nrChildren; i++)
      {
      childStrahler[i] = this->CalculateStrahler(
				children->GetNext(), 
				metric,
				graph);
      }
		// Determine if the children have the same strahler values
    same = true;
    maxStrahler = childStrahler[0];
    for (vtkIdType j = 1; j < nrChildren; j++) 
			{
      if (maxStrahler != childStrahler[j]) 
				{
				same = false;
				}
      if (maxStrahler <  childStrahler[j]) 
				{
				maxStrahler = childStrahler[j];
				}
			}
		// Calculate the strahler value for this node
    strahler = (same) ? maxStrahler + nrChildren - 1
                      : maxStrahler + nrChildren - 2;
  
    delete [] childStrahler;
		children->Delete();
    }
	// Record the strahler value within the array.
  metric->InsertValue(graph->GetNodeIndex(root), strahler);
	if (strahler > this->MaxStrahler)
		{
		this->MaxStrahler = strahler;
		}
  return strahler;
}


void vtkStrahlerMetric::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkGraph *output= this->GetOutput();
	
	// The output model should share the data of the input.
	output->ShallowCopy(input);
	vtkIdIterator *roots;
	vtkFloatArray *metric;

  vtkDebugMacro(<<"StrahlerMetric executing." );

	// Create a new array to hold the metric.
	metric = vtkFloatArray::New();
	metric->SetName("strahler");
	metric->SetNumberOfValues(input->GetNumberOfNodes());

	this->MaxStrahler = 1;

	// Calculate the metric, starting from each root node found
	// in the input forest.
	roots = input->GetSources();
	while (roots->HasNext())
		{
  	this->CalculateStrahler(
				roots->GetNext(), 
				metric,
				input);
		}
	roots->Delete();

	if (this->Normalize)
		{
		for (vtkIdType i = 0; i < input->GetNumberOfNodes(); i++)
			{
			metric->SetValue(i, metric->GetValue(i)/this->MaxStrahler);
			}
		}
	
	output->GetNodeData()->AddArray(metric);
  
	vtkDebugMacro(<<"StrahlerMetric done.");
}


void vtkStrahlerMetric::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkSource::PrintSelf(os, indent);
}
