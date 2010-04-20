/*
 *	File: vtkEdgeSpanTree.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkEdgeSpanTree.h"
#include "vtkObjectFactory.h"
#include "vtkFloatArray.h"
#include "vtkPriorityQueue.h"
#include "vtkBitArray.h"
#include "vtkIdTypeArray.h"

//--------------------------------------------------

vtkStandardNewMacro(vtkEdgeSpanTree);
vtkCxxRevisionMacro(vtkEdgeSpanTree,"1.0");


//--------------------------------------------------

vtkEdgeSpanTree::vtkEdgeSpanTree()
{
	this->ArrayName = NULL;
}

vtkEdgeSpanTree::~vtkEdgeSpanTree()
{
	if (this->ArrayName)
		{
		delete [] this->ArrayName;
		}
}


//--------------------------------------------------

void vtkEdgeSpanTree::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkGraph *output= this->GetOutput();
	vtkFloatArray *property = NULL;
	vtkFloatArray *edgeCost;
	vtkFloatArray *nodeCost;
	vtkBitArray *known;
	vtkIdTypeArray *route;

	double range[2];
	float *tuple;
	
	vtkIdType e, eID, n, nID, nIndex;
	vtkIdType firstUnknown;


  vtkIdType nrNodes;
	vtkIdType node, edge, src, dst;
	vtkIdIterator *nodes, *edges;
	
	vtkPriorityQueue *pqueue;

  vtkDebugMacro(<<"EdgeSpanTree executing." );

	if (!this->ArrayName)
		{
		vtkErrorMacro(<<"No property name set.");
		return;
		}
	property = vtkFloatArray::SafeDownCast(
		input->GetEdgeData()->GetArray(this->ArrayName));
	if (!property)
		{
		vtkErrorMacro(<<"Property not found in input edges.\n");
		return;
		}

	nrNodes = input->GetNumberOfNodes();
	if (nrNodes == 0)
		{
		vtkWarningMacro(<<"No nodes in graph.");
		return;
		}

	edgeCost = vtkFloatArray::New();
	edgeCost->DeepCopy(property);
	edgeCost->GetRange(range);
	
	// Copy nodes from input to output, along with data tuples.
	tuple = NULL;
	if (input->GetNodeData()->GetNumberOfComponents() > 0)
		{
		output->GetNodeData()->CopyStructure(input->GetNodeData());
		output->GetNodeData()->SetNumberOfTuples(
			input->GetNodeData()->GetNumberOfTuples());
		tuple = new float[input->GetNodeData()->GetNumberOfComponents()];
		}
	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		output->CreateNode(node);
		if (tuple)
			{
			output->GetNodeData()->InsertTuple(
				output->GetNodeIndex(node),
				input->GetNodeData()->GetTuple(
					input->GetNodeIndex(node)));
			}
		}	
	if (tuple)
		{
		delete [] tuple;
		}	
	nodes->Delete();

	
	// We want a MAXIMAL spannng tree, so subtract all distances
	// from the max distance.
	for (vtkIdType m = 0; m < edgeCost->GetNumberOfTuples(); m++)
		{
		edgeCost->SetValue(m, (float)range[1] - edgeCost->GetValue(m));
		}

	nrNodes = output->GetNumberOfNodes();
	
	nodeCost = vtkFloatArray::New();
	known = vtkBitArray::New();
	route = vtkIdTypeArray::New();
	
	nodeCost->SetNumberOfTuples(nrNodes);
	known->SetNumberOfTuples(nrNodes);
	route->SetNumberOfTuples(nrNodes);
	
	
	// We know that input has at least one edge component!
	tuple = new float[input->GetEdgeData()->GetNumberOfComponents()];

	output->GetEdgeData()->CopyStructure(input->GetEdgeData());
	output->GetEdgeData()->SetNumberOfTuples(
		input->GetEdgeData()->GetNumberOfTuples());

	// Initialize tables.
	for (vtkIdType i = 0; i < nrNodes; i++)
		{
		nodeCost->SetValue(i, VTK_FLOAT_MAX);
		known->SetValue(i, 0);
		route->SetValue(i, -1);
		}
	nodeCost->SetValue(0, 0.0);

	pqueue = vtkPriorityQueue::New();
	pqueue->Allocate(nrNodes, 100);
	
	// If the input graph is disconnected, not all nodes
	// will be visited even when traversal is complete.
	// So we keep track of the index at which the first
	// unknown node is found.

	firstUnknown = 0;
	while (firstUnknown < nrNodes)
		{
		pqueue->Insert(0.0, firstUnknown);
		
		while (pqueue->GetNumberOfItems() > 0)
			{
			nIndex = pqueue->Pop(0);
			node   = input->GetNodeId(nIndex);
			known->SetValue(nIndex, 1);
			edges = input->GetEdges(node); 
			while (edges->HasNext())
				{
				eID = edges->GetNext();
				e   = input->GetEdgeIndex(eID);
				src = input->GetSource(eID);
				dst = input->GetTarget(eID);
				nID = ((src == node) ? dst : src);
				n   = input->GetNodeIndex(nID);
				if (nodeCost->GetValue(n) > edgeCost->GetValue(e))
					{
					if (!known->GetValue(n))
						{
						nodeCost->SetValue(n, edgeCost->GetValue(e));
						route->SetValue(n, eID);
						pqueue->DeleteId(n);
						pqueue->Insert(nodeCost->GetValue(n), n);
						}
					}
				}
			edges->Delete();
			}
		firstUnknown = 0;
		while (firstUnknown < nrNodes && known->GetValue(firstUnknown))
			{
			firstUnknown++;
			}
		}
		
	for (vtkIdType r = 0; r < nrNodes; r++)
		{
		node = output->GetNodeId(r);
		edge = route->GetValue(r);
		if (edge == -1)
			{
			continue;
			}
		src = input->GetSource(edge);
		dst = input->GetTarget(edge);
		// If we are the target of this edge, then insert the
		// edge "as is" into the output, but otherwise reverse
		// it.
		if (node == input->GetTarget(edge))
			{
			output->CreateEdge(edge, input->GetSource(edge), node);
			}
		else
			{
			output->CreateEdge(edge, input->GetTarget(edge), node);
			}

		output->GetEdgeData()->InsertTuple(
			output->GetEdgeIndex(edge),
			input->GetEdgeData()->GetTuple(
				input->GetEdgeIndex(edge)));
		}

	delete [] tuple;
	pqueue->Delete();
	edgeCost->Delete();
	nodeCost->Delete();
	route->Delete();

  vtkDebugMacro(<<"EdgeSpanTree done.");
}


void vtkEdgeSpanTree::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkSource::PrintSelf(os, indent);
}
