/*
 *	File: vtkIdListIterator.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkIdListIterator - iterate over ids held in a list.
// .SECTION Description
// vtkIdListIterator is an implementation of the vtkIdIterator
// interface that implements the collection of ids to be
// traversed as a list, specifically, a vtkIdList object.
//
// .SECTION See Also
// vtkIdIterator vtkIdList

#ifndef __vtkIdListIterator_h
#define __vtkIdListIterator_h

#include "vtkIdIterator.h"
#include "vtkIdList.h"

class VTK_EXPORT vtkIdListIterator : public vtkIdIterator
{
public:
  static vtkIdListIterator *New();
  vtkTypeMacro(vtkIdListIterator,vtkIdIterator);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

	// ---------------------------------------------------
	// Re-implemented traversal functions
	
	// Description:
	// Initialize, ready for start of traversal.  This can 
	// be called at any time to reset the traversal to the
	// beginning.
	void Initialize();
	
	// Description:
	// Return 1 if and only if the iterator can return
	// another id via GetNext().
	int HasNext();
	
	// Description:
	// Return the next Id from the traversal, and advance
	// the current position within the collection. 
	vtkIdType GetNext();
	
	// ---------------------------------------------------
	// Traversal parameter set-up.
	
	// Description:
	// Set the list of ids to be iterated over.
	vtkSetObjectMacro(List, vtkIdList);
	
protected:
  vtkIdListIterator();
  ~vtkIdListIterator();

	vtkIdList *List;
	vtkIdType Position; 	// current iterator position
	
private:
  vtkIdListIterator(const vtkIdListIterator&);	// Not implemented.
  void operator=(const vtkIdListIterator&);			// Not implemented.
};

#endif /* __vtkIdListIterator_h */
 
