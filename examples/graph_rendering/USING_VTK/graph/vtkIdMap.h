/*
 *	File: vtkIdMap.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkIdMap - hash-table like functionality for integer key/value
// .SECTION Description
// vtkIdMap provides the functionality of a map, similar to a hash table,
// for use when both key and value are integers.  This is used in the 
// prototype as nodes and edges are identified by ints.  IdMaps are used
// to map the "external" id of a node or edge (e.g. obtained from a file)
// to the "internal" index of the node or edge within VTK data structures.
// A significantly more flexible naming scheme is under development for 
// the "production" version.
// 
// .SECTION See Also
// vtkGraph vtkIndexedSet

#ifndef __vtkIdMap_h
#define __vtkIdMap_h

#include "vtkObject.h"

class VTK_EXPORT vtkIdMap : public vtkObject 
{
public:
  static vtkIdMap *New();
  vtkTypeRevisionMacro(vtkIdMap,vtkObject); 
  void PrintSelf(ostream& os, vtkIndent indent);

	// ---------------------------------------------------
	// General management functions
	
  // Description:
  // Make a full copy of the map.
  void DeepCopy(vtkIdMap *m);
	
  // Description:
  // Return the memory in kilobytes consumed by this hashtable. Used to
  // support streaming and reading/writing data. The value returned is
  // guaranteed to be greater than or equal to the memory required to
  // actually represent the data represented by this object. The 
  // information returned is valid only after the pipeline has 
  // been updated.
  unsigned long GetActualMemorySize();
	
  // Description:
  // Return to initial state - any existing entries are purged, but
  // table space is left allocated.
  void Initialize();
  
	// Description:
	// Set/get the size of the table.  Changing the size results in
	// reallocation of the table and rehashing of entries.
	void SetIndexSize(vtkIdType nrSlots);
	vtkIdType GetIndexSize();

	
	// ---------------------------------------------------
	// Add/remove entries to/from the table.
	
  // Description:
  // Insert an entry into the map, linking "key" to "data".
  // If the key is already present in the map, the old
  // entry is overwritten by the new mapping.
  void SetEntry(vtkIdType key, vtkIdType val);

  // Description:
  // Remove the entry with the given key from the map.
  // If no entry has the key, the map is unchanged.
  void Remove(vtkIdType key);


	// ---------------------------------------------------
	// Query operations.
	
  // Description:
  // Determine if a particular key is present in the table.
  int HasKey(vtkIdType key);
  
  // Description:
  // Retrieve the entry matching the given key from the map.
  // If the key is present, the function returns non-0 and val
  // holds the corresponding value, otherwise the return result
  // is 0 and the value of 'val' is undefined.
  int GetEntry(vtkIdType key, vtkIdType& val);

  // Description:
  // Return the entry matching the given key from the map.
  // This function assumes that the given key is present
  // in the table -- the result is negative otherwise.
  vtkIdType Get(vtkIdType key);

  // Description:
  // Return the number of entries in the data.
  vtkIdType GetSize(){return this->NrEntries;}
  
	// Return the largest id stored in the table.
	vtkIdType GetMaxId();
  
protected:
  vtkIdMap();
  ~vtkIdMap();

	//BTX
  typedef struct _mapEntry 
		{
    vtkIdType Key;
    vtkIdType Value;
    struct _mapEntry *Next;
  	} 
		mapEntry, *mapEntryPtr;
	//ETX
  
  vtkIdType NrSlots;      // number of slots available in table
  vtkIdType NrSlotsUsed;  // actual number of slots used
  vtkIdType NrEntries;    // total number of entries in table.
  mapEntry **Table;   		// the actual table.
	vtkIdType MaxId;

	// Description:
	// Internal hash function: simply map the key onto the size of
	// the table using %.
  vtkIdType Hash(vtkIdType key);

private:
  vtkIdMap(const vtkIdMap&) {};
  void operator=(const vtkIdMap&) {};
};

#endif	/* __vtkIdMap_h */
