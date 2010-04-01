/*
 *	File: vtkIdArrayIterator.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkIdArrayIterator - iterate over ids held in an array.
// .SECTION Description
// vtkIdArrayIterator is an implementation of the vtkIdIterator
// interface that implements the collection of ids to be
// traversed as an array, with the range of iteration bounded
// by a start and end position.
//
// .SECTION See Also
// vtkIdIterator vtkChildIterator vtkParentIterator

#ifndef __vtkIdArrayIterator_h
#define __vtkIdArrayIterator_h

#include "vtkObject.h"
#include "vtkBitArray.h"
#include "vtkIdIterator.h"

class VTK_EXPORT vtkIdArrayIterator : public vtkIdIterator
{
public:
  static vtkIdArrayIterator *New();
  vtkTypeMacro(vtkIdArrayIterator,vtkIdIterator);
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
	// Set the array on which to traverse.
	void SetArray(vtkIdType *array);
	
	// Description:
	// Set the start position; the element at this position
	// in the array is the first element returned.
	void SetStart(vtkIdType pos);
	
	// Description:
	// Set the end position; this is the position immediately
	// following the last element to be returned.
	void SetEnd(vtkIdType pos);
	
protected:
  vtkIdArrayIterator();
  ~vtkIdArrayIterator();

	vtkIdType *Array;
	vtkIdType StartPos;
	vtkIdType EndPos;
	vtkIdType Position; 	// current position in traversal
	
private:
  vtkIdArrayIterator(const vtkIdArrayIterator&);	// Not implemented.
  void operator=(const vtkIdArrayIterator&);			// Not implemented.
};

#endif /* __vtkIdArrayIterator_h */
 
