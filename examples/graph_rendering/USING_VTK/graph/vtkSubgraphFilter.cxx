/*
 *	File: vtkSubgraphFilter.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkSubgraphFilter.h"
#include "vtkObjectFactory.h"
#include "vtkBitArray.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkSubgraphFilter);
vtkCxxRevisionMacro(vtkSubgraphFilter, "1.0");

vtkSubgraphFilter::vtkSubgraphFilter()
{
	this->KeepRemainder = 0;

	this->SelectorName = NULL;

	this->vtkSource::SetNthOutput(0,vtkGraph::New());
	this->vtkSource::SetNthOutput(1,vtkGraph::New());
	this->Outputs[0]->Delete();
	this->Outputs[1]->Delete();
}

vtkSubgraphFilter::~vtkSubgraphFilter()
{
	if (this->SelectorName)
		{
		delete [] this->SelectorName;
		}
}

vtkGraph *vtkSubgraphFilter::GetRemainder()
{
  if (this->NumberOfOutputs < 2)
    {
    return NULL;
    }
  
  return (vtkGraph *)(this->Outputs[1]);
}

void vtkSubgraphFilter::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkGraph *pass = this->GetOutput();
  vtkGraph *fail = this->GetRemainder();
	
	vtkIdIterator *nodes, *edges;
	vtkIdType node, edge, src, dst;

	vtkIdType nrBends, *bends;
	
	vtkBitArray *selector;
	vtkIdType nrNodes, nrEdges, nrPasses;
	float prop;

  vtkDebugMacro(<<"SubgraphFilter executing." );
	
	// Now create a model object to access the fields

	if (!this->SelectorName)
		{
		vtkErrorMacro(<<"No selector name.");
		return;
		}

	selector = vtkBitArray::SafeDownCast(
		input->GetNodeData()->GetArray(this->SelectorName));
	if (!selector)
		{
		vtkErrorMacro(<<"Selector not found in input node data.");
		return;
		}

	nrNodes = input->GetNumberOfNodes();
	nrEdges = input->GetNumberOfEdges();
	if (nrNodes == 0)
		{
		vtkWarningMacro(<<"No nodes in graph.");
		return;
		}

	// Copy strings, edge and node data structure
	pass->SetStrings(input->GetStrings());
	pass->SetNodeDataStringArrays(input->GetNodeDataStringArrays());
	pass->SetEdgeDataStringArrays(input->GetEdgeDataStringArrays());
	pass->GetNodeData()->CopyStructure(input->GetNodeData());
	pass->GetEdgeData()->CopyStructure(input->GetEdgeData());

	if (this->KeepRemainder)
		{
		fail->SetStrings(input->GetStrings());
		fail->SetNodeDataStringArrays(input->GetNodeDataStringArrays());
		fail->SetEdgeDataStringArrays(input->GetEdgeDataStringArrays());
		fail->GetNodeData()->CopyStructure(input->GetNodeData());
		fail->GetEdgeData()->CopyStructure(input->GetEdgeData());
		}
		
	// Count nr of passes.  Although this means traversing
	// the bit array, doing this will allow pre-allocation
	// of node and edge storage.
	nrPasses = 0;
	for (vtkIdType i = 0; i < nrNodes; i++)
		{
		if (selector->GetValue(i))
			{
			nrPasses++;
			}
		}

	// pre-allocate nr of nodes, and estimate nr of edges in 
	// output graphs.
	prop = ((float)nrPasses)/nrNodes;
	pass->SetInitialNodeSize(nrPasses);
	pass->SetInitialEdgeSize((vtkIdType)(prop*nrEdges));

	if (this->KeepRemainder)
		{
		fail->SetInitialNodeSize(nrNodes-nrPasses);
		fail->SetInitialEdgeSize(
			(vtkIdType)(((float)(nrNodes-nrPasses))*((float)(nrEdges/nrNodes))));
		}
	
	// Traverse nodes, copying into the "pass" graph if they are
	// selected, and into the "fail" graph if not and the KeepRemainder
	// selector is set.
	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		if (selector->GetValue(input->GetNodeIndex(node)))
			{
			pass->CreateNode(node);
			pass->SetEdgeArraySize(node, input->GetDegree(node));
			pass->GetNodeData()->InsertTuple(
				pass->GetNodeIndex(node),
				input->GetNodeData()->GetTuple(input->GetNodeIndex(node)));
			if (input->GetNodeIndex(node) < input->GetLayout()->GetNumberOfPoints())
				{
				pass->SetPosition(node, input->GetPosition(node));
				}
			}
		else
			{
			if (this->KeepRemainder)
				{
				fail->CreateNode(node);
				fail->SetEdgeArraySize(node, input->GetDegree(node));
				fail->GetNodeData()->InsertTuple(
					fail->GetNodeIndex(node),
					input->GetNodeData()->GetTuple(input->GetNodeIndex(node)));
				if (input->GetNodeIndex(node) < input->GetLayout()->GetNumberOfPoints())
					{
					fail->SetPosition(node, input->GetPosition(node));
					}
				}
			}
		}
	nodes->Delete();
	
	if (input->GetEdgeData()->GetNumberOfComponents() > 0)
		{
		pass->GetEdgeData()->CopyStructure(input->GetEdgeData());
		if (this->KeepRemainder)
			{
			fail->GetEdgeData()->CopyStructure(input->GetEdgeData());
			}
		}

	// Traverse edges.  Copy an edge into the "pass" graph if both
	// its endpoints are in that graph.  If the KeepRemainder selector
	// is set and both endpoints have not passed, copy the edge 
	// into the "fail" graph, creating the src or dst node as
	// appropriate.
	edges = input->GetEdges();
	while (edges->HasNext())
		{
		edge = edges->GetNext();
		src = input->GetSource(edge);
		dst = input->GetTarget(edge);
		if (pass->HasNode(src) && pass->HasNode(dst))
			{
			pass->CreateEdge(edge, src, dst);
			nrBends = input->GetNumberOfBends(edge);
			bends = input->GetBendArray(edge);
			for (int b = 0; b < nrBends; b++)
				{
				pass->InsertBend(edge, b, bends[b]);
				}
			pass->GetEdgeData()->InsertTuple(
				pass->GetEdgeIndex(edge),
				input->GetEdgeData()->GetTuple(input->GetEdgeIndex(edge)));
			}
		else
			{
			if (this->KeepRemainder)
				{
				if (!fail->HasNode(src))
					{
					fail->CreateNode(src);
				  fail->SetEdgeArraySize(src, input->GetDegree(src));
					fail->GetNodeData()->InsertTuple(
						fail->GetNodeIndex(src),
						input->GetNodeData()->GetTuple(input->GetNodeIndex(src)));
					if (input->GetNodeIndex(src) < input->GetLayout()->GetNumberOfPoints())
						{
						fail->SetPosition(src, input->GetPosition(src));
						}
					}
				if (!fail->HasNode(dst))
					{
					fail->CreateNode(dst);
					fail->SetEdgeArraySize(dst, input->GetDegree(dst));
					fail->GetNodeData()->InsertTuple(
						fail->GetNodeIndex(dst),
						input->GetNodeData()->GetTuple(input->GetNodeIndex(dst)));
					if (input->GetNodeIndex(dst) < input->GetLayout()->GetNumberOfPoints())
						{
						fail->SetPosition(dst, input->GetPosition(dst));
						}
					}
				fail->CreateEdge(edge, src, dst);
				nrBends = input->GetNumberOfBends(edge);
				bends = input->GetBendArray(edge);
				for (int b = 0; b < nrBends; b++)
					{
					fail->InsertBend(edge, b, bends[b]);
					}
				fail->GetEdgeData()->InsertTuple(
					fail->GetEdgeIndex(edge),
					input->GetEdgeData()->GetTuple(input->GetEdgeIndex(edge)));
				}
			}
		}
	edges->Delete();
	
	// Compact output graphs.
	pass->SetBends(input->GetBends());
	pass->Squeeze();
	if (this->KeepRemainder)
		{
		fail->SetBends(input->GetBends());
		fail->Squeeze();
		}

  vtkDebugMacro(<<"SubgraphFilter done.");
}

void vtkSubgraphFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkGraphToGraphFilter::PrintSelf(os, indent);
	os << indent << "Selector: ";
	os << (this->SelectorName ? this->SelectorName : "(null)") << "\n";
	if (this->KeepRemainder)
		{
		os << indent << "Retaining remainder of graph.\n";
		}
	else
		{
		os << indent << "Discarding remainder of graph.\n";
		}
}

