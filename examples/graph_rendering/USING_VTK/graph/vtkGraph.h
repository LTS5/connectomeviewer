/*
 *	File: vtkGraph.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.
 *
 *	Change Log:
 *	2003/12/18:	Modified interface for setting/getting positions
 *              to use double in place of float.
 *	2003/12/12:	String support enhanced by having graph carry
 *              a string table, and a record of which data
 *              arrays should be interpreted as string indices.
 *	2003/11/10:	Added functions to allow bends in edges.
 */
// .NAME vtkGraph - dataset representing a node-link graph.
// .SECTION Description
// vtkGraph supports the storage and manipulation of a graph,
// defined by: a set of nodes, a set of edges between nodes,
// an assignment of points to nodes, and data fields associated
// with both points and nodes.  Technically the graph is directed,
// in that each edge has a specific source and target node, but 
// as each node records both its incoming and outgoing edges it
// is straightforward to treat the graph as undirected.
//
// The class interface supports the construction and manipulation
// of a graph, ensuring the structure is consistent, and implements 
// a storage scheme for node/edge data that reuses existing VTK 
// classes in a way that reduces storage overheads.
// 
// Both nodes and edges are identified by a unique ID of type 
// vtkIdType.  The class is designed so that this ID is separate 
// from any notion of how the nodes and edges are stored; if 
// part of one graph is copied into another (i.e. a subgraph is
// extracted), the node in the subgraph can (and usually should)
// be given the same ID as the corresponding node in the original
// graph.
//
// Node and edge data are stored in instances of vtkFieldData. As
// the range of IDs assigned to nodes and edges is arbitrary, the
// vtkGraph class uses an internal mapping from node/edge ID to a
// node/edge INDEX, which is then used to access the data for the
// node or edge in the corresponding field data.  The class
// ensures that index values form a contiguous block of values
// from 0, thus keeping the storage of data in the field data 
// arrays compact.  To store or access a value or tuple, it is
// necessary to map an ID to an index, for example if "foo" is
// a data array for nodes in "graph", use:
//
//   foo->SetValue(graph->GetNodeIndex(node, val));
// 
// to set the foo-value of "node" to "val".  When nodes or edges
// are deleted, a vtkGraph object will reshuffle the mapping from
// IDs to INDEXes to retain compactness; for this reason, if a
// data array is created for a given graph, it is important to 
// ensure that it is added to the node/edge data of the graph 
// before carrying out operations that involve internal changes 
// to the mapping, currently these are the Delete operations.
//
// Internally, the graph uses hash tables (instances of vtkIdMap)
// to map from IDs to INDEXes.  The size of these tables can be
// changed by calling SetInternalTableSizes(nodeTS, edgeTS).
//
// Each node in the graph can be assigned a position via a 
// vtkPoints structure accessed via Set/GetLayout, and also
// via Set/GetPosition functions in the interface.  If the 
// vtkPoints object is accessed directly, node IDs must be mapped
// manually to indexes.  This mapping is performed automatically
// by the Set/GetPosition functions.
//
// Each edge can be associated with a sequence of points that 
// define a path, which can then be used when drawing the graph.
// The location of these "bend" points is held in a separate
// vtkPoints object, accessed via the Set/GetBends function;
// bend points are NOT nodes.  Instead, a bend point is created
// by a call to CreateBend, and this bend can then be inserted
// at an appropriate position in the path for an edge.  Functions
// exist to return the bends for a given edge.  It is NOT 
// necessary to use bends, graphs are often drawn with edges as
// straight lines between source and target nodes, and this
// information is given by the assignment of points to nodes.
//
// Traversal of the graph is supported by the use of iterators
// (see vtkIdIterator).  Currently, iterators can be obtained
// over: all nodes, all edges, sources (nodes with in-degee 0),
// sinks (nodes with out-degree 0), neighbors, children and
// parents of a given node, and the edges into and out of a
// given node.  NOTE: Iterators are not (generally) thread-safe; 
// if the structure of the graph is being modifed during traversal,
// the iterator should be used to first copy the relevant part
// of the graph.
//
// .SECTION See Also
// vtkIndexedSet vtkIdMap vtkIdIterator vtkFieldData

#ifndef __vtkGraph_h
#define __vtkGraph_h

#include "vtkDataObject.h"
#include "vtkIdIterator.h"
#include "vtkIndexedSet.h"
#include "vtkFieldData.h"
#include "vtkPoints.h"
#include "vtkBitArray.h"
#include "vtkObjectFactory.h"
#include "vtkSystemIncludes.h"
#include "vtkIdArrayIterator.h"
#include "vtkObject.h"
#include "vtkIdTypeArray.h"
#include "vtkIdIterator.h"
#include "vtkIdMap.h"
#include "vtkStringTable.h"
#include "vtkDataArraySelection.h"

//BTX
enum GraphAttributeType
{
	NODE_DATA = 0,
	EDGE_DATA = 1
};
//ETX

class VTK_EXPORT vtkGraph : public vtkDataObject
{
public:
  static vtkGraph *New();
  vtkTypeRevisionMacro(vtkGraph, vtkDataObject);
  void PrintSelf(ostream& os, vtkIndent indent);

	void SetUpdateExtent(int piece, int numPieces, int ghostLevel);
	
	// Description:
	// Set a flag determining the amount of detail to be 
	// printed.  At 0 (default), only the number of nodes
	// and edges are displayed.  At 1, the topological
	// structure is printed.  At 2, the location of each
	// node is displayed.
	vtkSetMacro(PrintLevel, int);
	vtkGetMacro(PrintLevel, int);

  //---------------------------------------------------------
	// House-keeping methods.
	
  // Description:
  // Release all data and reset graph properties.
  void Initialize();

  // Description:
  // Copy a graph (i.e. duplicate all stored structures).
  void DeepCopy(vtkDataObject *src);

  // Description:
  // Copy a graph by referencing the internal data arrays.
  void ShallowCopy(vtkDataObject *src);

	// Description:
	// Set the initial space allocated for storage of nodes
	// and edges.  The graph can grow beyond these bounds, but
	// setting inital values reduces the amount of memory
	// reallocation and copying needed as the structure grows.
	void SetInitialNodeSize(vtkIdType nrNodes);
	void SetInitialEdgeSize(vtkIdType nrEdges);
	
	// Description:
	// Set the size of the hash tables used internally to map
	// IDs to indexes.  The internal classes will rehash existing
	// entries, and this method can be called at any time.
	void SetInternalIndexSizes(vtkIdType nodeXS, vtkIdType edgeXS);

	// Description:
	// Compact space required for the graph by squeezing all 
	// internal arrays to minimum required to hold structure.
	// This is particularly useful if the initial size of the
	// graph has been over-estimated to constrain reallocation
	// of storage.
	void Squeeze();
	
  //---------------------------------------------------------
	// Creation and deletion of nodes and edges.

  // Description:
  // Create a node in the graph. An ID for the node is returned.
  vtkIdType CreateNode();
  
  // Description:
  // Create a node in the graph, using the given ID.  The Id
	// is returned.
  vtkIdType CreateNode(vtkIdType nodeId);
  
  // Description:
  // Create an edge between the nodes given by src and dst IDs.
  // The Id of the new edge is returned.
  vtkIdType CreateEdge(vtkIdType srcId, vtkIdType dstId);
  
  // Description:
  // Create an edge between two nodes, using the given edge ID.
  // The Id of the edge is returned.
  vtkIdType CreateEdge(vtkIdType edgeId, vtkIdType srcId, vtkIdType dstId);
  
	// Description:
	// Set the size of the edge array for a given node.  This is 
	// provided for cases where the number of edges is known in
	// advance, and prevents repeated re-allocation of edge space
	// in large graphs.
	void SetEdgeArraySize(vtkIdType nodeId, vtkIdType size);
	
  // Description:
  // Delete a given node and all associated data, including any
  // incident edges.
  void DeleteNode(vtkIdType nodeId);
  
  // Description:
  // Delete an edge, and any associated data.  The nodes
  // associated with the edge are not deleted from the
  // dataset.
  void DeleteEdge(vtkIdType edgeId);
  void DeleteEdge(vtkIdType srcId, vtkIdType dstId);
  
  //---------------------------------------------------------
	// Edge bend operations
	
	// Description:
	// Create a point representing a bend in an edge.  The
	// index of the point is returned.
	vtkIdType CreateBend();

	// Description:
	// Insert a bend point into an edge.  The bend is placed
	// at the given index of the bend points.  Any existing
  // bend points are moved up in the array, which is 
	// resized if necessary.
	void InsertBend(vtkIdType edgeId, int posn, vtkIdType bend);

	// Description:
	// Set the postion of a bend point.
	void SetBendPosition(vtkIdType bend, double x, double y, double z);
	void SetBendPosition(vtkIdType bend, double *position);

	// Description:
	// Get the position of a bend point.
	void GetBendPosition(vtkIdType bend, double &x, double &y, double &z);
	double *GetBendPosition(vtkIdType bend);
	void GetBendPosition(vtkIdType bend, double position[3]);

	// Description:
	// Set a specific component of a given bend's position.
	void SetBendX(vtkIdType nodeId, double x);
	void SetBendY(vtkIdType nodeId, double x);
	void SetBendZ(vtkIdType nodeId, double x);

	// Description:
	// Get a specific component of a given bend's position.
	double GetBendX(vtkIdType nodeId);
	double GetBendY(vtkIdType nodeId);
	double GetBendZ(vtkIdType nodeId);

	// Description:
	// Find the number of bend points.  
	vtkIdType GetNumberOfBends();

	// Description:
	// Get number of bend points for a given edge.
	vtkIdType GetNumberOfBends(vtkIdType edgeId);

	// Description:
	// Get the bends of an edge.
	vtkIdType *GetBendArray(vtkIdType edgeId);

	// Description:
	// Get/set the collection of points for bends
	vtkSetObjectMacro(Bends,vtkPoints);
	vtkGetObjectMacro(Bends,vtkPoints);

  //---------------------------------------------------------
	// Query operations
	
  // Description:
  // Query if there is a node/edge with a given Id in the graph.
  int HasNode(vtkIdType nodeId);
  int HasEdge(vtkIdType edgeId);
	
  // Description:
  // Query if there is an edge between two given nodes.
	// Here, edge direction is taken into account.
	int HasEdge(vtkIdType srcId, vtkIdType dstId);
  
  // Description:
  // Return the number of nodes and edges recorded.
  vtkIdType GetNumberOfNodes();
  vtkIdType GetNumberOfEdges();
  
  // Description:
  // Get the internal index of a node/edge.  This is the index
	// at which attribute data is stored for the node/edge.
  vtkIdType GetNodeIndex(vtkIdType nodeId);
  vtkIdType GetEdgeIndex(vtkIdType edgeId);
  
  // Description:
  // Return the Id of a node/edge currently using a given index.
	// A second form of GetEdgeId is provided for cases where the
	// source and destination nodes are known, but not the edgeId.
  vtkIdType GetNodeId(vtkIdType nodeIx);
  vtkIdType GetEdgeId(vtkIdType edgeIx);
	vtkIdType GetEdgeId(vtkIdType srcId, vtkIdType dstId);
	
	// Description;
	// Return the maximum node/edge Id in use.
	vtkIdType GetMaxNodeId();
	vtkIdType GetMaxEdgeId();

  // Description:
  // Return the degree of a given node (i.e. number of edges on
  // which it lies).  Similar functions are provided specifically
  // for the in- and out-degree.
  int GetDegree(vtkIdType nodeId);
  int GetNumberOfParents(vtkIdType nodeId);
  int GetNumberOfChildren(vtkIdType nodeId);
  
  // Description:
  // Return the source and/or target node Ids of a given edge.
  vtkIdType GetSource(vtkIdType edgeId);
  vtkIdType GetTarget(vtkIdType edgeId);
  void GetEdge(vtkIdType edgeId, vtkIdType &srcId, vtkIdType &dstId);
  
  //---------------------------------------------------------
	// Iterators.  NOTE: Iterators are NOT thread-safe.
	
	// Description:
	// Return an iterator traversing all the nodes/edges of 
	// the graph.
  vtkIdIterator *GetNodes();
	vtkIdIterator *GetEdges();
	
	// Description:
	// Return iterators to traverse the sources (nodes with
	// in-degree 0) or sinks (nodes with out-degree 0).
	vtkIdIterator *GetSources();
	vtkIdIterator *GetSinks();

	// Description:
	// Return an iterator traversing all the nodes in the graph
	// that neighbour the given node.
	vtkIdIterator *GetNeighbors(vtkIdType nodeId);

	// Description:
	// Return an iterator traversing all the parents of the
	// given node: A is a parent of B if there is an edge
	// from A to B in the graph.
	vtkIdIterator *GetParents(vtkIdType nodeId);
	
	// Description:
	// Return an iterator traversing all the children of the
	// given node: A is a child of B if there is an edge
	// from B to A in the graph.
	vtkIdIterator *GetChildren(vtkIdType nodeId);
	
	// Description:
	// Return an iterator traversing all the edges that have
	// the given node as source or target.
	vtkIdIterator *GetEdges(vtkIdType nodeId);
	
	// Description:
	// Return an iterator traversing all the edges that have
	// the given node as target.
	vtkIdIterator *GetIncomingEdges(vtkIdType nodeId);
	
	// Description:
	// Return an iterator traversing all the edges that have
	// the given node as source.
	vtkIdIterator *GetOutgoingEdges(vtkIdType nodeId);

  	
	// Description:
	// Return an iterator over the bends of an edge.
	vtkIdIterator *GetBends(vtkIdType edgeId);

  //---------------------------------------------------------
	// Attributes for node and edge data.
	
	vtkGetObjectMacro(NodeData,vtkFieldData);
	vtkGetObjectMacro(EdgeData,vtkFieldData);
	vtkFieldData *GetAttributeData(int location);

	// Description:
	// Set/get the table used internally to hold strings
	vtkSetObjectMacro(Strings,vtkStringTable);
	vtkGetObjectMacro(Strings,vtkStringTable);

	// Description:
	// Set/get the tables recording which arrays hold string indices
	vtkSetObjectMacro(NodeDataStringArrays,vtkDataArraySelection);
	vtkGetObjectMacro(NodeDataStringArrays,vtkDataArraySelection);
	vtkSetObjectMacro(EdgeDataStringArrays,vtkDataArraySelection);
	vtkGetObjectMacro(EdgeDataStringArrays,vtkDataArraySelection);

	// Description:
	// Indicate that the named data array indexes strings
	void MarkAsNodeDataStringArray(const char *name);
	void MarkAsEdgeDataStringArray(const char *name);
	void MarkAsDataStringArray(int location, const char *name);

	// Description:
	// Disable use of named data array as string indexes
	void DisableNodeDataStringArray(const char *name);
	void DisableEdgeDataStringArray(const char *name);
	void DisableDataStringArray(int location, const char *name);

	// Description:
	// Query whether a given array indexes strings
	int IsNodeDataStringArray(const char *name);
	int IsEdgeDataStringArray(const char *name);
	int IsDataStringArray(int location, const char *name);

  //---------------------------------------------------------
	// Node positions:
	
	// Description:
	// Set/get the vtkPoints object used to record the position
	// of a node.  Within the vtkPoints objects, the point for 
	// node n is stored at the index for that node, obtained via 
	// GetNodeIndex.  The operations given below perform this
	// mapping themselves.
  vtkSetObjectMacro(Layout,vtkPoints);
  vtkGetObjectMacro(Layout,vtkPoints);

	// Description:
	// Set/get the position of a given node.
	void SetPosition(vtkIdType nodeId, double x, double y, double z);
	void GetPosition(vtkIdType nodeId, double& x, double& y, double& z);
	double *GetPosition(vtkIdType nodeId);
	void SetPosition(vtkIdType nodeId, double *position);
	void GetPosition(vtkIdType nodeId, double position[3]);

	// Description:
	// Set a specific component of a given node's position.
	void SetX(vtkIdType nodeId, double x);
	void SetY(vtkIdType nodeId, double x);
	void SetZ(vtkIdType nodeId, double x);

	// Description:
	// Get a specific component of a given node's position.
	double GetX(vtkIdType nodeId);
	double GetY(vtkIdType nodeId);
	double GetZ(vtkIdType nodeId);

  //---------------------------------------------------------
	// General tests.
	
  // Description:
  // Check whether or not the graph is acyclic.
  int IsAcyclic();
	
protected:
  vtkGraph();
  ~vtkGraph();

	//BTX
	vtkIndexedSet<_vtkNode_s> *Nodes;
	vtkIndexedSet<_vtkEdge_s> *Edges;
	//ETX

	// Properties of nodes and edges.
	vtkFieldData *NodeData;
	vtkFieldData *EdgeData;

	// String table - maps ints to strings
	vtkStringTable *Strings;

	// Record of which data arrays are indexes of strings
	vtkDataArraySelection *NodeDataStringArrays;
	vtkDataArraySelection *EdgeDataStringArrays;

	// Location of nodes, and location of points defining edge bends.
	vtkPoints *Layout;
	vtkPoints *Bends;

	int PrintLevel;

	// Description:
	// Utility functions for graph manipulation: add an edge to a
	// node, and remove an edge from a node.
	void AddEdge(vtkIdType nodeId, vtkIdType edgeId, int isTarget);
	void RemoveEdgeFromNode(vtkIdType edgeId, vtkIdType nodeId);

  // Description:
  // Helper functions for testing acyclic property.
	int TestAcyclic(
		vtkIdType node, 
		vtkBitArray *seen, 
		vtkBitArray *done);
	
private:
  vtkGraph(const vtkGraph&) {};
  void operator=(const vtkGraph&) {};
};

inline vtkIdType vtkGraph::GetNumberOfNodes()
{ return this->Nodes->GetNumberOfElements(); }

inline vtkIdType vtkGraph::GetNumberOfEdges()
{ return this->Edges->GetNumberOfElements(); }

#endif	/* __vtkGraph_h */

