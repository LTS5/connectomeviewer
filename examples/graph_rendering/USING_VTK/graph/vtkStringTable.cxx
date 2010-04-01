/*
 *	File: vtkStringTable.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 *
 *	Change Log:
 *	2003/03/12:	Added SetString to allow use as lookup table.
 */
 
#include "vtkStringTable.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkStringTable);
vtkCxxRevisionMacro(vtkStringTable, "1.0");

// ----------------------------------------------------
// Constructors, initialization and printing.

typedef struct _vtkStab_s *_vtkStab_ptr;
typedef char *charptr;

// Instantiate object.
vtkStringTable::vtkStringTable()
{
  this->TableSize = 0;
  this->IndexSize = 5011;
	this->NextId = 0;

	this->Index = new _vtkStab_ptr[this->IndexSize];
	this->Table = NULL;
	
  for (vtkIdType i = 0; i < this->IndexSize; i++)
		{
    this->Index[i] = NULL;
		}
}

vtkStringTable::~vtkStringTable()
{
	this->Initialize();
	delete [] this->Index;
}

void vtkStringTable::Initialize()
{
  struct _vtkStab_s *bucket;
  
  for (vtkIdType i = 0; i < this->IndexSize; i++)
		{
		while (bucket = this->Index[i]) 
			{
			this->Index[i] = bucket->next;
			delete [] this->Table[bucket->id];
			delete bucket;
			}
    }
	this->NextId = 0;
	if (this->Table)
		{
		delete [] this->Table;
		}
	this->Table = NULL;
	this->TableSize = 0;
}

void vtkStringTable::DeepCopy(vtkStringTable *src)
{
	this->Initialize();
	if (src->NextId > 0)
		{
		this->SetTableSize(src->NextId);
		this->NextId = src->NextId;
		}
  if (this->IndexSize != src->IndexSize)
    {
		this->SetIndexSize(src->IndexSize);
		}
  for (vtkIdType i = 0; i < src->NextId; i++)
    {
		if (src->Table[i])
			{
			this->Table[i] = new char[strlen(src->Table[i])+1];
			strcpy(this->Table[i], src->Table[i]);
			this->MakeIndexEntry(this->Table[i], i);
			}
		else
			{
			this->Table[i] = NULL;
			}
		}
}

void vtkStringTable::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "Table size " << this->TableSize << "\n";
  os << indent << "Index size " << this->IndexSize << "\n";
  os << indent << "NextId     " << this->NextId << "\n";
}


// ----------------------------------------------------
// Table and index manipulation.

vtkIdType vtkStringTable::AddString(char *text)
{
  vtkIdType slot = this->Hash(text);
  struct _vtkStab_s *bucket = this->Index[slot];
	
  while (bucket && strcmp(text, this->Table[bucket->id]))
		{
		bucket = bucket->next;
		} 
	if (!bucket)
		{
		if (this->NextId == this->TableSize)
			{
			this->SetTableSize(this->TableSize+1000);
			}
		this->Table[this->NextId] = new char[strlen(text)+1];
		strcpy(this->Table[this->NextId], text);
		this->MakeIndexEntry(text, this->NextId);
		return this->NextId++;
		}
	else
		{
		return bucket->id;
		}
}

void vtkStringTable::SetString(char *text, vtkIdType posn)
{
  vtkIdType slot = this->Hash(text);
  struct _vtkStab_s *bucket = this->Index[slot];
	
  while (bucket && strcmp(text, this->Table[bucket->id]))
		{
		bucket = bucket->next;
		} 
	if (bucket)
		{
		if (bucket->id == posn)
			{
			// string already present at the position	
			return;				
			}
		else
			{
			// sring present, but at a different position.
			this->RemoveString(bucket->id);
			}
		}
	if (this->TableSize <= posn)
		{
		this->SetTableSize(posn+10);
		}
	if (this->Table[posn])
		{
		// Another string present at this position; replace it.
		this->RemoveString(posn);
		}
	this->Table[posn] = new char[strlen(text)+1];
	strcpy(this->Table[posn], text);
	if (posn >= this->NextId)
		{
		this->NextId = posn+1;
		}
	this->MakeIndexEntry(text, posn);
}


void vtkStringTable::RemoveString(vtkIdType key)
{
  vtkIdType slot;
  struct _vtkStab_s *prev;
  struct _vtkStab_s *bucket;
  
	if (key >= this->NextId || !this->Table[key])
		{
		vtkErrorMacro(<<"No string at index "<<key);
		return;
		}
		
	slot = this->Hash(this->Table[key]);
	prev = NULL;
	bucket = this->Index[slot];
  while (bucket && strcmp(this->Table[key], this->Table[bucket->id]))
    {
		prev = bucket;
		bucket = bucket->next;
		}
	if (prev)
		{
		prev->next = bucket->next;
		}
	else
		{
		this->Index[slot] = bucket->next;
		}
	delete bucket;
	delete [] this->Table[key];
	this->Table[key] = NULL;
}

void vtkStringTable::SetIndexSize(vtkIdType newSize)
{
	struct _vtkStab_s **newIndex;
	struct _vtkStab_s **oldIndex;
  struct _vtkStab_s *bucket;
	vtkIdType oldSize;
	
	if (!(newIndex = new _vtkStab_ptr[newSize]))
		{
		vtkErrorMacro(<<"Could not allocate new index.");
		return;
		}
	
	oldSize  = this->IndexSize;
	oldIndex = this->Index;
	
	this->Index = newIndex;
	this->IndexSize = newSize;
	
  for (vtkIdType i = 0; i < newSize; i++)
		{
    newIndex[i] = NULL;
		}
		
	// rehash any entries in the existing table.
  for (vtkIdType j = 0; j < oldSize; j++)
		{
		while (bucket = oldIndex[j]) 
			{
			this->MakeIndexEntry(this->Table[bucket->id], bucket->id);
			oldIndex[j] = bucket->next;
			delete bucket;
			}
    }
	delete [] oldIndex;
}

vtkIdType vtkStringTable::GetIndexSize()
{
	return this->IndexSize;
}

void vtkStringTable::SetTableSize(vtkIdType newSize)
{
	char **newTable;
	
	if (!newSize || !(newTable = new charptr[newSize]))
	 {
	 vtkErrorMacro(<<"Could not allocate new string table.");
	 return;
	 }
	if (this->Table)
		{
		for (vtkIdType i = 0; i < this->NextId; i++)
	    {
		  newTable[i] = this->Table[i];
		  }
		delete [] this->Table;
		}
	for (vtkIdType j = this->NextId; j < newSize; j++)
		{
		newTable[j] = NULL;
		}
	this->Table = newTable;
	this->TableSize = newSize;
}

vtkIdType vtkStringTable::GetTableSize()
{
	return this->TableSize;
}


// ----------------------------------------------------
// Query functions.

char *vtkStringTable::GetString(vtkIdType key)
{
	return (key < this->TableSize) 
		? this->Table[key]
		: NULL;
}

vtkIdType vtkStringTable::GetKey(char *text)
{
  vtkIdType slot = this->Hash(text);
  struct _vtkStab_s *bucket = this->Index[slot];
  
  while (bucket && strcmp(text, this->Table[bucket->id]))
		{
		bucket = bucket->next;
		} 
	return (bucket != NULL) ? bucket->id : -1;
}
	
int vtkStringTable::HasString(char *text)
{
  vtkIdType slot = this->Hash(text);
  struct _vtkStab_s *bucket = this->Index[slot];
  
  while (bucket && strcmp(text, this->Table[bucket->id]))
		{
		bucket = bucket->next;
		} 
	return (bucket != NULL);
}


// ----------------------------------------------------
// Helper functions

void vtkStringTable::MakeIndexEntry(char *key, vtkIdType id)
{
  vtkIdType slot = this->Hash(key);
  struct _vtkStab_s *bucket = this->Index[slot];
  
  while (bucket && (strcmp(this->Table[bucket->id], key)))
    {
		bucket = bucket->next;
		}
  if (!bucket)
    {
		bucket = new (struct _vtkStab_s);
		bucket->next = this->Index[slot];
		this->Index[slot] = bucket;
		}
	bucket->id = id;
}

// Hashing function, following Figure 5.4 in:
//   Data Structures and Algorithm Analysis in C++,
//   Mark Allen Weiss, Addison Wesley, 1999

vtkIdType vtkStringTable::Hash(char *key)
{
  vtkIdType result = 0;

  while (*key)
		{
    result = 37*result + *key++;
		}
	result %= this->IndexSize;
	
  if (result < 0)
		{
		result += this->IndexSize;
		}
	return result;
}



