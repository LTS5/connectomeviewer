/*
 *	File: vtkReingoldTilfordLayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkConeTree - 2D Reingold-Tilford layout of a tree
// .SECTION Description
// vtkReingoldTilfordLayout uses the Reingold-Tilford layout
// algorithm with modifications due to Walker, to lay out a
// tree in 2 dimensions.  See the following papers for details:
//
// E.M. Reingold and J.S. Tilford, Tidier Drawings of Trees, 
// IEEE Trans on Soft.Eng. SE-7(2), March 1981.
//
// J.Q. Walker II, A Node-positioning Algorithm for General
// Trees, Software-Practice and Experience, 20(7), July 1990.
//
// This implementation is based on a tree layout module for
// the "Latour" tool, written by I. Herman, G. Melancon and 
// M.S. Marshall from CWI Amsterdam: see 
// www.cwi.nl/ projects/InfoVisu/OldStuff/
//
// The algorithm does not check that its input is a tree
// rather than an arbitrary graph.  This is for the client
// to ensure.  It will however handle the case where the
// input is a forest (i.e. a set of trees), by using an
// extra root node to produce the layout, and then 
// discarding the root.
//
// .SECTION See Also
// vtkGraph vtkRadialLayout

#ifndef __vtkReingoldTilfordLayout_h
#define __vtkReingoldTilfordLayout_h

#include "vtkGraphToGraphFilter.h"


class VTK_EXPORT  vtkReingoldTilfordLayout : public vtkGraphToGraphFilter 
{
public:
  static vtkReingoldTilfordLayout *New();
  vtkTypeRevisionMacro(vtkReingoldTilfordLayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
	
	// ---------------------------------------------------------
	// Filter parameters
	
	// Description:
	// Set/get the distance between layers.
  vtkSetMacro(Spacing, float);
  vtkGetMacro(Spacing, float);
	
protected:
  vtkReingoldTilfordLayout();
  ~vtkReingoldTilfordLayout() {};

  void Execute();

  float Spacing;
	
	// Parameters shared between functions. 	
	double *Xpreliminary;			// preliminary x-coord of nodes
	double *Xmodifier; 				// x-coordinate modifier
	double *Size;							// size of each node
	vtkIdType *LeftBrother; 	// node to the left of a given node
	vtkIdType *Layer; 				// the depth of a node in the tree
	vtkIdType *Contour; 			// rightmost node at each layer

	// Internal functions used to implement the layout
	
	// Initialize internal parameters, in particular find the
	// left brother of each node in the tree.
  void Initialize(
		vtkIdType root, 
		vtkIdType layer, 
		vtkGraph *tree);

	// Report the x-coord of root, taking account of parent modifiers.
  double TrueX(
		vtkIdType root, 
		vtkGraph *tree);

	// Find preliminary coordinate for tree at root.
  void FirstWalk(
		vtkIdType root, 
		vtkGraph *tree);
		
	// Find final position for tree at root. 
  void SecondWalk(
		vtkIdType root,
		double modifier,
		vtkPoints *layout,
		vtkGraph *tree); 

private:
  vtkReingoldTilfordLayout(const vtkReingoldTilfordLayout&) {};
  void operator=(const vtkReingoldTilfordLayout&) {};
};

#endif

