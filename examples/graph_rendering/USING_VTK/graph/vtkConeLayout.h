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
 */
// .NAME vtkConeLayout - produce a cone-tree layout for a forest
// .SECTION Description
// vtkConeLayout positions the nodes of a tree(forest) in 3D 
// space based on the cone-tree approach first described by 
// Robertson, Mackinlay and Card in Proc. CHI'91.  This
// implementation incorporates refinements to the layout 
// developed by Carriere and Kazman, and by Auber.
//
// The input graph must be a forest (i.e. a set of trees, or a 
// single tree); in the case of a forest, the input will be
// converted to a single tree by introducing a new root node, 
// and connecting each root in the input forest to the meta-root. 
// The tree is then laid out, after which the meta-root is removed.  
//
// The cones are positioned so that children lie in planes parallel
// to the X-Y plane, with the axis of cones parallel to Z, and
// with Z coordinate increasing with distance of nodes from the root.
//
// This layout filter was originally built from a module developed 
// by D. Auber as part of the Tulip graph visualization tool; see
// www.tulip-software.org for details. The overall algorithm 
// is similar, although much of the code has been rewritten to 
// work with the vtkGraph library, and incorporates the refinements
// mentioned above.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkConeLayout_h
#define __vtkConeLayout_h

#include "vtkGraphToGraphFilter.h"
#include "vtkFloatArray.h"
#include "vtkGraph.h"

class VTK_EXPORT  vtkConeLayout : public vtkGraphToGraphFilter
{
public:
  static vtkConeLayout *New();
  vtkTypeRevisionMacro(vtkConeLayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Determine the compactness, the ratio between the
  // average width of a cone in the tree, and the 
  // height of the cone.  The default setting is 0.75
  // which (empirically) seems reasonable, but this
  // will need adapting depending on the data.
  vtkSetMacro(Compactness, float);
  vtkGetMacro(Compactness, float);

  // Description:
  // Determine if layout should be compressed, i.e. the 
  // layout puts children closer together, possibly allowing
  // sub-trees to overlap.  This is useful if the tree is 
  // actually the spanning tree of a graph.  For "real" trees, 
  // non-compressed layout is best, and is the default.
  vtkSetMacro(Compression, int);
  vtkGetMacro(Compression, int);
  vtkBooleanMacro(Compression, int);

	// Description:
	// Set the spacing parameter that affects space between
	// layers of the tree.  If compression is on, Spacing is the
	// actual distance between layers.  If compression is off, 
	// actual distance also includes a factor of the compactness
	// and maximum cone radius.
  vtkSetMacro(Spacing, float);
  vtkGetMacro(Spacing, float);
	
  // Description:
  // Access various statistics accumulated
  // during the tree layout process.
  vtkGetMacro(NrCones,int);
  vtkGetMacro(SumOfRadii,float);
  vtkGetMacro(MinRadius,float);
  vtkGetMacro(MaxRadius,float);
  

protected:
  vtkConeLayout();
  ~vtkConeLayout() {};

  void Execute();

  // Description:
  // Helper operations for tree layout.  Layout is performed
  // in two traversals of the tree.  The first traversal finds
  // the position of child nodes relative to their parent. The
  // second traversal positions each node absolutely, working
  // from the initial position of the root node.

  double LocalPlacement(
            vtkIdType root, 
						vtkFloatArray *size,
						vtkGraph *graph);
            
  void GlobalPlacement(
            vtkIdType root, 
						vtkGraph *graph,
            double refX,         // absolute x-y coordinate of
            double refY,         // parent node; z coordinate
            double level );      // derived from level.

  float Compactness;            // factor used in mapping layer to Z
  int   Compression;            // force a compact layout?
  double MinRadius;
  double MaxRadius;
  int   NrCones;
  double SumOfRadii;
  float Spacing;		// Scale vertical spacing of cones.
  
private:
  vtkConeLayout(const vtkConeLayout&) {};
  void operator=(const vtkConeLayout&) {};
};

#endif

