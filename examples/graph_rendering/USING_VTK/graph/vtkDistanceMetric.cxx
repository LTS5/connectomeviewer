/*
 *	File: vtkDistanceMetric.cxx
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

#include "vtkDistanceMetric.h"
#include "vtkObjectFactory.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkDistanceMetric);
vtkCxxRevisionMacro(vtkDistanceMetric,"1.00");

vtkDistanceMetric::vtkDistanceMetric()
{
	this->ArrayName = NULL;
  this->MaxDistance = 0;
	this->Normalize = 0;
	this->StartMode = 0;
}

vtkDistanceMetric::~vtkDistanceMetric()
{
	if (this->ArrayName)
		{
		delete [] this->ArrayName;
		}
}

void vtkDistanceMetric::SetStartModeToUseNode(vtkIdType nodeId)
{
	this->StartMode = 0;
	this->StartNode = nodeId;
}

void vtkDistanceMetric::SetStartModeToUseSources()
{
	this->StartMode = 1;
}

void vtkDistanceMetric::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkGraph *output= this->GetOutput();
	
	vtkIdIterator *roots, *children;
	vtkFloatArray *metric;
	vtkIdType node, child, index;
	float dist;
	vtkIdType *queue;
	vtkIdType head, tail;

  vtkDebugMacro(<<"DistanceMetric executing." );

	if (!input)
		{
		return;
		}
	if (!this->ArrayName)
		{
		vtkErrorMacro(<<"No array name set.");
		return;
		}
	if (this->StartMode == 0 && !input->HasNode(this->StartNode))
		{
		vtkErrorMacro(<<"Start node not present in graph.");
		return;
		}

	// The output model should share the data of the input.
	output->ShallowCopy(input);

	// Create a new array to hold the metric; initially
	// no distances are known (marked as -1).
	metric = vtkFloatArray::New();
	metric->SetName(this->ArrayName);
	metric->SetNumberOfValues(input->GetNumberOfNodes());
	for (vtkIdType i = 0; i < input->GetNumberOfNodes(); i++)
		{
		metric->SetValue(i, -1);
		}

	this->MaxDistance = 0;

	// Queue of nodes for traversal
	queue = new vtkIdType[input->GetNumberOfNodes()];
	head = 0;
	tail = 0;

	if (this->StartMode == 0)
		{
		// Start from an explicit start node
		queue[head++] = this->StartNode;
		metric->SetValue(input->GetNodeIndex(this->StartNode), 0);
		}
	else
		{
		// Start from each source node in the graph
		roots = input->GetSources();
		while (roots->HasNext())
			{
			node = roots->GetNext();
  		queue[head++] = node;
			metric->SetValue(input->GetNodeIndex(node), 0);
			}
		roots->Delete();
		}

	while (head != tail)
		{
		node = queue[tail++];
		children = input->GetChildren(node);
		dist = metric->GetValue(input->GetNodeIndex(node));
		if (dist > this->MaxDistance)
			{
			this->MaxDistance = dist;
			}
		while (children->HasNext())
			{
			child = children->GetNext();
			index = input->GetNodeIndex(child);
			if (metric->GetValue(index) < 0)
				{
				metric->SetValue(index, dist+1);
				queue[head++] = child;
				}
			}
		children->Delete();
		}
	delete [] queue;
	
	if (this->Normalize)
		{
		for (vtkIdType n = 0; n < input->GetNumberOfNodes(); n++)
			{
			metric->SetValue(n, metric->GetValue(n)/this->MaxDistance);
			}
		}
	
	output->GetNodeData()->AddArray(metric);
  
	vtkDebugMacro(<<"DistanceMetric done.");
}


void vtkDistanceMetric::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkSource::PrintSelf(os, indent);
	os << "Normalize: " << (this->Normalize ? "on\n" : "off\n");
	os << "StartMode: " << (this->StartMode ? "from node\n" : "from sources\n");
	if (!this->StartMode)
		{
		os << indent << "StartNode: " << this->StartNode << "\n";
		}
}


