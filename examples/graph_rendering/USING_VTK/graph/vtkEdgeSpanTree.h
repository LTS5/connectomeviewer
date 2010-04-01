/*
 *	File: vtkEdgeSpanTree.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkEdgeSpanTree - find a spanning tree MAXIMAL wrt an edge property.
// .SECTION Description
// vtkEdgeSpanTree produces a spanning tree T for the input graph, such
// that T is a maximal spanning tree with respect to a specified edge
// property.  The property is identified by the name of the data array
// in which it is stored; this array must be a vtkFloatArray.
//
// .SECTION See Also
// vtkGraph


#ifndef __vtkEdgeSpanTree_h
#define __vtkEdgeSpanTree_h

#include "vtkGraphToGraphFilter.h"
#include "vtkIntArray.h"

class VTK_EXPORT vtkEdgeSpanTree : public vtkGraphToGraphFilter
{
public:
  static vtkEdgeSpanTree *New();
  vtkTypeRevisionMacro(vtkEdgeSpanTree,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
	
	// Description:
	// Set the name of the property that determines cost of edges.
	vtkSetStringMacro(ArrayName);
	vtkGetStringMacro(ArrayName);

protected:
  vtkEdgeSpanTree();
  ~vtkEdgeSpanTree();
   
  void Execute();

	char *ArrayName;
  
private:
  vtkEdgeSpanTree(const vtkEdgeSpanTree&) {};
  void operator=(const vtkEdgeSpanTree&) {};
};

#endif	/* __vtkEdgeSpanTree_h */

