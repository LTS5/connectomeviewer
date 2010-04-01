/*
 *	File: vtkSpanLayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkSpanLayout - 3D graph layout based on spanning DAG
// .SECTION Description
// vtkSpanLayout is based on a 3D graph layout algorithm 
// developed as part of the "tulip" tool by David Auber;
// see www.tulip-software.org   
//
// It has been modified (a) to use VTK data structures,
// (b) to use pluggable spanning DAG and tree layout
// filters, (c) to use a constant number of auxiliary
// nodes when fitting non-tree edges, (d) use of a 
// parameter to determine spacing between DAG levels,
// and (e) minor changes to the implementation of the 
// edit-list.
//
// Note: the code does not currently handle loops in the
// graph; any loops (an edge a-a) in the input will not
// appear in the output.

// .SECTION See Also
// Other layout methods.

#ifndef __vtkSpanLayout_h
#define __vtkSpanLayout_h

#include "vtkGraphToGraphFilter.h"

class VTK_EXPORT vtkSpanLayout : public vtkGraphToGraphFilter 
{
public:
  static vtkSpanLayout *New();
  vtkTypeRevisionMacro(vtkSpanLayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/get the filter to be used to extract a spanning
  // DAG from the input graph.
  vtkSetObjectMacro(SpanningDAG,vtkGraphToGraphFilter);
  vtkGetObjectMacro(SpanningDAG,vtkGraphToGraphFilter);

  // Description:
  // Set the filter that is to be used in laying out the
  // underlying spanning tree derived from the DAG.
  vtkSetObjectMacro(TreeLayout,vtkGraphToGraphFilter);
  vtkGetObjectMacro(TreeLayout,vtkGraphToGraphFilter);
 
  // Description:
  // Set/get the vertical spacing between DAG levels. 
  vtkSetMacro(Spacing, float);
  vtkGetMacro(Spacing, float);
	
protected:
  vtkSpanLayout();
  ~vtkSpanLayout();

  float Spacing;	// vertical space between DAG levels
  vtkGraphToGraphFilter *TreeLayout;
  vtkGraphToGraphFilter *SpanningDAG;
	
  void Execute();

private:
  vtkSpanLayout(const vtkSpanLayout&) {};
  void operator=(const vtkSpanLayout&) {};
};

#endif	/* __vtkSpanLayout_h */

