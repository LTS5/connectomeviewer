/*
 *	File: vtkRadialLayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkRadialLayout - produce a 2D radial layout of a tree
// .SECTION Description
// vtkRadialLayout implements a 2D radial layout using an
// algorithm first described in:
//
// P. Eades, Drawing Free Trees, Bull. of the Institute for
// Combinatorics and its Applications, 5, 1992.
//
// This implementation is based on a tree layout module for the
// "Latour" tool, written by I. Herman, G. Melancon and 
// M.S. Marshall from CWI Amsterdam: see 
// www.cwi.nl/ projects/InfoVisu/OldStuff/
//
// The algorithm incorporates a convexity check that aims to
// ensure that subtrees do not overlap.  In practice, it has
// been found that any overlap is minimal, and usually the
// leaving the convexity check off results in better use of
// space.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkRadialLayout_h
#define __vtkRadialLayout_h

#include "vtkGraphToGraphFilter.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkGraph.h"

class VTK_EXPORT  vtkRadialLayout : public vtkGraphToGraphFilter
{
public:
  static vtkRadialLayout *New();
  vtkTypeMacro(vtkRadialLayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Determine whether the convexity constraint should
	// be enforced (default false).
  vtkSetMacro(Convexity, int);
  vtkGetMacro(Convexity, int);
	vtkBooleanMacro(Convexity, int);

	static double Delta() { return 1.0; };
	static double DefaultIncrease() { return 80.0; };

protected:
  vtkRadialLayout();
  ~vtkRadialLayout() {};

  void Execute();

  // Description:
  // Helper operations for tree layout.  Layout is performed
  // in two traversals of the tree.  The first traversal finds
  // the position of child nodes relative to their parent. The
  // second traversal positions each node absolutely, working
  // from the initial position of the root node.

  double SetWidth(vtkIdType root, vtkGraph *graph);
            
  void SetPlacement(
          vtkIdType root, 
					double ro, 
					double alpha1, 
					double alpha2,
          int level,
					vtkGraph *graph);      // derived from level.

	// Arrays used to share information between different
	// passes of the layout algorithm.
	
  vtkFloatArray *Width;
	vtkFloatArray *FullWidth;
	vtkFloatArray *Angle;
	vtkFloatArray *Distance;
	double Increase;

  int   Convexity;        // force wedges to be convex.

private:
  vtkRadialLayout(const vtkRadialLayout&) {};
  void operator=(const vtkRadialLayout&) {};
};

#endif

