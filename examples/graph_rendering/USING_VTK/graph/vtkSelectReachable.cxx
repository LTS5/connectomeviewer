/*
 *	File: vtkSelectReachable.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkSelectReachable.h"
#include "vtkObjectFactory.h"
#include "vtkBitArray.h"
#include "vtkIdIterator.h"

//-------------------------------------------------


vtkStandardNewMacro(vtkSelectReachable);
vtkCxxRevisionMacro(vtkSelectReachable, "1.0");


vtkSelectReachable::vtkSelectReachable()
{
	this->SelectorName = NULL;
  this->StartNodeSet = 0;
  this->SearchMode = SEARCH_UNDIRECTED;
	this->MaxAtInfinity = 1;
}

vtkSelectReachable::~vtkSelectReachable()
{
	if (this->SelectorName)
		{
		delete [] this->SelectorName;
		}
}

void vtkSelectReachable::SetStartNode(vtkIdType node)
{
	this->StartNode = node;
	this->StartNodeSet = 1;
}

vtkIdType vtkSelectReachable::GetStartNode()
{
	return this->StartNode;
}

void vtkSelectReachable::SetMaxDistance(vtkIdType dist)
{
	this->MaxDistance = dist;
	this->MaxAtInfinity = 0;
}

void vtkSelectReachable::SetMaxDistanceToInfinity()
{
	this->MaxAtInfinity = 1;
}

vtkIdType vtkSelectReachable::GetMaxDistance()
{
	return (this->MaxAtInfinity ? -1 : this->MaxDistance);
}

//-------------------------------------------------
// Filter execution

void vtkSelectReachable::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkGraph *output = this->GetOutput();
	
	vtkBitArray *selector;
	vtkIdType node, index, nrNodes, i, distance;
	vtkIdIterator *neighbours;
	
	vtkIdType *queue;
	vtkIdType *dist;
	vtkIdType head, tail;
	
  vtkDebugMacro(<<"SelectReachable executing." );
	
	if (!input)
		{
		return;
		}
	if (!this->SelectorName)
		{
		vtkErrorMacro(<<"Selector name must be set.");
		return;
		}

	nrNodes = input->GetNumberOfNodes();
	// create the selector array.
	selector = vtkBitArray::New();
	selector->SetNumberOfTuples(nrNodes);
	selector->SetName(this->SelectorName);

	for (i = 0; i < nrNodes; i++)
		{
		selector->SetValue(i, 0);
		}
	// Shallow-copy graph to output and add selector.
	output->ShallowCopy(input);
	output->GetNodeData()->AddArray(selector);

	if (this->MaxAtInfinity)
		{
		this->MaxDistance = nrNodes + 1;
		}
	// If no start node is set, or there are nodes that could be marked,
	// we are done.  Adjust ref count on new array, and return.
	if (!this->StartNodeSet || nrNodes == 0 || this->MaxDistance == 0)
		{
		selector->Delete();
		return;
		}
	if (!input->HasNode(this->StartNode))
		{
		vtkWarningMacro(<<"Startnode not found in graph!");
		selector->Delete();
		return;
		}

	dist  = new vtkIdType[nrNodes];
	for (i = 0; i < nrNodes; i++)
		{
		dist[i] = -1;
		}
	
	queue = new vtkIdType[nrNodes];
	head = 0;
	tail = 0;

	queue[head++] = this->StartNode;
	dist[input->GetNodeIndex(this->StartNode)] = 0;
	
	while (head != tail)
		{
		node = queue[tail++];
		index = input->GetNodeIndex(node);
		distance = dist[index]; 
		selector->SetValue(index, 1);
		if (distance == this->MaxDistance)
			{
			continue;
			}
		switch(this->SearchMode)
			{
			case SEARCH_UNDIRECTED:
				neighbours = input->GetNeighbors(node);
				break;
			case SEARCH_DIRECTED:
				neighbours = input->GetChildren(node);
				break;
			case SEARCH_REVERSED:
				neighbours = input->GetParents(node);
				break;
			default:
				vtkErrorMacro(<<"Unknown search mode");
				selector->Delete();
				delete [] queue;
				delete [] dist;
				return;
			}
		while (neighbours->HasNext())
			{
			node = neighbours->GetNext();
			index = input->GetNodeIndex(node);
			if (dist[index] < 0)
				{
				queue[head++] = node;
				dist[index] = 1 + distance;
				}
			}
		neighbours->Delete();
		}

	// Update ref count.
	selector->Delete();
	delete [] queue;
	delete [] dist;

  vtkDebugMacro(<<"SelectReachable done.");
}


void vtkSelectReachable::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkGraphToGraphFilter::PrintSelf(os, indent);
	os << indent << "Selector: ";
	os << (this->SelectorName ? this->SelectorName : "(null)") << "\n";

	os << indent << "Start node: ";
	if (this->StartNodeSet)
		{
		os << this->StartNode << "\n";
		}
	else
		{
		os << "(none)\n";
		}
	
	os << indent << "Search mode: ";
	switch (this->SearchMode)
		{
		case SEARCH_UNDIRECTED:
			os << "undirected\n";
			break;
		case SEARCH_DIRECTED:
			os << "directed\n";
			break;
		case SEARCH_REVERSED:
			os << "reverse (directed)\n";
			break;
		default:
			os << "unknown mode!\n";
		}
	
	os << indent << "Max distance: ";
	if (this->MaxAtInfinity)
		{
		os << "infinity\n";
		}
	else
		{
		os << this->MaxDistance << "\n";
		}
}


