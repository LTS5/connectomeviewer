/*
 *	File: vtkStringTable.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkStringTable - lookup table for strings
// .SECTION Description
// vtkStringTable implements a table for storing strings and 
// accessing strings using a key (a value of type vtkIdType).
// This key can then be stored in a vtkIdType array and passed 
// through the visualization pipeline within field/point/cell 
// data.  In this way arbitrary strings to be associated with 
// structures within a dataset.  Implementation of this type
// was motivated by the need to label nodes and edges within a
// graph, but it may also be useful for other datasets, e.g.
// tagging objects (atoms, groups, chains etc) at various 
// levels of detail within a molecule.  See the graph library
// classes vtkGMLReader and vtkGMLWriter for examples of use.
//
// .SECTION See Also
// vtkXGMLReader vtkXGMLWriter

#ifndef __vtkStringTable_h
#define __vtkStringTable_h

#include "vtkObject.h"

//BTX
// Description:
// The string table is implemented as an array of strings; the
// index of a string is its key.  For fast lookup of a given
// string, the table uses a hashtable; the key and values are
// combined in that a hashtable entry is an index into the 
// string table where a string with that hashvalue is stored.
struct _vtkStab_s 
	{
  vtkIdType id;
  struct _vtkStab_s *next;
  };
//ETX

class VTK_EXPORT vtkStringTable : public vtkObject 
{
public:
  static vtkStringTable *New();
  vtkTypeRevisionMacro(vtkStringTable,vtkObject); 
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Make a full copy of the map.
  void DeepCopy(vtkStringTable *src);

  // Description:
  // Return to initial state - any existing entries are purged, 
	// but table space is left allocated.
  void Initialize();

	// ---------------------------------------------------------
	// Modify the string table and index
  
  // Description:
	// Add the given string to the table and return a key
	// that can be used to retrieve the string efficiently.
	// If the string already exists in the table, the key 
	// of the existing string is returned.
  vtkIdType AddString(char *text);

  // Description:
	// Place the given string in the table at the position
	// indicated. If the string already exists in the table, 
	// it is relocated to the position.  Any existing
	// string at that position is removed.
  void SetString(char *text, vtkIdType posn);

	// Description:
	// Remove the string with the specified key from the table.
	void RemoveString(vtkIdType key);
	
	// Set/get the size of the table.  Changing the size 
	// results in reallocation of the table; the index
	// is not modified.
	void SetTableSize(vtkIdType newSize);
	vtkIdType GetTableSize();
	
	// Description:
	// Set/get the size of the index.  Changing the size 
	// results in reallocation of the index, and rehashing.
	void SetIndexSize(vtkIdType newSize);
	vtkIdType GetIndexSize();

	// ---------------------------------------------------------
	// Table query functions
  
	// Description:
	// Return the string identified by the key.  The return
	// value is a pointer to the stored string, this should
	// be modified, else the hash table will be corrupted.
 	char *GetString(vtkIdType key);
	
	// Description:
	// Return the index to which the given string is assigned.
	// If the string is not present, the result is not 
	// meaningful: use HasString to determine that the string
	// is present.
 	vtkIdType GetKey(char *text);

  // Description:
	// Return 1 if and only if the table contains the given
	// string, otherwise return 0.
  int HasString(char *text);
  
	// Description:
	// Return the next available Id.  Useful for clients that
	// need to allocate a table indexed by label id.
	vtkGetMacro(NextId, vtkIdType);
	
protected:
  vtkStringTable();
  ~vtkStringTable();

	//BTX
	struct _vtkStab_s **Index;	// hashtable of strings 
	char **Table; 							// map key (index) to string
	//ETX
	
  vtkIdType Hash(char *key);
	void MakeIndexEntry(char *key, vtkIdType id);
	
	vtkIdType TableSize;	// number of slots available in table
	vtkIdType NextId;			// id to allocate to next new string
  vtkIdType IndexSize;	// number of slots available in index

private:
  vtkStringTable(const vtkStringTable&) {};
  void operator=(const vtkStringTable&) {};
};

#endif /* __vtkStringTable_h */

