/*
 *	File: vtkReingoldTilfordLayout.cxx
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
 *	2003/12/09: modified so that node size is determined by "size"
 *	            array if present, but otherwise defaults to 1.0
 */

#include "vtkReingoldTilfordLayout.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"
#include "vtkFloatArray.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkReingoldTilfordLayout);
vtkCxxRevisionMacro(vtkReingoldTilfordLayout, "1.0");

vtkReingoldTilfordLayout::vtkReingoldTilfordLayout()
{
  this->Spacing  = 10.0;
}


/*
 * Initialise a node for RT-layout.  This involves finding,
 * for each node, the left-hand brother of the node, i.e. the
 * node at the same depth in the tree that would be to the 
 * immediate left of this node.  This is achieved by doing a
 * traversal of the tree.
 * 
 * The parameter "layer" represents the nodes at the level
 * of the tree currently being considered.
 */


void vtkReingoldTilfordLayout::Initialize(
	vtkIdType root,
	vtkIdType layer,
	vtkGraph *tree) 
{
	vtkIdIterator *children;
	
	vtkIdType index = tree->GetNodeIndex(root);
	
	this->LeftBrother[index] = this->Contour[layer];
	this->Contour[layer] = root;
	this->Layer[index] = layer;

	children = tree->GetChildren(root);
	while (children->HasNext())
		{
		this->Initialize(children->GetNext(), layer+1, tree);
		}
	children->Delete();
}


// Return actual x coordinate of a node, taking into account
// the position modifiers of its ancestors up to the root. 
double vtkReingoldTilfordLayout::TrueX(vtkIdType root, vtkGraph *tree) 
{
  vtkIdType node, index;
  double x;	
	vtkIdIterator *parents;
	int okay = 1;
	
	node = root;
  index = tree->GetNodeIndex(node);
  x = this->Xpreliminary[index];
  
	while (okay)
    {
    x += this->Xmodifier[index];
    parents = tree->GetParents(node);
		okay = parents->HasNext();
    if (okay)
      {
      node = parents->GetNext();
      index = tree->GetNodeIndex(node); 
      }
		parents->Delete();  
    }
  return x;
}


void vtkReingoldTilfordLayout::FirstWalk(vtkIdType root, vtkGraph *tree) 
{
  double modifier;

  double Xroot;
  double Xbrother;
	
  double leftCoord, rightCoord;
  double leftModif, rightModif;

  vtkIdType brother;
	vtkIdType index;
	vtkIdType leftChild, rightChild;
  vtkIdType leftIndex, rightIndex;
  vtkIdType nrChildren;

	vtkIdIterator *children;
  index = tree->GetNodeIndex(root);
  children = tree->GetChildren(root);
  nrChildren = tree->GetNumberOfChildren(root);

  // Case 1. Node is a leaf
  if (nrChildren == 0) 
    {
    brother = this->LeftBrother[index];
    if (brother < 0)
      {           
      Xroot = 0.0; 	// no left brother
      }
    else 
      {
			// Find coordinate relative to brother.  Include the
			// size of the node and a factor of 0.5 to allow some
			// minimum distance of either side.
  	  Xroot = this->TrueX(brother, tree) + this->Size[index]*1.5;
      }
		modifier = 0.0;
		}
  else 
    {
		// Apply FirstWalk to each child, taking care to
		// remember the last child, and then retrieve the
		// first child, as these will be the left and rightmost
		// children in the layout for this node.
		while (children->HasNext())
      {
			rightChild = children->GetNext();
			this->FirstWalk(rightChild, tree);
      }
		children->Initialize();
		leftChild = children->GetNext();

		// Position the node.
    leftIndex = tree->GetNodeIndex(leftChild);
    leftCoord = this->Xpreliminary[leftIndex];
    leftModif = this->Xmodifier[leftIndex];

    rightIndex = tree->GetNodeIndex(rightChild);
    rightCoord = this->Xpreliminary[rightIndex];
    rightModif = this->Xmodifier[rightIndex];

    Xroot = ((leftCoord+leftModif) + (rightCoord+rightModif)) / 2.0;
		modifier = 0.0;
		brother = this->LeftBrother[index];

    if (brother >= 0) 
      {
			Xbrother = this->TrueX(brother, tree);
      modifier = this->Size[index]*1.5 - (Xroot - Xbrother);
			if (modifier < 0.0) 
				{
				modifier = 0.0;
				}
      }
		}

  this->Xpreliminary[index] = Xroot;
  this->Xmodifier[index] = modifier;
	children->Delete();
}


void vtkReingoldTilfordLayout::SecondWalk(
  vtkIdType root, 
  double modifier, 
  vtkPoints *layout, 
  vtkGraph *tree ) 
{
  double p[3], childModif;
  vtkIdType nrChildren;
	vtkIdIterator *children;

  vtkIdType index = tree->GetNodeIndex(root);

  p[0] = this->Xpreliminary[index] 
			   + this->Xmodifier[index]
			   + modifier;
  p[1] = -1.0 * this->Layer[index] * this->Spacing;
  p[2] = 0.0;
  layout->SetPoint(index, p);

  childModif = modifier + this->Xmodifier[index];
	nrChildren = tree->GetNumberOfChildren(root);
	
	children = tree->GetChildren(root);
  while (children->HasNext())
    {
    this->SecondWalk(
			children->GetNext(), 
			childModif, 
			layout, 
			tree);
    }
	children->Delete();
}

void vtkReingoldTilfordLayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();

  vtkPoints *layout;
  
	vtkIdType node, edge, root, src, dst, index;
	vtkIdType nrNodes, nextEdgeId;
  vtkIdIterator *nodes, *edges;
	vtkFloatArray *size;
	vtkGraph *tree;
	
  // Initialize
  //
  vtkDebugMacro(<<"ReingoldTilfordLayout executing.");

  if (input->GetNumberOfNodes() == 0)
		{
		vtkErrorMacro(<<"Empty graph.");
    return;
		} 

	size = vtkFloatArray::SafeDownCast(
			input->GetNodeData()->GetArray("size"));

	nrNodes = input->GetNumberOfNodes() + 1;
	tree = vtkGraph::New();
	tree->SetInitialNodeSize(nrNodes);
	tree->SetInitialEdgeSize(input->GetNumberOfEdges());
	nextEdgeId = input->GetMaxEdgeId()+1;

	this->Xpreliminary = new double[nrNodes];
	this->Xmodifier    = new double[nrNodes];
	this->Size         = new double[nrNodes];
	this->LeftBrother  = new vtkIdType[nrNodes];
	this->Layer        = new vtkIdType[nrNodes];
	this->Contour      = new vtkIdType[nrNodes];
			
	root = tree->CreateNode(input->GetMaxNodeId()+1);
	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		tree->CreateNode(node);
		index = tree->GetNodeIndex(node);
		if (input->GetNumberOfParents(node) == 0)
			{
			tree->SetEdgeArraySize(node, input->GetDegree(node)+1);
			tree->CreateEdge(nextEdgeId++, root, node);
			}
		else
			{
			tree->SetEdgeArraySize(node, input->GetDegree(node));
			}
		this->Size[index] = 
			(size ? size->GetValue(input->GetNodeIndex(node)) : 1.0);
		}
	nodes->Delete();
	this->Size[tree->GetNodeIndex(root)] = 1.0;
	
	for (vtkIdType i = 0; i < nrNodes; i++)
		{
		this->Xpreliminary[i] = 0;
		this->Xmodifier[i] = 0;
		this->Contour[i] = -1;
		this->LeftBrother[i] = -1;
		}
		
	edges = input->GetEdges();
	while (edges->HasNext())
		{
		edge = edges->GetNext();
		input->GetEdge(edge, src, dst);
		tree->CreateEdge(edge, src, dst);
		}
	edges->Delete();
	
  // Allocate arrays and storage for layout.
	
  layout = vtkPoints::New();
  layout->SetNumberOfPoints(nrNodes);
		
  this->Initialize(root, 0, tree);
  this->FirstWalk(root, tree);
  this->SecondWalk(root, 0.0, layout, tree);
	
	output->ShallowCopy(input);
	output->SetLayout(vtkPoints::New());
	
	nodes = output->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		output->SetPosition(node, layout->GetPoint(tree->GetNodeIndex(node)));
		}
	nodes->Delete();
	
	tree->Delete();
	layout->Delete();
	
  delete [] this->Xpreliminary;
  delete [] this->Xmodifier;
  delete [] this->LeftBrother;
  delete [] this->Layer;
  delete [] this->Contour;
	delete [] this->Size;
	  	
	vtkDebugMacro(<<"ReingoldTilfordLayout complete.");
}

void vtkReingoldTilfordLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
  os << indent << "Reingold-Tilford Layout: " << "\n";
}
