/*
 *	File: vtkGraphStrahlerMetric.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkGraphStrahlerMetric.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"
#include "vtkFloatArray.h"

vtkStandardNewMacro(vtkGraphStrahlerMetric);
vtkCxxRevisionMacro(vtkGraphStrahlerMetric,"1.0");

vtkGraphStrahlerMetric::vtkGraphStrahlerMetric()
{
	this->Normalize = 0;
}

//-------------------------------------------------
// Mergesort implementation, based on code from
// M.A. Weiss, Data Structures and Algorithms in C++.

void vtkGraphStrahlerMetric::Merge(
	float *array,
	float *temp,
	vtkIdType leftPos,
	vtkIdType rightPos,
	vtkIdType rightEnd)
{
	vtkIdType leftEnd = rightPos-1;
	vtkIdType tmpPos = leftPos;
	vtkIdType size = rightEnd - leftPos + 1;
	
	while (leftPos <= leftEnd && rightPos <= rightEnd)
		{
		if (array[leftPos] <= array[rightPos])
			{
			temp[tmpPos++] = array[leftPos++];
			}
		else
			{
			temp[tmpPos++] = array[rightPos++];
			}
		} 
	while (leftPos <= leftEnd)
		{
		temp[tmpPos++] = array[leftPos++];
		}
	while (rightPos <= rightEnd)
		{
		temp[tmpPos++] = array[rightPos++];
		}
	for (vtkIdType i = 0; i < size; i++, rightEnd--)
		{
		array[rightEnd] = temp[rightEnd];
		}
}

void vtkGraphStrahlerMetric::MergeSort(
	float *array,
	float *temp,
	vtkIdType left,
	vtkIdType right)
{
	vtkIdType center;
	
	if (left < right)
		{
		center = (left + right)/2;
		this->MergeSort(array, temp, left, center);
		this->MergeSort(array, temp, center+1, right);
		this->Merge(array, temp, left, center+1, right);
		}
}

// Strahler Computation ---------------------------------------


void vtkGraphStrahlerMetric::ComputeGStrahler(
  vtkIdType node,
  vtkGraph *graph )
{
  vtkIdType nodeIndex;		// index of "node"
  vtkIdType nbor;
	vtkIdType nborIndex;
	vtkIdType nrChildren;
	vtkIdIterator *neighbors;
	
	vtkIdType stackDifference;
  vtkIdType additional;
  vtkIdType available;
  float needed;

	// strahlers of children, and temp array for sorting.
  float *strahlers;
	float *temp;
	vtkIdType size;
	
	
  nodeIndex = graph->GetNodeIndex(node);
	this->Seen[nodeIndex] = 1;

	nrChildren = graph->GetNumberOfChildren(node);
	if (nrChildren == 0) 
    {
    this->Done[nodeIndex] = 1;
    return;
    }
  
	// Node has a subgraph, allocate space to store
	// strahler values of child nodes.
  additional = 0;
	neighbors = graph->GetChildren(node);
	strahlers = new float[nrChildren];
	temp      = new float[nrChildren];
	size = 0;
	
  while (neighbors->HasNext())
    {
    nbor = neighbors->GetNext();
    nborIndex = graph->GetNodeIndex(nbor);
    if (!this->Seen[nborIndex])
	  	{
	  	// normal tree-edge.
	  	this->ComputeGStrahler(nbor, graph);
	  	strahlers[size++] = this->Strahler[nborIndex];
	  	this->UsedStacks[nodeIndex] += this->UsedStacks[nborIndex];
			stackDifference = this->Stacks[nborIndex]-this->UsedStacks[nborIndex];
	  	if (additional < stackDifference)
        {
	    	additional = stackDifference;
        }
	  	}
    else
	  	{
	  	if (this->Done[nborIndex])
	   		{
				// descent edge, to previously processed node
	   		strahlers[size++] = this->Strahler[nborIndex];
	   		if (additional < this->Stacks[nborIndex])
         	{
		 			additional = this->Stacks[nborIndex];
        	}
	   		}
	  	else 
	   		{
				// return edge
	   		strahlers[size++] = 1;
   			this->UsedStacks[nodeIndex]++;
	   		}
	  	}
    }
  neighbors->Delete();
  
  this->Stacks[nodeIndex] = additional + this->UsedStacks[nodeIndex];

	neighbors = graph->GetParents(node);
  while (neighbors->HasNext())
    {
    nbor = neighbors->GetNext();
    if (this->Done[graph->GetNodeIndex(nbor)])
			{
	  	this->UsedStacks[nodeIndex]--;
			}
    }
	neighbors->Delete();

  this->MergeSort(strahlers, temp, 0, size-1);

	additional = 0;
	available = 0;
  while (size > 0)
    {
    needed = strahlers[--size];
    if (needed > available)
	  	{
	  	additional += (int)(needed - available);
	  	available = (int)(needed - 1);
	  	}
    else
			{
	  	available -= 1;
			}
    }
  delete [] strahlers;
	delete [] temp;
  this->Strahler[nodeIndex] = additional;
  this->Done[nodeIndex] = 1;
}

void vtkGraphStrahlerMetric::Execute()
{
  vtkFloatArray  *metric;
  float strahler;

  vtkIdType nrNodes;
  vtkIdType node, index;
	vtkIdIterator *nodes;

  bool finished;

  vtkGraph *input = this->GetInput();
  vtkGraph *output= this->GetOutput();

  // Initialize
  nrNodes = input->GetNumberOfNodes();
  vtkDebugMacro(<<"GraphStrahlerMetric executing.");

  metric = vtkFloatArray::New();
  metric->SetNumberOfValues(nrNodes);
  metric->SetName("strahler");

	this->MaxStrahler = 1.0;

	// working arrays
	this->Strahler = new float[nrNodes];
	this->Stacks = new vtkIdType[nrNodes];
	this->UsedStacks = new vtkIdType[nrNodes];
	this->Seen = new unsigned char[nrNodes];
	this->Done = new unsigned char[nrNodes];
  
  for (vtkIdType i = 0; i < nrNodes; i++)
    {
		this->Seen[i] = 0;
		this->Done[i] = 0;
		this->Strahler[i] = 1;
		this->Stacks[i] = 0;
		this->UsedStacks[i] = 0;
    }
		
  nodes = input->GetNodes();
  while (nodes->HasNext())
    {
		node = nodes->GetNext();
    if (!this->Seen[input->GetNodeIndex(node)])
      {
      this->ComputeGStrahler(node, input);
      }
    }

  do 
    {
    finished = true;
		nodes->Initialize();
    while (nodes->HasNext())
	  	{
			node = nodes->GetNext();
	  	if (!this->Done[input->GetNodeIndex(node)]) 
        {
        this->ComputeGStrahler(node, input);
        finished = false;
        }
	  	}
    }
	while (!finished);

  nodes->Initialize();
  while (nodes->HasNext())
    {
		node = nodes->GetNext();
    index = input->GetNodeIndex(node);
 
		strahler = sqrt(
			(double)this->Strahler[index]*(double)this->Strahler[index]
      + 
      (double)this->Stacks[index]*(double)this->Stacks[index]);
		metric->SetValue(index, strahler);
		if (strahler > this->MaxStrahler)
			{
			this->MaxStrahler = strahler;
			}
    }
	nodes->Delete();

  output->ShallowCopy(input);

	if (this->Normalize)
		{
		for (vtkIdType n = 0; n < input->GetNumberOfNodes(); n++)
			{
			metric->SetValue(n, metric->GetValue(n)/this->MaxStrahler);
			}
		}

  output->GetNodeData()->AddArray(metric);
  metric->Delete();
	
  delete [] Strahler;
	delete [] Stacks;
	delete [] UsedStacks;
	delete [] Seen;
	delete [] Done;
	
	vtkDebugMacro(<<"GraphStrahlerMetric done.");
}


void vtkGraphStrahlerMetric::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
}


