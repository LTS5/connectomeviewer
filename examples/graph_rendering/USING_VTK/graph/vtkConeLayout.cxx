/*
 *	File: vtkConeLayout.h
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
 *	2003/11/15:	modified layout to use Auber's refinement.
 */

#include "vtkConeLayout.h"
#include "vtkObjectFactory.h"
#include "vtkMath.h"
#include "vtkGraph.h"
#include "vtkFloatArray.h"
#include "vtkIdIterator.h"
#include "vtkPoints.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkConeLayout);
vtkCxxRevisionMacro(vtkConeLayout, "1.10");

vtkConeLayout::vtkConeLayout()
{
  this->Compression = 0;
  this->Compactness = 0.75;
  this->Spacing = 8.0;
}

//----------------------------------------------------------------------------

// Position the tree rooted at "root"; node positions are recorded in
// "posn", topology is available through the instance of vtkGraph.
// The allocated posiition is not final; each sub-tree is positioned
// on the assumption that its parent is located at the origin, and all
// nodes are positioned in the plane z=0.  This will be corrected in a
// second pass over the tree.
//
// The return value is the radius of the circle containing the layout
// of this (sub)-tree.


double vtkConeLayout::LocalPlacement(
  vtkIdType node, 
	vtkFloatArray *size,
	vtkGraph *graph)
{
  vtkIdType child;
  vtkIdType prevChild;	
	vtkIdType nrChildren;

	vtkIdIterator *children;

  double circum = 0;      // circum of cone below this node
  double radius;          // radius of cone
  double dAlpha;
  double alpha = 0;
  double largest = 0.0;    // size of largest cone of any child
  
	double *radii;
	
	double cx, cy, cr, vx, vy, norm, i1x, i1y, i3x, i3y;
	
  // Initially position this node at the origin ...
	graph->SetPosition(node, 0.0, 0.0, 0.0);
	
	nrChildren = graph->GetNumberOfChildren(node);

  // If there are no children, we are done; return size.
  if (nrChildren == 0)
    {
    return (size ? size->GetValue(graph->GetNodeIndex(node)) : 1.0);
    }
		
	children = graph->GetChildren(node);
  if (nrChildren == 1)
    {
  	// For one child, simply position that child; radius required 
  	// for the tree will then depend on radius required for that 
  	// child.
    radius = this->LocalPlacement(
			children->GetNext(),
			size,
			graph);
		children->Delete();
		return radius;
    }

  // If there is more than one child nodes ...
	radii = new double[nrChildren];
  for (vtkIdType i = 0; i < nrChildren; i++)
    {
    // Layout the next child and record its raidus.  If 
		// necessary update the size of largest child cone.  
		// Accumulate the (approximate) arc-length 
		// required by child nodes as "circum".
		
    child = children->GetNext();
		radii[i] =  this->LocalPlacement(child, size, graph);
    circum += radii[i]*2.0;
    if (radii[i] > largest) 
      {
      largest = radii[i];
      }
    }
  radius = circum / (2.0*vtkMath::Pi());

  // Iterate over the children, assigning the node of each 
  // a position around a circle of the required radius, 
  // based on the radii of that child and its predecessor,

  prevChild = nrChildren-1;

	children->Initialize();
  for (vtkIdType j = 0; j < nrChildren; j++)
    {
    child = children->GetNext();
    dAlpha = (radii[j] + radii[prevChild]) / radius;
    alpha  += dAlpha;
		graph->SetPosition(
			child,
			radius*cos(alpha),
			radius*sin(alpha),
			0.0);
    prevChild = j;
		if (j == 0)
			{
			cx = radius*cos(alpha);
			cy = radius*sin(alpha);
			cr = radius;
			}
		else
			{
			vx = cx - radius*cos(alpha);
			vy = cy = radius*sin(alpha);
			norm = sqrt(vx*vx + vy*vy);
			if (norm == 0.0)
				{
				continue;
				}
			vx /= norm;
			vy /= norm;
			i1x = cx - vx*cr;
			i1y = cy - vy*cr;
			i3x = radius*cos(alpha) - radius*vx;
			i3y = radius*sin(alpha) - radius*vy;
			norm = sqrt((i1x-i3x)*(i1x-i3x) + (i1y-i3y)*(i1y-i3y));
			if (radius > norm)
				{
				cx = radius*cos(alpha);
				cy = radius*sin(alpha);
				cr = radius;
				}
			else
				{
				if (norm > cr)
					{
					cx = (i1x + i3x)/2.0;
					cy = (i1y + i3y)/2.0;
					cr = norm/2.0;
					}
				}
			}
    }
	children->Delete();
	delete [] radii;

  // Update statistics, used when height of cones is calculated.
  if (radius < this->MinRadius) 
    {
    this->MinRadius = radius;
    }
  if (radius > this->MaxRadius) 
    {
    this->MaxRadius = radius;
    }
  this->SumOfRadii += radius;
  this->NrCones++;

  // For compact placement, space is allocated simply for the
  // radius of this cone plus "a little" extra.  For non-compact
  // placement, allow also for the radius allocated to the
  // largest child.
  return radius + (this->Compression ? 1 : cr);
}


// Calculate the final layout for a node, given its level in 
// the tree, and the (final) position of its parent.

void vtkConeLayout::GlobalPlacement(
  vtkIdType root, 
	vtkGraph *graph,
  double refX,					// parent's X coord
  double refY,					// parent's Y coord
  double level )				// level of this node in the tree.
{
  vtkIdType child;
	vtkIdIterator *children;

	double xFinal;
	double yFinal;
	double zFinal;

	graph->GetPosition(root, xFinal, yFinal, zFinal);
	xFinal += refX;
	yFinal += refY;
	if (this->Compression)
		{
		zFinal = level*this->Spacing;
		}
	else
		{
		zFinal = level*this->Spacing*(this->MaxRadius*this->Compactness);
		}
	graph->SetPosition(root, xFinal, yFinal, zFinal);
  
	// Having fixed the position of "root", now iterate over its
  // children and fix their positions, remembering that these
  // are one level further down the tree.

  children = graph->GetChildren(root);
  while (children->HasNext())
    {
    child = children->GetNext();
    this->GlobalPlacement(
			child,
			graph,
			xFinal, yFinal, level+1);
    }
}


void vtkConeLayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
	vtkGraph *graph  = NULL;
	
	vtkPoints *layout;
	
	vtkIdIterator *nodes, *edges, *roots;
	vtkIdType node, edge, src, dst;
	vtkIdType root;
	vtkIdType nrRoots;
	
	vtkFloatArray *size;
	
	
  vtkDebugMacro(<<"Starting Cone Layout.");

	size = vtkFloatArray::SafeDownCast(
		input->GetNodeData()->GetArray("size"));
		
	nrRoots = 0;
	roots = input->GetSources();
	while (roots->HasNext())
		{
		root = roots->GetNext();
		nrRoots++;
		}

	if (nrRoots == 0)
		{
		vtkErrorMacro(<<"Input is not a forest");
		return;
		}
		
	output->ShallowCopy(input);
	
	if (nrRoots == 1)
		{
		graph = output;
		}
	else
		{
		graph = vtkGraph::New();
		graph->SetInitialNodeSize(input->GetNumberOfNodes()+1);
		graph->SetInitialEdgeSize(input->GetNumberOfEdges()+nrRoots);
		nodes = input->GetNodes();
		while (nodes->HasNext())
			{
			node = nodes->GetNext();
			graph->CreateNode(node);
			graph->SetEdgeArraySize(node, input->GetDegree(node)+1);
			}
		nodes->Delete();
		edges = input->GetEdges();
		while (edges->HasNext())
			{
			edge = edges->GetNext();
			input->GetEdge(edge, src, dst);
			graph->CreateEdge(edge, src, dst);
			}
		edges->Delete();
		
		root = graph->CreateNode(graph->GetMaxNodeId()+1);
		roots->Initialize();
		while (roots->HasNext())
			{
			node = roots->GetNext();
			graph->CreateEdge(graph->GetMaxEdgeId()+1, root, node);
			}
		}
	roots->Delete();

	layout = vtkPoints::New();
	layout->SetNumberOfPoints(graph->GetNumberOfNodes());
	graph->SetLayout(layout);
	layout->Delete();

  this->MinRadius  = 1.0E10F;
  this->MaxRadius  = 0.0;
  this->SumOfRadii = 0.0;
  this->NrCones    = 0;
  
  // Layout is performed in two passes.  First, find a provisional location
  // for each node, via a bottom-up traversal.  Then calculate a final 
  // position for each node, by performing a top-down traversal, placing
  // the root at the origin, and then positioning each child using the
  // provisional location of the child and final location of the parent.
	//
	// NOTE: If a new root has been created, the "size" array will not
	// have an entry for root.  However, node size is only considered
	// in the local placement of LEAF nodes, and if a new root has been
	// created, it MUST therefore have more than one child.
	//
	// If this assumption changes in the future, one option is to pass
	// the size and identity of the root as additional parameters, or
	// else make a complete copy of the size array and add the value for
	// the root.

  this->LocalPlacement(
		root,
		size,
		graph);
		
	// Second pass: fix absolute node position.
  this->GlobalPlacement(
		root,
		graph,
		0.0, 0.0, 0.0);

	if (graph != output)
		{
		output->SetLayout(graph->GetLayout());
		graph->Delete();
		}
	
  vtkDebugMacro(<< "Cone Layout Done.");
}


void vtkConeLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "Compactness: " << this->Compactness << "\n";
  os << indent << "Compression: " << this->Compression << "\n";
}

