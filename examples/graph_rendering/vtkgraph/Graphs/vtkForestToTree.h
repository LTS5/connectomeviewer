/*
 *	File: vtkForestToTree.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkForestToTree - convert a forest into a tree
// .SECTION Description
// vtkForestToTree converts a forest into a tree by adding
// (if necessary) a new root node, connected to the roots
// of the forest.  Point data for this new node is 
// taken to be the average over data for the roots. Edge
// data for the new edges is not defined.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkForestToTree_h
#define __vtkForestToTree_h

#include "vtkGraphToGraphFilter.h"
#include "vtkGraph.h"

class VTK_EXPORT  vtkForestToTree : public vtkGraphToGraphFilter
{
public:
  static vtkForestToTree *New();
  vtkTypeRevisionMacro(vtkForestToTree,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkForestToTree();
  ~vtkForestToTree() {};

  void Execute();

private:
  vtkForestToTree(const vtkForestToTree&) {};
  void operator=(const vtkForestToTree&) {};
};

#endif	/* __vtkForestToTree_h */

