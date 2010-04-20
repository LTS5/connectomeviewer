/*
 *	File: vtkSpanLayout.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkSpanLayout.h"
#include "vtkObjectFactory.h"
#include "vtkIntArray.h"
#include "vtkFloatArray.h"
#include "vtkIdIterator.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkSpanLayout);
vtkCxxRevisionMacro(vtkSpanLayout, "1.0");

vtkSpanLayout::vtkSpanLayout()
{
  this->TreeLayout = NULL;
	this->SpanningDAG = NULL;
	this->Spacing = 10.0;
}

vtkSpanLayout::~vtkSpanLayout()
{
  if (this->TreeLayout)
  	{
		this->TreeLayout->Delete();
  	this->TreeLayout = NULL;
		}
  if (this->SpanningDAG)
  	{
		this->SpanningDAG->Delete();
  	this->SpanningDAG = NULL;
		}
}


// Edges that cross levels of the spanning DAG will be
// replaced by two or three edges involving new intermediate
// nodes to accommodate bends.  The following structure
// tracks the replacements that are made to the graph.

struct _vtkBridge_s
	{
	vtkIdType edge;
	vtkIdType source;
	vtkIdType target;
	vtkIdType delta;
	vtkIdType anchor[2];
	};

void vtkSpanLayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();

	// Intermediate graphs used in the layout process.
  vtkGraph *dag;
  vtkGraph *tree;
	vtkGraph *layout;

  vtkIdType nrNodes, nrEdges;
	vtkIdType nextFreeNodeId;
	vtkIdType nextFreeEdgeId;
	vtkIdType edgeId;
	vtkIdType *edgesCopy;
  vtkIdType edge, src, dst, node, child, nIndex, cIndex;

  vtkIdType tmpNode1, tmpNode2;
  vtkIdType tmpEdge1, tmpEdge2, tmpEdge3;
  vtkIdType startNode;
  vtkIdType delta;
  vtkIdType srcLevel, dstLevel;
  vtkIdType indeg; 

	
	// Iterators for traversing graphs.
	vtkIdIterator *children;
	vtkIdIterator *edges, *nodes;

	// Node properties used during layout; the node size
	// property is obtained from the input graph, but has
	// to be extended during layout, while the dagLevel
	// property is computer wholly within the algorithm.
	vtkFloatArray *treeNodeSize;
	vtkFloatArray *inputNodeSize;
 
	// Variables for computing maximum DAG level of each node. 
	vtkIntArray *dagLevel;
	vtkIdType *queue;
	vtkIdType head, tail;
	vtkIdType *inDegree;
  vtkIdType level, clevel;
  
	// editlist is the list of edits made to the graph to
	// address edges that span more than one DAG level.
	_vtkBridge_s *editlist;
	_vtkBridge_s link;
	vtkIdType editsize;
  vtkIdType bend;
	
	// ----------------------------------------------------------
	
  vtkDebugMacro(<<"vtkSpanLayout executing.");
  
	// Ensure that all required inputs are available.
  nrNodes = input->GetNumberOfNodes();
  nrEdges = input->GetNumberOfEdges();

  if (nrNodes == 0 || nrEdges == 0) 
    {
		vtkErrorMacro(<< "No points or no edges in graph." );
		return;
		}
  if (!this->TreeLayout)
  	{
		vtkErrorMacro(<< "No tree layout filter.");
		return;
		}
  if (!this->SpanningDAG)
  	{
		vtkErrorMacro(<< "No spanning tree filter.");
		return;
		}
	// Copy attribute data of graph to output, and prepare 
	// a fresh set of points to hold layout.
	output->GetNodeData()->ShallowCopy(input->GetNodeData());
	output->GetEdgeData()->ShallowCopy(input->GetEdgeData());
	output->SetLayout(vtkPoints::New());
	output->SetStrings(input->GetStrings());
	output->SetNodeDataStringArrays(input->GetNodeDataStringArrays());
	output->SetEdgeDataStringArrays(input->GetEdgeDataStringArrays());

	// Compute a spanning DAG for the graph using the 
	// filter provided.  NOTE: we force the SpanningDAG
	// filter to execute by marking it as modified; if we
	// did not do so, and this filter re-executes, the 
	// DAG will not be reconstructed.  We could get around
	// this by leaving the output (rather than initializing
	// it once it has been used).
	this->SpanningDAG->SetInput(input);
	this->SpanningDAG->Modified();
	this->SpanningDAG->Update();
  dag = (vtkGraph*)this->SpanningDAG->GetOutput();

	// From the spanning DAG and input, a tree will be
	// generated for subsequent input to the tree layout 
	// filter. The estimate of initial size is only
	// approximate, as new nodes and edges will be added
	// and later removed.
  tree = vtkGraph::New();
	tree->SetInitialNodeSize(input->GetNumberOfNodes() + 2*input->GetNumberOfEdges()); 
	tree->SetInitialEdgeSize(input->GetNumberOfEdges() * 4);
	tree->SetInternalIndexSizes(
			(vtkIdType)(input->GetNumberOfNodes()*1.5),
			(vtkIdType)(input->GetNumberOfEdges()*1.5));
	nextFreeNodeId = input->GetMaxNodeId() + 1;
	nextFreeEdgeId = input->GetMaxEdgeId() + 1;
	
	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		tree->CreateNode(node);
		tree->SetEdgeArraySize(node, input->GetDegree(node));
		}
	nodes->Delete();

  // Copy all spanning DAG edges APART from any loops.
  edges = input->GetEdges();
  while (edges->HasNext())
    {
		edgeId = edges->GetNext();
		src = input->GetSource(edgeId);
		dst = input->GetTarget(edgeId);
		if (dag->HasEdge(edgeId))
	  	{
	  	tree->CreateEdge(edgeId, src, dst);
	  	}
  	else
	  	{
	  	if (src != dst)
				{
				tree->CreateEdge(edgeId, dst, src);
				}
	  	else
    		{
				// Currently loops are simply ignored.
				
				// Create loop by inserting new nodes and edges.  Care is
				// needed to ensure that ids of new nodes and edges will
				// not clash with any remaining node/edge ids from the
				// input graph.
	/*
				tmpNode1 = tree->CreateNode(nextFreeNodeId++);
				tmpNode2 = tree->CreateNode(nextFreeNodeId++);
				tmpEdge1 = tree->CreateEdge(nextFreeEdgeId++, src, tmpNode1);
				tmpEdge2 = tree->CreateEdge(nextFreeEdgeId++, tmpNode1, tmpNode2);
				tmpEdge3 = tree->CreateEdge(nextFreeEdgeId++, tmpNode2,dst);

				edit.oldEdge = edge;
				edit.nrEdges = 3;
				edit.src = new vtkIdType[3];
				edit.dst = new vtkIdType[3];
				edit.src[0] = src;      edit.dst[0] = tmpNode1;
				edit.src[1] = tmpNode1; edit.dst[0] = tmpNode2;
				edit.src[2] = tmpNode2; edit.dst[0] = dst;
				editlist.push_front(edit);
	*/
    		}
	  	}
		}	/* edges of input graph */
	edges->Delete();

	// Allocate a new start node; this covers the case that the
	// spanning DAG contains disconnected components.  The 
	// start node is then linked to all sources (nodes with
	// indegree zero) of the tree, other than the startNode.
  startNode = tree->CreateNode(nextFreeNodeId++);  
  nodes = tree->GetSources();
  while (nodes->HasNext())
    {  
		node = nodes->GetNext();
    if (node != startNode)
      {
	  	tree->CreateEdge(nextFreeEdgeId++, startNode, node);
      }
    }
	nodes->Delete();

  // Compute the DAG level for each node in the graph.
	// The level is stored as a VTK data array ...
	dagLevel = vtkIntArray::New();
	dagLevel->SetNumberOfValues(tree->GetNumberOfNodes());
	/*
	 * Add the property to the tree node data.  This step is
	 * important, as if we delete a node from the tree, the
	 * mapping from node to data may be changed, and the 
	 * tree must therefore manage this attribute.
	 */
	dagLevel->SetName("Dag Level");
	tree->GetNodeData()->AddArray(dagLevel);
	
	// First pass of indegree assigns depth to nodes based on 
	// a topological traversal.  For this, we need to store
	// the indegree of each node.
	inDegree = new vtkIdType[tree->GetNumberOfNodes()];
	
	for (int i = 0; i < tree->GetNumberOfNodes(); i++)
		{
		dagLevel->SetValue(i,0);
		inDegree[i] = tree->GetNumberOfParents(tree->GetNodeId(i));
		}
		
	// queue = those nodes whose indegree has fallen to 0.
	queue = new vtkIdType[tree->GetNumberOfNodes()];
	head = 0;
	tail = 0;
	queue[head++] = startNode;

	while (head != tail)
		{
		node = queue[tail++];
		nIndex = tree->GetNodeIndex(node);
		// For each child, decrement the indegree; if this has
		// gone to 0, add the child to the queue, and set its
		// daglevel to one more than that of its parent.
		children = tree->GetChildren(node);
    while (children->HasNext())
			{
	  	child = children->GetNext();
			cIndex = tree->GetNodeIndex(child);
			inDegree[cIndex]--;
			if (inDegree[cIndex] == 0)
				{
				dagLevel->SetValue(cIndex, dagLevel->GetValue(nIndex)+1);
				queue[head++] = child;
				}
			}
		children->Delete();
		}
	
	// Revisit each node, in reverse order, i.e. those with
	// highest daglevel first.  For each node, set its DAG level to
	// be one less than minimum DAGlevel of its children.
	while (tail > 0)
		{
		node = queue[--tail];
		nIndex = tree->GetNodeIndex(node);
		children = tree->GetChildren(node);
		if (children->HasNext())
			{
			cIndex = tree->GetNodeIndex(children->GetNext());
			level = dagLevel->GetValue(cIndex);
    	while (children->HasNext())
				{
				cIndex = tree->GetNodeIndex(children->GetNext());
				clevel = dagLevel->GetValue(cIndex);
				if (clevel < level)
					{
					level = clevel;
					}
				}
			dagLevel->SetValue(nIndex, level-1);
			}
		children->Delete();
		}
	delete [] queue;
	delete [] inDegree;	
	
	// In the subsequent step, new nodes and edges will be added
	// to the tree.  As iterators are not thread-safe, first we
	// make a copy of the existing edges, held in an array.
 	nrEdges = tree->GetNumberOfEdges();
 	edgesCopy = new vtkIdType[nrEdges];
 	edges = tree->GetEdges();
 	for (vtkIdType ed = 0; edges->HasNext(); ed++)
		{
   	edgesCopy[ed] = edges->GetNext();
		}
	edges->Delete();

	// Check each edge to see if it spans more than one DAG-level.
	// If it does, the edge will need to be replaced with a pair
	// or trio of new edges; we assume the latter, and create
	// new intermediate nodes and edges, and record the change to
	// be made within an edit list.
	
	editsize = 0;
	editlist = new _vtkBridge_s[nrEdges];
	
	for (vtkIdType e = 0; e < nrEdges; e++)
		{
		edge = edgesCopy[e];
		src = tree->GetSource(edge);
		dst = tree->GetTarget(edge);

		srcLevel = dagLevel->GetValue(tree->GetNodeIndex(src));
		dstLevel = dagLevel->GetValue(tree->GetNodeIndex(dst));
		delta = dstLevel - srcLevel;

		if (delta > 1)
     	{
			link.edge = edge;
			link.source = src;
			link.target = dst;
			link.delta = delta;
			
			tmpNode1 = tree->CreateNode(nextFreeNodeId++);
			dagLevel->InsertValue(tree->GetNodeIndex(tmpNode1), srcLevel+1);
			link.anchor[0] = tmpNode1;
			
			tmpEdge1 = tree->CreateEdge(nextFreeEdgeId++, src, tmpNode1);

			if (delta > 2)
				{
				tmpNode2 = tree->CreateNode(nextFreeNodeId++);
				dagLevel->InsertValue(tree->GetNodeIndex(tmpNode2), dstLevel-1);
				link.anchor[1] = tmpNode2;
			
				tmpEdge2 = tree->CreateEdge(nextFreeEdgeId++, tmpNode1, tmpNode2);
				tmpEdge3 = tree->CreateEdge(nextFreeEdgeId++, tmpNode2, dst);
				}
			else
				{
				tmpEdge2 = tree->CreateEdge(nextFreeEdgeId++, tmpNode1, dst);
				}
			editlist[editsize++] = link;	
 			}
		}		/* for each copied edge */
	delete [] edgesCopy;

	// Iterate over the list of edits, removing all of the
	// original edges from the graph.
	for (vtkIdType ri = 0; ri < editsize; ri++)
		{
		link = editlist[ri];
		edge = link.edge;
		if (tree->HasEdge(edge)) 
			{
			tree->DeleteEdge(edge);
			}
		}

	// The tree layout may take into account node size.  By convention
	// in this library, this is communicated via the "size" attribute
	// of a node.  However, the nodes added to the tree in the
	// previous step will not have this attribute.  Changing the input 
	// node data is Not A Good Idea; instead if sizes are given on 
	// the input, a new data array is created to hold node sizes for layout. 

	inputNodeSize = vtkFloatArray::SafeDownCast(
		input->GetNodeData()->GetArray("size"));
	treeNodeSize = NULL;
	if (inputNodeSize)	
		{
		treeNodeSize = vtkFloatArray::New();
		treeNodeSize->SetNumberOfTuples(tree->GetNumberOfNodes());
		treeNodeSize->SetName("size");
		tree->GetNodeData()->AddArray(treeNodeSize);
		}

	// Traverse the tree nodes; if the "tree" node still has
	// more than one parent (possible, as it was copied from 
	// a dag, remove all but one of the parents.  If the node
	// came from input, copy its size, otherwise set its
	// size to 1.0 (a default value).
 	nodes = tree->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		indeg = tree->GetNumberOfParents(node);
		edges = tree->GetIncomingEdges(node);
		if (indeg > 1)
			{
			edgesCopy = new vtkIdType[indeg-1];
  		for (vtkIdType i = 0; i < indeg-1; i++)
				{
				edgesCopy[i] = edges->GetNext();
				}
			for (vtkIdType id = 0; id < indeg-1; id++)
				{
				tree->DeleteEdge(edgesCopy[id]);
				}
			delete [] edgesCopy;
			}
		edges->Delete();

		// If node sizes are given explicitly, we have to copy
		// into the new array.
		if (!inputNodeSize)
			{
			continue;
			}
		if (input->HasNode(node))
			{
			treeNodeSize->SetValue(
				tree->GetNodeIndex(node),
				inputNodeSize->GetValue(input->GetNodeIndex(node)));
			}
		else
			{
			treeNodeSize->SetValue(tree->GetNodeIndex(node), 1.0);
			}
		}
	nodes->Delete();

	//  Layout the tree using the layout filter provided.
	this->TreeLayout->SetInput(tree);
	this->TreeLayout->Update();
	layout = TreeLayout->GetOutput();
	if (treeNodeSize)
		{
		treeNodeSize->Delete();
		}
	tree->Delete();
 

	// Copy the nodes from input to output, setting the
	// position of each node in the output.  We also pre-allocate
	// space for the edges, using the number of edges of that 
	// node from the input graph. Note that the
	// z-coordinate will later be overwritten to impose the
	// spanning DAG levels on the graph.

	output->SetInitialNodeSize(
		input->GetNumberOfNodes() + editsize*2);
	output->SetInitialEdgeSize(
		input->GetNumberOfEdges() + editsize*3);
		
	// Copy the nodes of the input dataset to the output;
	// the actual nodes will be the same.
	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		output->CreateNode(node);
		output->SetEdgeArraySize(node, input->GetDegree(node));
		output->SetPosition(node, layout->GetPosition(node));
		// z-coordinates of all nodes are now determined based on
		// their dag level.
		output->SetZ(node, this->Spacing*(dagLevel->GetValue(
			layout->GetNodeIndex(node))));
		}
 	nodes->Delete();

	// Copy edges of the input graph to the output; note
	// that loops are omitted (the algorithm can be modified
	// to deal with loops, if required).
 	edges = input->GetEdges();
 	while (edges->HasNext())
		{
		edge = edges->GetNext();
		if (input->GetSource(edge) != input->GetTarget(edge))
			{
			output->CreateEdge(
				edge, 
				input->GetSource(edge),
				input->GetTarget(edge));
			}
		}
	edges->Delete();

	// Prepare to apply edits to the output graph, replacing
	// single level-crossing edges with pairs or triples as
	// defined in the edit list.  As part of this process, 
	// bends will be created in the output dataset.  

	// Process the editlist.
	for (vtkIdType ei = 0; ei < editsize; ei++)
		{
		link = editlist[ei];
		if (!output->HasEdge(link.edge))
			{
			continue;
			}
		// Create the first bend point
		bend = output->CreateBend();
		output->SetBendPosition(bend, layout->GetPosition(link.anchor[0]));
		output->SetBendZ(bend, this->Spacing*(dagLevel->GetValue(
			layout->GetNodeIndex(link.anchor[0]))));
		output->InsertBend(link.edge, 0, bend);

		// If the delta between src and dst was greater than two, there 
		// is a second bend point to define.
		// separate case.
		if (link.delta > 2)
			{
			bend = output->CreateBend();
			output->SetBendPosition(bend, layout->GetPosition(link.anchor[1]));
			output->SetBendZ(bend, this->Spacing*(dagLevel->GetValue(
				layout->GetNodeIndex(link.anchor[1]))));
			// If the edge was reversed in building the DAG, the order of
			// the bend points has to swap around.
			if (dag->HasEdge(link.edge))
				{
				output->InsertBend(link.edge, 1, bend);
				}
			else
				{
				output->InsertBend(link.edge, 0, bend);
				}
			}
		}
	delete [] editlist;

	dag->Initialize();
	layout->Initialize();

	vtkDebugMacro(<<"SpanLayout complete.");
}

void vtkSpanLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
  os << indent << "vtkSpanLayout\n";
}


