/*
 *	File: vtkRadialLayout.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkRadialLayout.h"
#include "vtkObjectFactory.h"
#include "vtkMath.h"
#include "vtkGraph.h"
#include "vtkFloatArray.h"
#include "vtkPoints.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkRadialLayout);

vtkRadialLayout::vtkRadialLayout()
{
  this->Convexity = 0;
	this->Increase = 
		(vtkRadialLayout::DefaultIncrease()*vtkRadialLayout::Delta())/100.0;
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


double vtkRadialLayout::SetWidth(vtkIdType root, vtkGraph *graph)
{
	double width;
	vtkIdIterator *children;

  if (graph->GetNumberOfChildren(root) == 0)
    {
		width = 1.0;
		this->Width->InsertValue(graph->GetNodeIndex(root), width);
		this->FullWidth->InsertValue(graph->GetNodeIndex(root), width);
    }
  else
		{
		children = graph->GetChildren(root);
		width = 0.0;
		while (children->HasNext())
			{
			width += this->SetWidth(children->GetNext(), graph);
			}
		this->Width->InsertValue(graph->GetNodeIndex(root), width);
		this->FullWidth->InsertValue(graph->GetNodeIndex(root), width);
		children->Delete();
		}
	return width;
}


// Calculate the final layout for a node, given its level in 
// the tree, and the (final) position of its parent.

void vtkRadialLayout::SetPlacement(
	vtkIdType root,
	double ro,
	double alpha1,
	double alpha2,
	int layer,
	vtkGraph *graph)
{
  vtkIdType child;
	vtkIdIterator *children;
	double s, tau, alpha, rootFullWidth, childWidth;

	double myDelta = vtkRadialLayout::Delta() + layer*this->Increase;
	double fi = (alpha1 + alpha2)/2.0;

	graph->SetPosition(root, 
		ro*sin(fi),
		ro*cos(fi),
		0.0);

	this->Angle->InsertValue(graph->GetNodeIndex(root), fi);
	this->Distance->InsertValue(graph->GetNodeIndex(root), ro);
	
	if (graph->GetNumberOfChildren(root) > 0)
		{
		tau = this->Convexity ? 2.0*acos(ro/(ro + myDelta)) : 0.0;
		rootFullWidth = this->FullWidth->GetValue(graph->GetNodeIndex(root));
		if (this->Convexity && ro != 0.0 && tau < (alpha2-alpha1))
			{
			s = tau / rootFullWidth;
			alpha = (alpha1 + alpha2 - tau)/2.0;
			}
		else 
			{
			s = (alpha2 - alpha1)/rootFullWidth;
			alpha = alpha1;
			}
		children = graph->GetChildren(root);
		while (children->HasNext())
			{
			child = children->GetNext();
			childWidth = this->Width->GetValue(graph->GetNodeIndex(child));
			this->SetPlacement(
				child, 
				ro+myDelta, 
				alpha,
				alpha + s*childWidth,
				layer+1, 
				graph);
			alpha += s*childWidth;
			}
		children->Delete();
		}
}


void vtkRadialLayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
	vtkIdType root;
	vtkIdIterator *roots;

	output->ShallowCopy(input);

	// Allocate a new set of points to hold this layout.
	output->SetLayout(vtkPoints::New());

  vtkDebugMacro(<<"Starting Radial Layout.");

	roots = input->GetSources();
	if (roots->HasNext())
		{
		root = roots->GetNext();
		}
	else
		{
		vtkErrorMacro(<<"No root could be found.\n");
		return;
		}
	roots->Delete();

  this->Width = vtkFloatArray::New();
	this->FullWidth = vtkFloatArray::New();
	this->Angle = vtkFloatArray::New();
	this->Distance = vtkFloatArray::New();

  // Layout is performed in two passes.  First, find a provisional location
  // for each node, via a bottom-up traversal.  Then calculate a final 
  // position for each node, by performing a top-down traversal, placing
  // the root at the origin, and then positioning each child using the
  // provisional location of the child and final location of the parent.

  this->SetWidth(root, output);
  this->SetPlacement(root, 0.0, 0.0, 2.0*vtkMath::Pi(), 0, output);

  this->Width->Delete();
	this->FullWidth->Delete();
	this->Angle->Delete();
	this->Distance->Delete();

  vtkDebugMacro(<< "Radial Layout Done.");
}


void vtkRadialLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

