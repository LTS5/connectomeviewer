/*
 *	File: vtkGraph.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 *
 *	Change log:
 *	2003/12/18:	Updated position setting and getting to use double
 *              in line with move from float to double within VTK.
 *	2003/11/10:	Major additions to support bend points in edges.
 *	2003/10/30:	Included work-around from John Shalf for Mac OSX.
 */
 
#include "vtkGraph.h"
#include "vtkObjectFactory.h"
#include "vtkSystemIncludes.h"
#include "vtkIdArrayIterator.h"
#include "vtkIdListIterator.h"
#include "vtkChildIterator.h"
#include "vtkParentIterator.h"

vtkStandardNewMacro(vtkGraph);
vtkCxxRevisionMacro(vtkGraph, "1.10");

//  Needed to compile using gcc3 on OSX until new stdc++.dylib
#ifdef __APPLE_CC__
extern "C"
{
  void oft_initGraphs()
  {
  extern void _ZNSt8ios_base4InitC4Ev();
  _ZNSt8ios_base4InitC4Ev();
  }
}
#endif

//-------------------------------------------------------------------------

vtkGraph::vtkGraph()
{
	this->Nodes = vtkIndexedSet<_vtkNode_s>::New();
	this->Edges = vtkIndexedSet<_vtkEdge_s>::New();
	this->PrintLevel = 0;
	this->Layout = vtkPoints::New();
	this->NodeData = vtkFieldData::New();
	this->EdgeData = vtkFieldData::New();
	this->Bends = NULL;
	this->Strings = NULL;
	this->NodeDataStringArrays = vtkDataArraySelection::New();
	this->EdgeDataStringArrays = vtkDataArraySelection::New();
}

vtkGraph::~vtkGraph()
{	
	this->Nodes->UnRegister(this);
	this->Edges->UnRegister(this);
	this->Layout->UnRegister(this);
	this->NodeData->UnRegister(this);
	this->EdgeData->UnRegister(this);
	if (this->Bends)
		{
		this->Bends->UnRegister(this);
		}
	if (this->Strings)
		{
		this->Strings->UnRegister(this);
		}
	this->NodeDataStringArrays->UnRegister(this);
	this->EdgeDataStringArrays->UnRegister(this);
}

void vtkGraph::PrintSelf(ostream& os, vtkIndent indent)
{
	vtkIndent nextIndent = indent.GetNextIndent();
	_vtkNode_s *node;
	_vtkEdge_s *edge;
	double position[3];

	os << indent << "vtkGraph:\n";
	os << indent << "nodes: " << this->GetNumberOfNodes() << "\n";
	os << indent << "edges: " << this->GetNumberOfEdges() << "\n";

	if (this->PrintLevel > 0)
		{
		os << "NODES\n";
		for (vtkIdType i = 0; i < this->GetNumberOfNodes(); i++)
			{
			os << indent << i << "\t[" << this->GetNodeId(i) << "]";
			if (this->PrintLevel > 1)
				{
				if (i < this->Layout->GetNumberOfPoints())
					{
					this->Layout->GetPoint(i, position);
					os << "\t(" << position[0] 
							<< ", " << position[1] 
							<< ", " << position[2] << ")";
					}
				}
			node = this->Nodes->Array + i;
			if (node->numEdgesIn > 0)
				{
				os << " in:";
				for (vtkIdType j = 0; j < node->numEdgesIn; j++)
					{
					os << " " << node->edges[j];
					}
				}
			if (node->numEdges > node->numEdgesIn)
				{
				os << " out:";
				for (vtkIdType j = node->numEdgesIn; j < node->numEdges; j++)
					{
					os << " " << node->edges[j];
					}
				}
			os << "\n";
			}
		os << "EDGES\n";
		for (vtkIdType e = 0; e < this->GetNumberOfEdges(); e++)
			{
			os << indent << e << "\t[" << this->GetEdgeId(e) << "]";
			os << "\t";
			edge = this->Edges->Array + e;
			os << edge->source << "->" << edge->target;
			if (edge->pathLength > 0)
				{
				for (int b = 0; b < edge->pathLength; b++)
					{
					os << " " << edge->path[b];
					}
				}
			os << "\n";
			}
		}
}

void vtkGraph::Initialize()
{
	if (this->Nodes)
		{
		this->Nodes->UnRegister(this);
		this->Nodes = vtkIndexedSet<_vtkNode_s>::New();
		}
	if (this->Edges)
		{
		this->Edges->UnRegister(this);
		this->Edges = vtkIndexedSet<_vtkEdge_s>::New();
		}
	if (this->Layout)
		{
		this->Layout->UnRegister(this);
		this->Layout = vtkPoints::New();
		}
	if (this->NodeData)
		{
		this->NodeData->UnRegister(this);
		this->NodeData = vtkFieldData::New();
		}
	if (this->EdgeData)
		{
		this->EdgeData->UnRegister(this);
		this->EdgeData = vtkFieldData::New();
		}
	if (this->Bends)
		{
		this->Bends->UnRegister(this);
		this->Bends = NULL;
		}
	if (this->NodeDataStringArrays)
		{
		this->NodeDataStringArrays->UnRegister(this);
		this->NodeDataStringArrays = vtkDataArraySelection::New();
		}
	if (this->EdgeDataStringArrays)
		{
		this->EdgeDataStringArrays->UnRegister(this);
		this->EdgeDataStringArrays = vtkDataArraySelection::New();
		}
	if (this->Strings)
		{
		this->Strings->UnRegister(this);
		this->Strings = NULL;
		}
}

void vtkGraph::DeepCopy(vtkDataObject *src)
{
	vtkGraph *graph = vtkGraph::SafeDownCast(src);

	if (graph) 
		{
		// Assume that all internal objects are allocated.
		// This is guaranteed by constructor; any changes
		// should respect this assumption.
		this->Nodes->DeepCopy(graph->Nodes);
		this->Edges->DeepCopy(graph->Edges);
		this->Layout->DeepCopy(graph->Layout);
		this->NodeData->DeepCopy(graph->NodeData);
		this->EdgeData->DeepCopy(graph->EdgeData);
		if (graph->Bends)
			{
			if (!this->Bends)
				{
				this->Bends = vtkPoints::New();
				}
			this->Bends->DeepCopy(graph->Bends);
			}
		else
			{
			if (this->Bends)
				{
				this->Bends->UnRegister(this);
				this->Bends = NULL;
				}
			}
		if (this->Strings)
			{
			this->Strings->UnRegister(this);
			}
		if (graph->Strings)
			{
			this->Strings = vtkStringTable::New();
			this->Strings->DeepCopy(graph->Strings);
			}
		else
			{
			this->Strings = NULL;
			}
		this->NodeDataStringArrays->RemoveAllArrays();
		this->NodeDataStringArrays->CopySelections(graph->NodeDataStringArrays);
		this->EdgeDataStringArrays->RemoveAllArrays();
		this->EdgeDataStringArrays->CopySelections(graph->EdgeDataStringArrays);
		}
	this->vtkDataObject::DeepCopy(src);
}

void vtkGraph::ShallowCopy(vtkDataObject *src)
{
	vtkGraph *graph = vtkGraph::SafeDownCast(src);

	if (graph) 
		{
		// Assume that all internal objects are allocated.
		// This is guaranteed by constructor; any changes
		// should respect this assumption.
		this->Nodes->UnRegister(this);
		this->Edges->UnRegister(this);
		this->Layout->UnRegister(this);

		this->Nodes = graph->Nodes;
		this->Edges = graph->Edges;
		this->Layout = graph->Layout;

		this->Nodes->Register(this);
		this->Edges->Register(this);
		this->Layout->Register(this);

		this->NodeData->ShallowCopy(graph->NodeData);
		this->EdgeData->ShallowCopy(graph->EdgeData);

		if (this->Bends)	
			{
			this->Bends->UnRegister(this);
			}
		this->Bends = graph->Bends;
		if (this->Bends)	
			{
			this->Bends->Register(this);
			}
		if (this->Strings)
			{
			this->Strings->UnRegister(this);
			}
		this->Strings = graph->Strings;
		if (this->Strings)
			{
			this->Strings->Register(this);
			}
		if (this->NodeDataStringArrays)
			{
			this->NodeDataStringArrays->UnRegister(this);
			}
		if (this->EdgeDataStringArrays)
			{
			this->EdgeDataStringArrays->UnRegister(this);
			}
		this->NodeDataStringArrays = graph->NodeDataStringArrays;
		this->EdgeDataStringArrays = graph->EdgeDataStringArrays;
		this->NodeDataStringArrays->Register(this);
		this->EdgeDataStringArrays->Register(this);
		}
	this->vtkDataObject::ShallowCopy(this);
}

void vtkGraph::SetUpdateExtent(int piece, int numPieces, int ghostLevel)
{
	this->UpdatePiece = piece;
	this->UpdateNumberOfPieces = numPieces;
	this->UpdateGhostLevel = ghostLevel;
	this->UpdateExtentInitialized = 1;
}

void vtkGraph::SetInitialNodeSize(vtkIdType nrNodes)
{
	this->Nodes->SetInitialSize(nrNodes);
	// Must resize arrays individually, as there is no resize operation
	// in the field data interface.
	for (int i = 0; i < this->NodeData->GetNumberOfArrays(); i++)
		{
		this->NodeData->GetArray(i)->Resize(nrNodes);
		}
	// We do not pre-allocate point space, as fresh points will
	// often be created and added to the graph, e.g. by a 
	// layout routine.
}

void vtkGraph::SetInitialEdgeSize(vtkIdType nrEdges)
{
	this->Edges->SetInitialSize(nrEdges);
	for (int i = 0; i < this->EdgeData->GetNumberOfArrays(); i++)
		{
		this->EdgeData->GetArray(i)->Resize(nrEdges);
		}
}

void vtkGraph::SetInternalIndexSizes(vtkIdType nodeXS, vtkIdType edgeXS)
{
	this->Nodes->SetIndexSize(nodeXS);
	this->Edges->SetIndexSize(edgeXS);
}

void vtkGraph::Squeeze()
{
	_vtkNode_s *node;
	vtkIdType *array;

	this->Nodes->Squeeze();
	this->Edges->Squeeze();
	
	// re-allocate edge storage to fit exact number of
	// edges per node.  This duplicates functionality from 
	// SetEdgeArraySize, but saves multiple calls to extract
	// node ids, and deals with the case where a node has
	// no edges but has space allocated for it.
	for (vtkIdType i = 0; i < this->GetNumberOfNodes(); i++)
		{
		array = NULL;
		node = this->Nodes->GetElementAtIndex(i);
		if (node->arraySize > node->numEdges)
			{
			if (node->numEdges > 0)
				{
				array = new vtkIdType[node->numEdges];
				for (vtkIdType j = 0; j < node->numEdges; j++)
					{
					array[j] = node->edges[j];
					}
				}
			if (node->edges)
				{
				delete [] node->edges;
				}
			node->edges = array;
			node->arraySize = node->numEdges;
			}
		}
	this->NodeData->Squeeze();
	this->EdgeData->Squeeze();
	this->Layout->Squeeze();
	if (this->Bends)
		{
		this->Bends->Squeeze();
		}
}

// Node/edge creation ----------------------------------------

vtkIdType vtkGraph::CreateNode()
{
	vtkIdType nodeId = this->Nodes->Create();
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	
	node->numEdges = 0;
	node->numEdgesIn = 0;
	node->edges = NULL;
	node->arraySize = 0;

	return nodeId;
}

vtkIdType vtkGraph::CreateNode(vtkIdType nodeId)
{
	this->Nodes->Create(nodeId);
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	
	node->numEdges = 0;
	node->numEdgesIn = 0;
	node->edges = NULL;
	node->arraySize = 0;

	return nodeId;
}
	
vtkIdType vtkGraph::CreateEdge(vtkIdType srcId, vtkIdType dstId)
{
	vtkIdType edgeId = this->Edges->Create();
	_vtkEdge_s *edge = this->Edges->GetElementWithId(edgeId);

	if (!this->Nodes->Contains(srcId))
		{
		this->CreateNode(srcId);
		}
	if (!this->Nodes->Contains(dstId))
		{
		this->CreateNode(dstId);
		}
	edge->source = srcId;
	edge->target = dstId;
	edge->arraySize = 0;
	edge->pathLength = 0;
	edge->path = NULL;

	this->AddEdge(srcId, edgeId, 0);
	this->AddEdge(dstId, edgeId, 1);

	return edgeId;
}
 
vtkIdType vtkGraph::CreateEdge(
	vtkIdType edgeId, 
	vtkIdType srcId, 
	vtkIdType dstId)
{
	_vtkEdge_s *edge;

	if (this->Edges->Contains(edgeId))
		{
		vtkWarningMacro(<<"Overriding edge "<<edgeId);
		this->DeleteEdge(edgeId);
		}
	if (!this->Nodes->Contains(srcId))
		{
		this->CreateNode(srcId);
		}
	if (!this->Nodes->Contains(dstId))
		{
		this->CreateNode(dstId);
		}
		
	this->Edges->Create(edgeId);

	edge = this->Edges->GetElementWithId(edgeId);
	edge->source = srcId;
	edge->target = dstId;
	edge->arraySize = 0;
	edge->pathLength = 0;
	edge->path = NULL;

	this->AddEdge(srcId, edgeId, 0);
	this->AddEdge(dstId, edgeId, 1);

	return edgeId;
}

// Internal function.  Add an edge to the given node.
// The edgeId is recorded in the "edges" array of the
// node struct, in a position that depends on whether the
// edge is incoming or outgoing relative to the node.
// NOTE: the edge array size is incremented by a small 
// value -- large-scale changes to the edge array should
// be managed directly via SetEdgeArraySize

void vtkGraph::AddEdge(vtkIdType nodeId, vtkIdType edgeId, int isTarget)
{
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);

	// Resize the edge array, if necessary.
	if (node->numEdges == node->arraySize)
		{
		this->SetEdgeArraySize(nodeId, node->arraySize+2);
		}
	if (isTarget)
		{
		// Add an incoming edge, repositioning an outgoing edge to
		// the end of the array to make room.
		node->edges[node->numEdges] = node->edges[node->numEdgesIn];
		node->edges[node->numEdgesIn] = edgeId;
		node->numEdgesIn++;
		}
	else
		{
		// Add an outgoing edge.
		node->edges[node->numEdges] = edgeId;
		}
	node->numEdges++;
} 

void vtkGraph::SetEdgeArraySize(vtkIdType nodeId, vtkIdType size)
{
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	vtkIdType *array;

	// Resize the edge array, if necessary.
	if (size > node->numEdges)
		{
		array = new vtkIdType[size];
		for (vtkIdType i = 0; i < node->numEdges; i++)
			{
			array[i] = node->edges[i];
			}
		if (node->edges)
			{
			delete [] node->edges;
			}
		node->edges = array;
		node->arraySize = size;
		}
}

// Node/edge deletion ---------------------------------------------

void vtkGraph::DeleteNode(vtkIdType nodeId)
{
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	vtkIdType *array;
	vtkIdType movedIx, nodeIx; 
	vtkIdType numToRemove;

	if (node->numEdges > 0)
		{
		// Copy edge array before deleting edges, to guard against
		// side effects when the edge is removed from the source and 
		// target nodes.
		array = new vtkIdType[node->numEdges];
		for (vtkIdType i1 = 0; i1 < node->numEdges; i1++)
			{
			array[i1] = node->edges[i1];
			}
		numToRemove = node->numEdges;

		// Now remove the edges ...		
		for (vtkIdType i2 = 0; i2 < numToRemove; i2++)
			{
			this->DeleteEdge(array[i2]);
			}
		delete [] array;
		
		// At this point each individual edge should be gone, but
		// the storage has not been reclaimed.
		delete [] node->edges;
		}
	
	// Delete the node from the indexed set.  The index used by 
	// the node may then be reused to maintain compaction of the
	// node data and point structures; in this case, the node
	// and point information for the node moved into the vacant
	// index position must also be relocated.
	nodeIx = this->Nodes->IndexOfId(nodeId);
	if (this->Nodes->Delete(nodeId, movedIx))
		{
		if (movedIx < this->Layout->GetNumberOfPoints())
			{
			this->Layout->SetPoint(nodeIx, this->Layout->GetPoint(movedIx));
			}
		if (movedIx < this->NodeData->GetNumberOfTuples())
			{
			this->NodeData->InsertTuple(
				nodeIx,
				this->NodeData->GetTuple(movedIx));
			}
		}
}
 
void vtkGraph::DeleteEdge(vtkIdType edgeId)
{
	_vtkEdge_s *edge = this->Edges->GetElementWithId(edgeId);
	vtkIdType movedIx, edgeIx; 

	this->RemoveEdgeFromNode(edgeId, edge->source);
	this->RemoveEdgeFromNode(edgeId, edge->target);
	if (edge->path)
		{
		delete [] edge->path;
		}

	// Delete the edge from the indexed set.  The index used by 
	// the edge may then be reused to maintain compaction of the
	// edge data; in this case, the information for the edge 
	// moved into the vacant index position must also be relocated.
	edgeIx = this->Edges->IndexOfId(edgeId);
	if(this->Edges->Delete(edgeId, movedIx))
		{
		if (movedIx < this->EdgeData->GetNumberOfTuples())
			{
			this->EdgeData->InsertTuple(
				edgeIx,
				this->EdgeData->GetTuple(movedIx));
			}
		}
}

// Alternative interface for edge deletion: first find
// the edgeId, then remove the edge.
void vtkGraph::DeleteEdge(vtkIdType srcId, vtkIdType dstId)
{
	this->DeleteEdge(this->GetEdgeId(srcId, dstId));
}

// Remove edge from node.  Function maintains compact edge
// array by moving an edge down to fill the hole made by 
// the removed edge.  Care is needed in case the removed
// edge is an in-edge, the hole must first be filled by the
// last in-edge, and then this hole is filled by the last
// edge.

void vtkGraph::RemoveEdgeFromNode(vtkIdType edgeId, vtkIdType nodeId)
{
  _vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	vtkIdType i = 0;	// position of "hole"
	
	while (i < node->numEdges && node->edges[i] != edgeId)
		{
		i++;
		}
	if (i == node->numEdges)
		{
		vtkErrorMacro(<<"RemoveEdgeFromNode: graph structure corrupted.");
		return;
		}
	// if the edge is an in-edge, move the last in-edge in
	// to fill the hole; the moved edge position then becomes
	// the new hole.
	if (i < node->numEdgesIn)
		{
		node->edges[i] = node->edges[node->numEdgesIn-1];
		i = --node->numEdgesIn;
		}
	// move the last edge into the hole, and decrement the total
	// number of edges.
	node->edges[i] = node->edges[node->numEdges-1];
	--node->numEdges;
}


// ---------------------------------------------------------
// Property queries.  These are delegated to the appropriate
// instance of the indexed sets carrying the graph structure.

int vtkGraph::HasNode(vtkIdType nodeId)
{	
	return this->Nodes->Contains(nodeId);
}

int vtkGraph::HasEdge(vtkIdType edgeId)
{
	return this->Edges->Contains(edgeId); 
}
 
int vtkGraph::HasEdge(vtkIdType srcId, vtkIdType dstId)
{
  _vtkNode_s *node;
	vtkIdType i;	
	
	if (!this->HasNode(srcId))
		{
		return 0;
		}
	node = this->Nodes->GetElementWithId(srcId);
	i = node->numEdgesIn; // first out-going edge
	while (i < node->numEdges)
		{
		if (this->GetTarget(node->edges[i]) == dstId)
			{
			return 1;
			}
		else
			{
			i++;
			}
		}
	return 0;
}
 
vtkIdType vtkGraph::GetNodeIndex(vtkIdType nodeId)
{
	return this->Nodes->IdToIndex->Get(nodeId);
}
 
vtkIdType vtkGraph::GetEdgeIndex(vtkIdType nodeId)
{
	return this->Edges->IdToIndex->Get(nodeId);
}

vtkIdType vtkGraph::GetNodeId(vtkIdType nodeIx)
{
	return this->Nodes->IndexToId->GetValue(nodeIx);
}

vtkIdType vtkGraph::GetEdgeId(vtkIdType edgeIx)
{
	return this->Edges->IndexToId->GetValue(edgeIx);
}

// Find the id of an edge from src to dst.  To do this, we
// run through the outgoing edges of src looking for the
// first edge with dst as its target.
vtkIdType vtkGraph::GetEdgeId(vtkIdType srcId, vtkIdType dstId)
{
  _vtkNode_s *node = this->Nodes->GetElementWithId(srcId);
	vtkIdType i = node->numEdgesIn;	// first out-going edge
	
	while (i < node->numEdges)
		{
		if (this->GetTarget(node->edges[i]) == dstId)
			{
			break;
			}
		else
			{
			i++;
			}
		}
	return (i < node->numEdges) ? node->edges[i] : -1;
}

vtkIdType vtkGraph::GetMaxNodeId()
{
	return this->Nodes->IdToIndex->GetMaxId();
}

vtkIdType vtkGraph::GetMaxEdgeId()
{
	return this->Edges->IdToIndex->GetMaxId();
}

vtkIdType vtkGraph::GetDegree(vtkIdType nodeId)
{
	return this->Nodes->GetElementWithId(nodeId)->numEdges;
}

vtkIdType vtkGraph::GetNumberOfParents(vtkIdType nodeId)
{
	return this->Nodes->GetElementWithId(nodeId)->numEdgesIn;
}

vtkIdType vtkGraph::GetNumberOfChildren(vtkIdType nodeId)
{
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);

	return node->numEdges - node->numEdgesIn;
}
  
void vtkGraph::GetEdge(
	vtkIdType edgeId, 
	vtkIdType &srcId, 
	vtkIdType &dstId)
{
	_vtkEdge_s *edge = this->Edges->GetElementWithId(edgeId);
	srcId = edge->source;
	dstId = edge->target;
}
  
vtkIdType vtkGraph::GetSource(vtkIdType edgeId)
{
	return this->Edges->GetElementWithId(edgeId)->source;
}

vtkIdType vtkGraph::GetTarget(vtkIdType edgeId)
{
	return this->Edges->GetElementWithId(edgeId)->target;
}
  

// -------------------------------------------------------
// String array maintainance

vtkFieldData *vtkGraph::GetAttributeData(int location)
{
	if (location == NODE_DATA)
		{
		return this->NodeData;
		}
	if (location == EDGE_DATA)
		{
		return this->EdgeData;
		}
	vtkErrorMacro(<<"Bad data location.");
	return 0;
}

void vtkGraph::MarkAsNodeDataStringArray(const char *name)
{
	this->NodeDataStringArrays->EnableArray(name);
}

void vtkGraph::MarkAsEdgeDataStringArray(const char *name)
{
	this->EdgeDataStringArrays->EnableArray(name);
}

void vtkGraph::MarkAsDataStringArray(int location, const char *name)
{
	if (location == NODE_DATA)
		{
		this->NodeDataStringArrays->EnableArray(name);
		return;
		}
	if (location == EDGE_DATA)
		{
		this->EdgeDataStringArrays->EnableArray(name);
		return;
		}
	vtkErrorMacro(<<"Bad data location.");
}

void vtkGraph::DisableNodeDataStringArray(const char *name)
{
	if (this->NodeDataStringArrays->ArrayExists(name))
		{
		this->NodeDataStringArrays->DisableArray(name);
		}
}

void vtkGraph::DisableEdgeDataStringArray(const char *name)
{
	if (this->EdgeDataStringArrays->ArrayExists(name))
		{
		this->EdgeDataStringArrays->DisableArray(name);
		}
}

void vtkGraph::DisableDataStringArray(int location, const char *name)
{
	if (location == NODE_DATA)
		{
		this->NodeDataStringArrays->DisableArray(name);
		return;
		}
	if (location == EDGE_DATA)
		{
		this->EdgeDataStringArrays->DisableArray(name);
		return;
		}
	vtkErrorMacro(<<"Bad data location.");
}

int vtkGraph::IsNodeDataStringArray(const char *name)
{
	return this->NodeDataStringArrays->ArrayIsEnabled(name);
}

int vtkGraph::IsEdgeDataStringArray(const char *name)
{
	return this->EdgeDataStringArrays->ArrayIsEnabled(name);
}

int vtkGraph::IsDataStringArray(int location, const char *name)
{
	if (location == NODE_DATA)
		{
		return this->NodeDataStringArrays->ArrayIsEnabled(name);
		}
	if (location == EDGE_DATA)
		{
		return this->EdgeDataStringArrays->ArrayIsEnabled(name);
		}
	vtkErrorMacro(<<"Bad data location.");
	return 0;
}


// -------------------------------------------------------
// Functions involving node positions.

void vtkGraph::SetPosition(vtkIdType nodeId, double x, double y, double z)
{
	this->Layout->InsertPoint(
		this->Nodes->IndexOfId(nodeId), 
		x, 
		y, 
		z);
}

void vtkGraph::SetPosition(vtkIdType nodeId, double *position)
{
	this->Layout->InsertPoint(
		this->Nodes->IndexOfId(nodeId),
		position);
}

void vtkGraph::GetPosition(vtkIdType nodeId, double& x, double& y, double& z)
{
	double pos[3];

	this->Layout->GetPoint(this->Nodes->IndexOfId(nodeId), pos);
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

double *vtkGraph::GetPosition(vtkIdType nodeId)
{
	return this->Layout->GetPoint(this->Nodes->IndexOfId(nodeId));
}

void vtkGraph::GetPosition(vtkIdType nodeId, double position[3])
{
	this->Layout->GetPoint(
		this->Nodes->IndexOfId(nodeId), position);
}

// SetX/Y/Z functions take into account the possibility that
// the node is already associated with a point.  If the node
// does not have an associated point, the other components
// of the coordinate are set to 0.0.
void vtkGraph::SetX(vtkIdType nodeId, double x)
{ 
	double coord[3] = {0.0, 0.0, 0.0};
	vtkIdType index = this->Nodes->IndexOfId(nodeId);

	if (index < this->Layout->GetNumberOfPoints())
		{
		this->Layout->GetPoint(index, coord);
		}
	coord[0] = x;
	this->Layout->InsertPoint(index, coord);
}

void vtkGraph::SetY(vtkIdType nodeId, double y)
{
	double coord[3] = {0.0, 0.0, 0.0};
	vtkIdType index = this->Nodes->IndexOfId(nodeId);

	if (index < this->Layout->GetNumberOfPoints())
		{
		this->Layout->GetPoint(index, coord);
		}
	coord[1] = y;
	this->Layout->InsertPoint(index, coord);
}

void vtkGraph::SetZ(vtkIdType nodeId, double z)
{
	double coord[3] = {0.0, 0.0, 0.0};
	vtkIdType index = this->Nodes->IndexOfId(nodeId);

	if (index < this->Layout->GetNumberOfPoints())
		{
		this->Layout->GetPoint(index, coord);
		}
	coord[2] = z;
	this->Layout->InsertPoint(index, coord);
}

double vtkGraph::GetX(vtkIdType nodeId)
{
	double coord[3];
	vtkIdType index = this->Nodes->IndexOfId(nodeId);

	this->Layout->GetPoint(index, coord);
	return coord[0];
}

double vtkGraph::GetY(vtkIdType nodeId)
{
	double coord[3];
	vtkIdType index = this->Nodes->IndexOfId(nodeId);

	this->Layout->GetPoint(index, coord);
	return coord[1];
}

double vtkGraph::GetZ(vtkIdType nodeId)
{
	double coord[3];
	vtkIdType index = this->Nodes->IndexOfId(nodeId);

	this->Layout->GetPoint(index, coord);
	return coord[2];
}


// Edge bend operations --------------------------------------

vtkIdType vtkGraph::CreateBend()
{
	if (!this->Bends)
		{
		this->Bends = vtkPoints::New();
		}
	return this->Bends->InsertNextPoint(0.0, 0.0, 0.0);
}

// 
void vtkGraph::InsertBend(vtkIdType edgeId, int posn, vtkIdType bend)
{
	_vtkEdge_s *edge = this->Edges->GetElementWithId(edgeId);
	vtkIdType *path;
	vtkIdType length;

	length = edge->pathLength+1;
  if (length < posn)
		{
		length = posn;
		}
	if (edge->arraySize < length)
		{
		path = new vtkIdType[length];
		if (edge->path)
			{
			for (int i = 0; i < edge->pathLength; i++)
				{
				path[i] = edge->path[i];
				}
			delete [] edge->path;
			}
		edge->path = path;
		edge->arraySize = length;
		}
	for (int j = length-1; j>posn; j--)
		{
		edge->path[j] = edge->path[j-1];
		}
	edge->path[posn] = bend;
	edge->pathLength = length;
}

vtkIdType *vtkGraph::GetBendArray(vtkIdType edgeId)
{
	_vtkEdge_s *edge = this->Edges->GetElementWithId(edgeId);
	return edge->path;
}

void vtkGraph::SetBendPosition(vtkIdType bend, double x, double y, double z)
{
	this->Bends->SetPoint(bend, x, y, z);
}

void vtkGraph::SetBendPosition(vtkIdType bend, double *position)
{
	this->Bends->SetPoint(bend, position);
}

void vtkGraph::GetBendPosition(vtkIdType bend, double &x, double &y, double &z)
{
	double pos[3];
	this->Bends->GetPoint(bend, pos);
	x = pos[0];
	y = pos[1];
	z = pos[2];
}

double *vtkGraph::GetBendPosition(vtkIdType bend)
{
	return this->Bends->GetPoint(bend);
}

void vtkGraph::GetBendPosition(vtkIdType bend, double position[3])
{
	this->Bends->GetPoint(bend, position);
}

vtkIdType vtkGraph::GetNumberOfBends()
{
	return this->Bends ? this->Bends->GetNumberOfPoints() : 0;
}

vtkIdType vtkGraph::GetNumberOfBends(vtkIdType edgeId)
{
	_vtkEdge_s *edge = this->Edges->GetElementWithId(edgeId);
	return edge->pathLength;
}

vtkIdIterator *vtkGraph::GetBends(vtkIdType edgeId)
{
	vtkIdArrayIterator *iter = vtkIdArrayIterator::New();
	_vtkEdge_s *edge = this->Edges->GetElementWithId(edgeId);
	
	iter->SetArray(edge->path);
	iter->SetStart(0);
	iter->SetEnd(edge->pathLength);
	iter->Initialize();
	
	return iter;
}

// SetX/Y/Z functions take into account the possibility that
// the node is already associated with a point.  If the node
// does not have an associated point, the other components
// of the coordinate are set to 0.0.
void vtkGraph::SetBendX(vtkIdType bend, double x)
{ 
	double coord[3] = {0.0, 0.0, 0.0};

	if (bend < this->Bends->GetNumberOfPoints())
		{
		this->Bends->GetPoint(bend, coord);
		}
	coord[0] = x;
	this->Bends->InsertPoint(bend, coord);
}

void vtkGraph::SetBendY(vtkIdType bend, double y)
{
	double coord[3] = {0.0, 0.0, 0.0};

	if (bend < this->Bends->GetNumberOfPoints())
		{
		this->Bends->GetPoint(bend, coord);
		}
	coord[1] = y;
	this->Bends->InsertPoint(bend, coord);
}

void vtkGraph::SetBendZ(vtkIdType bend, double z)
{
	double coord[3] = {0.0, 0.0, 0.0};

	if (bend < this->Bends->GetNumberOfPoints())
		{
		this->Bends->GetPoint(bend, coord);
		}
	coord[2] = z;
	this->Bends->InsertPoint(bend, coord);
}

double vtkGraph::GetBendX(vtkIdType bend)
{
	double coord[3];

	this->Bends->GetPoint(bend, coord);
	return coord[0];
}

double vtkGraph::GetBendY(vtkIdType bend)
{
	double coord[3];

	this->Bends->GetPoint(bend, coord);
	return coord[1];
}

double vtkGraph::GetBendZ(vtkIdType bend)
{
	double coord[3];

	this->Bends->GetPoint(bend, coord);
	return coord[2];
}



// -------------------------------------------------------
// Iterators.  Traversal is implemented by a family of
// iterator classes defined as subtypes of vtkIdIterator.
// vtkGraph is defined as a friend of these and sets their
// internal data according to the kind of traversal needed.

vtkIdIterator *vtkGraph::GetNodes()
{
	return this->Nodes->GetIds();
}

vtkIdIterator *vtkGraph::GetEdges()
{
	return this->Edges->GetIds();
}

// Traversal of neighboring nodes.  Only neighboring edges 
// are stored explicitly, so this function defines an 
// iterator that works on an explicitly generated list of
// neighboring node ids, constructed from the edge list.
vtkIdIterator *vtkGraph::GetNeighbors(vtkIdType nodeId)
{
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	vtkIdListIterator *iter = vtkIdListIterator::New();
	vtkIdList *list = vtkIdList::New();
	vtkIdType i;
	
	list->SetNumberOfIds(node->numEdges);
	// Edges from 0 to numEdgesIn have nodeId as target, so
	// request the source node to add to the list.
	for (i = 0; i < node->numEdgesIn; i++)
		{
		list->SetId(i, this->GetSource(node->edges[i]));
		}
	// Remaining edges have nodeId as source, so request
	// the target and add to the list.
	while (i < node->numEdges)
		{
		list->SetId(i, this->GetTarget(node->edges[i]));
		i++;
		}
	iter->SetList(list);
	iter->Initialize();
	list->UnRegister(this);
	
	return iter;
}

// Parent traversal is a frequent operation supported by an
// explicit subclass of vtkIdIterator that goes through the
// incoming edges of a node and returns the source for each
// edge.
vtkIdIterator *vtkGraph::GetParents(vtkIdType nodeId)
{
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	vtkParentIterator *iter = vtkParentIterator::New();
	
	iter->SetArray(node->edges);
	iter->SetStart(0);
	iter->SetEnd(node->numEdgesIn);
	iter->SetGraph(this);
	iter->Initialize();
	
	return iter;
}

// Child traversal is a frequent operation supported by an
// explicit subclass of vtkIdIterator that goes through the
// outgoing edges of a node and returns the target for each
// edge.
vtkIdIterator *vtkGraph::GetChildren(vtkIdType nodeId)
{
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	vtkChildIterator *iter = vtkChildIterator::New();

	iter->SetArray(node->edges);
	iter->SetStart(node->numEdgesIn);
	iter->SetEnd(node->numEdges);
	iter->SetGraph(this);
	iter->Initialize();
	
	return iter;
}

// Edge traversal is straightforward, as each node has the
// list of incoming/outgoing edges.
vtkIdIterator *vtkGraph::GetEdges(vtkIdType nodeId)
{
	vtkIdArrayIterator *iter = vtkIdArrayIterator::New();
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	
	iter->SetArray(node->edges);
	iter->SetStart(0);
	iter->SetEnd(node->numEdges);
	iter->Initialize();
	
	return iter;
}

vtkIdIterator *vtkGraph::GetIncomingEdges(vtkIdType nodeId)
{
	vtkIdArrayIterator *iter = vtkIdArrayIterator::New();
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	
	iter->SetArray(node->edges);
	iter->SetStart(0);
	iter->SetEnd(node->numEdgesIn);
	iter->Initialize();
	
	return iter;
}

vtkIdIterator *vtkGraph::GetOutgoingEdges(vtkIdType nodeId)
{
	vtkIdArrayIterator *iter = vtkIdArrayIterator::New();
	_vtkNode_s *node = this->Nodes->GetElementWithId(nodeId);
	
	iter->SetArray(node->edges);
	iter->SetStart(node->numEdgesIn);
	iter->SetEnd(node->numEdges);
	iter->Initialize();
	
	return iter;
}

// Source traversal: an iterator is returned that uses an
// explicitly constructed list of all node ids for nodes
// with in-degree 0.
vtkIdIterator *vtkGraph::GetSources()
{
	vtkIdListIterator *iter = vtkIdListIterator::New();
	vtkIdList *list = vtkIdList::New();
	vtkIdType nodeId;
	
	list->Initialize();
	for (vtkIdType i = 0; i < this->Nodes->GetNumberOfElements(); i++)
		{
		nodeId = this->GetNodeId(i);
		if (this->GetNumberOfParents(nodeId) == 0)
			{
			list->InsertNextId(nodeId);
			}
		}

	iter->SetList(list);
	iter->Initialize();
	list->UnRegister(this);
	
	return iter;
}

// Sink traversal: an iterator is returned that uses an
// explicitly constructed list of all node ids for nodes
// with out-degree 0.
vtkIdIterator *vtkGraph::GetSinks()
{
	vtkIdListIterator *iter = vtkIdListIterator::New();
	vtkIdList *list = vtkIdList::New();
	vtkIdType nodeId;
	
	list->Initialize();
	for (vtkIdType i = 0; i < this->Nodes->GetNumberOfElements(); i++)
		{
		nodeId = this->GetNodeId(i);
		if (this->GetNumberOfChildren(nodeId) == 0)
			{
			list->InsertNextId(nodeId);
			}
		}

	iter->SetList(list);
	iter->Initialize();
	list->UnRegister(this);
	
	return iter;
}

// ---------------------------------------------------------
// Graph properties.

// Test whether the graph is acyclic. 
int vtkGraph::IsAcyclic()
{
  vtkBitArray *seen;
  vtkBitArray *done;
  vtkIdType node;
  vtkIdType acyclic;
	vtkIdIterator *nodes;
	
  seen = vtkBitArray::New();
  done = vtkBitArray::New();
  seen->SetNumberOfValues(this->GetNumberOfNodes());
  done->SetNumberOfValues(this->GetNumberOfNodes());
  for (vtkIdType i = 0; i < this->GetNumberOfNodes(); i++)
    {
		seen->SetValue(i,0);
		done->SetValue(i,0);
		}
  acyclic = 1;
  
	nodes = this->GetNodes();
  while (acyclic && nodes->HasNext())
    {
		node = nodes->GetNext();
		if (!seen->GetValue(this->Nodes->IndexOfId(node)))
			{
			acyclic &= this->TestAcyclic(node,seen,done);
			}
		}
  seen->Delete();
  done->Delete();
  return acyclic;
}

int vtkGraph::TestAcyclic(
	vtkIdType node, 
	vtkBitArray *seen, 
	vtkBitArray *done)
{
  vtkIdType nodeIx = this->Nodes->IndexOfId(node);
  vtkIdType nbor, nborIx;
  int acyclic;
	vtkIdIterator *neighbors;

  seen->SetValue(nodeIx, 1);
  neighbors = this->GetChildren(node);
  acyclic = 1;
  while (acyclic && neighbors->HasNext())
		{
		nbor = neighbors->GetNext();
		nborIx = this->Nodes->IndexOfId(nbor);
		if (seen->GetValue(nborIx))
			{
			if (!done->GetValue(nborIx))
				{
				acyclic = 0;
				}
			}
		else
			{
			acyclic = this->TestAcyclic(nbor, seen, done);
			}
		}
	done->SetValue(nodeIx,1);
  neighbors->Delete();
  return acyclic;
}
  
