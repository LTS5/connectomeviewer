/*
 *	File: vtkIndexedSet.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkIndexedSet - template class for indexed collection
// .SECTION Description
// vtkIndexedSet is a templated class implementing an indexed
// collection of values (the type of value is the template
// parameter).  Elements in the collection are identifed by a
// value of vtkIdType.  As the Ids can be arbitrary values from
// vtkIdType, for efficient storage instances of vtkIndexedSet
// maintain an internal mapping from the Id of an element to 
// the index at which it is stored in the collection.  The class
// manages the mapping of IDs to indices to ensure that the 
// indices form a contiguous sequence of values starting from 0.
// The mapping from ID to index is implemented via a lookup table,
// vtkIdMap, taking vtkIdTypes (IDs) to vtkIdTypes (indices). The
// inverse mapping is via an array (a vtkIdTypeArray).
//
// The class was designed to support the node/edge data storage
// of vtkGraph, but may have other uses.  Note that the 
// implementation is in vtkIndexedSet.txx (not .cxx).
//
// .SECTION See Also
//	vtkGraph vtkIdMap vtkIdIterator

#ifndef __vtkIndexedSet_h
#define __vtkIndexedSet_h

#include "vtkObject.h"
#include "vtkIdTypeArray.h"
#include "vtkIdIterator.h"
#include "vtkIdMap.h"


struct _vtkNode_s {
	vtkIdType numEdges;
	vtkIdType numEdgesIn;
	vtkIdType arraySize;
	vtkIdType *edges;
};

// EDGES: the source and target nodes are recorded.  There is
// also provision for recording a sequence of point-ids, used
// if the edge has a number of bends.  CURRENTLY THIS 
// FACILITY IS NOT SUPPORTED.

struct _vtkEdge_s
{
	vtkIdType source;
	vtkIdType target;
  vtkIdType arraySize;
	vtkIdType pathLength;
  vtkIdType *path;
};

template<class T>
class VTK_EXPORT vtkIndexedSet : public vtkObject
{
public:
  static vtkIndexedSet *New();
	//vtkTypeRevisionMacro(vtkGraph, vtkDataObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  //---------------------------------------------------------
	// House-keeping methods.

  // Description:
  // Release all data and reset graph properties.
  void Initialize();

  // Description:
  // Copy the structure, duplicating all stored components.
  void DeepCopy(vtkIndexedSet<T> *src);

	// Description:
	// Set the table size to be used by the internal lookup
	// table mapping Ids to indices.
	void SetIndexSize(vtkIdType size);
	
	// Description:
	// Set/get the initial number of entries allocated for the
	// collection (this will also be the number of entries in 
	// the IndexToId map.  Initial allocation of a large
	// chunk will save reallocation and copying for applications
	// that create a large collection.  Provision of an initial
	// size does not prevent dynamic growth of the collection 
	// if necessary.
	// NOTE: These should be defined using vtkSet/GetMacros, but
	// for some reason this caused a problem with MSVC 6.0.
	void SetInitialSize(vtkIdType size) { this->InitialSize = size; };
	vtkIdType GetInitialSize() { return this->InitialSize; };
	
	// Description:
	// Where possible, reallocate space to the minimum required
	// to store the collection.
	void Squeeze();
	
  //---------------------------------------------------------
	// Creation and deletion of entries.

  // Description:
  // Create a new entry in the collection, using the next
	// available Id value.  The Id is returned.
  vtkIdType Create();
  
  // Description:
  // Create an entry in the collection using the given Id
	// value to identify it.  If the Id is already in use a
	// warning is issued: use Contains(id) to first test if
	// an Id has been allocated.
  void Create(vtkIdType id);
  
  // Description:
	// Delete the element specified by the id.  If deleting
	// the element created a hole in the array, the last item
	// of the array is moved into the hole, and the function
	// returns "1".  The internal mappings between Id and 
	// index are updated, and the old index of the relocated 
	// item is returned, in case the caller has used this index 
	// to associate information with the element in other 
	// structures.  If no remapping takes place, 0 is returned.
  int Delete(vtkIdType id, vtkIdType &movedIx);
    
  //---------------------------------------------------------
	// Query operations.

	// Description:
	// Return a specific element, either by giving and id, or
	// by providing the index directly.
	T *GetElementWithId(vtkIdType id);
	T *GetElementAtIndex(vtkIdType ix);

  // Description:
  // Return the internal index of at which the element with
	// the given id is located.
  vtkIdType IndexOfId(vtkIdType id);
	
  // Description:
  // Return 1 if the given id is in use for an element of the
	// collection, otherwise return 0.
  int Contains(vtkIdType id);
  
  // Description:
  // Return the number of elements in the collection.
  vtkIdType GetNumberOfElements();
  
	// Description:
	// Return an iterator over the Ids in the collection. Note
	// that the iterator is not thread-safe.
  vtkIdIterator *GetIds();
  
protected:
  vtkIndexedSet();
  ~vtkIndexedSet();

  T *Array;										// the collection
	vtkIdType NumberOfElements; // actual nr of elements
	vtkIdType ArraySize;				// allocated size of array

	vtkIdType	FirstFreeId;			// next id to allocate
	vtkIdTypeArray *IndexToId;	// map element id to index
	vtkIdMap *IdToIndex;				// map element index to id
	vtkIdType InitialSize;			// nr of entries to allow for initially.

	// Description:
	// Return the next free index, resizing internal structures
	// if necessary.
	vtkIdType AllocateNextIndex();
	
	// Description:
	// Resize the collection array.
	void Resize(vtkIdType size);
	
	//BTX
	friend class vtkGraph;
	//ETX

private:
  vtkIndexedSet(const vtkIndexedSet&) {};
  void operator=(const vtkIndexedSet&) {};
};

// #include "vtkIndexedSet.txx"

#endif	/* __vtkIndexedSet_h */

