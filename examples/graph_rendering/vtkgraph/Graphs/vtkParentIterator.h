/*
 *	File: vtkParentIterator.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkParentIterator - traverse parents of a given node.
// .SECTION Description
// vtkparentIterator is an implementation of the vtkIdIterator
// interface that operates on an array of Ids that are assumed
// to be the ids of edges belonging to a vtkGraph object.  Each 
// value returned by the iterator is obtained by asking the
// graph for the "source" node id of the corresponding edge id.
//
// .SECTION See Also
// vtkIdIterator vtkGraph vtkIdArrayIterator vtkChildIterator

#ifndef __vtkParentIterator_h
#define __vtkParentIterator_h

#include "vtkIdArrayIterator.h"
#include "vtkGraph.h"

class VTK_EXPORT vtkParentIterator : public vtkIdArrayIterator
{
public:
  static vtkParentIterator *New();
  vtkTypeRevisionMacro(vtkParentIterator,vtkIdArrayIterator);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

	// ---------------------------------------------------
	// Re-implemented traversal functions
	
	// Description:
	// Return the next child node id. 
	vtkIdType GetNext();
	
	// ---------------------------------------------------
	// Traversal parameter set-up.
	
	// Description:
	// Set the graph to which the node/children belong.
	vtkSetObjectMacro(Graph, vtkGraph);
	
protected:
  vtkParentIterator();
  ~vtkParentIterator();

	vtkGraph *Graph;
	
private:
  vtkParentIterator(const vtkParentIterator&);	// Not implemented.
  void operator=(const vtkParentIterator&);			// Not implemented.
};

#endif /* __vtkParentIterator_h */
