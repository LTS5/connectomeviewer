/*
 *	File: vtkIdIterator.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkIdIterator - interface for traversing collection of ids.
// .SECTION Description
// vtkIdIterator is an abstract class that defines an interface 
// for traversing a collection of ids, values of type vtkIdType.
// How the collection is provided is determined within various
// subclasses.
//
// .SECTION See Also
// vtkIdArrayIterator vtkIdListIterator vtkIdParentIterator
// vtkIdChildIterator vtkGraph

#ifndef __vtkIdIterator_h
#define __vtkIdIterator_h

#include "vtkObject.h"

class VTK_EXPORT vtkIdIterator : public vtkObject
{
public:
	static vtkIdIterator *New();
  vtkTypeMacro(vtkIdIterator,vtkObject);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  //---------------------------------------------------------
	// Traversal-related functions.
	
	// Description:
	// Initialize, ready for start of traversal.  This can 
	// be called at any time to reset the traversal to the
	// beginning.
	virtual void Initialize();
	
	// Description:
	// Return 1 if and only if the iterator can return
	// another id via GetNext().
	virtual int HasNext();
	
	// Description:
	// Return the next Id from the traversal, and advance
	// the current position within the collection. 
	virtual vtkIdType GetNext();
		
protected:
  vtkIdIterator() {};
  ~vtkIdIterator() {};
	
private:
  vtkIdIterator(const vtkIdIterator&);	// Not implemented.
  void operator=(const vtkIdIterator&);		// Not implemented.
};

#endif /* __vtkIdIterator_h */
 
