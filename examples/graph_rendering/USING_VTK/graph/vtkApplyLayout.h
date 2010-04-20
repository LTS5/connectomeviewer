/*
 *	File: vtkApplyLayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkApplyLayout - set the layout of one graph to that of a second.
// .SECTION Description
// vtkApplyLayout copies its input graph to its output, and assigns
// to the output a layout extracted from a second input, the layout 
// source.  It is assumed that the node Ids of the input graph are
// a subset of the node Ids of the layout source graph.  
//
// .SECTION Caveats
// This filter does not handle edge bends, and will be deprecated 
// in favour of vtkUseLayout.
//
// .SECTION See Also
// vtkUseLayout vtkGraph

#ifndef __vtkApplyLayout_h
#define __vtkApplyLayout_h

#include "vtkGraphToGraphFilter.h"

class VTK_EXPORT vtkApplyLayout : public vtkGraphToGraphFilter 
{
public:
  static vtkApplyLayout *New();
  vtkTypeRevisionMacro(vtkApplyLayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
	
  //---------------------------------------------------------
	// Layout source.

	// Description:
	// Set the graph object from which the layout will be taken.
  void SetLayoutSource(vtkGraph *layout)
  { this->vtkProcessObject::SetNthInput(1, layout); };

protected:
  vtkApplyLayout();
  ~vtkApplyLayout();

  void Execute();

private:
  vtkApplyLayout(const vtkApplyLayout&) {};
  void operator=(const vtkApplyLayout&) {};
};

#endif	/* __vtkApplyLayout_h */

