/*
 *	File: vtkUseLayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkUseLayout - layout one graph using the layout of a second.
// .SECTION Description
// vtkUseLayout assigns nodes in its input graph to positions 
// associated with the nodes of a second (layout) graph.  Matching
// of nodes is based on nodeID.  Nodes in the input graph that are 
// not found in the layout graph are not copied to the output, nor
// are their incident edges.  Note that the filter vtkGraphOperations
// can be used to compute the union/difference of graphs.  The filter 
// also takes account of edge bends: if an edge from src-dst in the
// input exists in the layout, and the layout graph provides a
// path for an edge from src-dst, that path is copied. 
// 
// Note 1: all bend points are copied from the layout graph, not 
// just those used in edges that occur in the input graph.
// Note 2: matching of edges between layout and input is done on
// the basis of source and target nodes, not on the basis of
// edge Id.  If the layout contains multiple edges from src to
// dst, the edge whose path is taken for the layout will be
// whichever edge is foudn first within the graph structure.
// Note 3: the filter vtkApplyLayout is similar to this, but
// does not handle bends.  It will be deprecated in time.
//
// .SECTION See Also
// vtkGraph vtkGraphOperations vtkApplyLayout

#ifndef __vtkUseLayout_h
#define __vtkUseLayout_h

#include "vtkGraphToGraphFilter.h"

class VTK_EXPORT vtkUseLayout : public vtkGraphToGraphFilter 
{
public:
  static vtkUseLayout *New();
  vtkTypeRevisionMacro(vtkUseLayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
	
  //---------------------------------------------------------
	// Layout source.

	// Description:
	// Set the graph object from which the layout will be taken.
  void SetLayoutSource(vtkGraph *layout)
  { this->vtkProcessObject::SetNthInput(1, layout); };

protected:
  vtkUseLayout();
  ~vtkUseLayout();

  void Execute();

private:
  vtkUseLayout(const vtkUseLayout&) {};
  void operator=(const vtkUseLayout&) {};
};

#endif	/* __vtkUseLayout_h */

