/*
 *	File: vtkIdMap.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkIdMap.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkIdMap);
vtkCxxRevisionMacro(vtkIdMap, "1.0");


//-----------------------------------------------

vtkIdMap::vtkIdMap()
{
	this->MaxId = -1;
  this->NrSlots = 0;
  this->NrSlotsUsed = 0;
  this->NrEntries   = 0;
	this->Table = NULL;
	
	this->SetIndexSize(60111);	// arbitrary
}

vtkIdMap::~vtkIdMap()
{
  if (this->Table)
    {
		this->Initialize();
    delete [] this->Table;
		}
}

void vtkIdMap::SetIndexSize(vtkIdType nrSlots)
{
	mapEntry **newTable, **oldTable;
  mapEntry *bucket;
	vtkIdType oldNrSlots;
	
	if (!(newTable = new mapEntryPtr[nrSlots]))
		{
		vtkErrorMacro(<<"Could not allocate new IdMap table.");
		return;
		}
	
	oldTable = this->Table;
	oldNrSlots = this->NrSlots;
	
	this->Table = newTable;
	this->NrSlots = nrSlots;
  for (vtkIdType i = 0; i < this->NrSlots; i++)
		{
    this->Table[i] = NULL;
		}
		
	// Rehash any entries in the existing table.
  for (vtkIdType j = 0; j < oldNrSlots; j++)
		{
		while (bucket = oldTable[j]) 
			{
			this->SetEntry(bucket->Key, bucket->Value);
			oldTable[j] = bucket->Next;
			delete bucket;
			}
    }
	if (oldTable)
		{
		delete [] oldTable;
		}
}

vtkIdType vtkIdMap::GetIndexSize()
{
	return this->NrSlots;
}

void vtkIdMap::Initialize()
{
  mapEntry *bucket;
  
  if (!(this->Table))
		{
		return;
		}
  for (vtkIdType i = 0; i < this->NrSlots; i++)
		{
		while (bucket = this->Table[i]) 
			{
			this->Table[i] = bucket->Next;
			delete bucket;
			}
    }
	this->MaxId = -1;
}

void vtkIdMap::DeepCopy(vtkIdMap *m)
{
  mapEntry **bucket, *entry;
  
  if (this->NrSlots != m->NrSlots)
    {
		this->Initialize();
		delete [] this->Table;
		this->NrSlots = m->NrSlots;
		this->Table = new mapEntryPtr[this->NrSlots];
		}
  for (vtkIdType i = 0; i < this->NrSlots; i++)
    {
		bucket = Table+i;
		entry = m->Table[i];
		while (entry != NULL)
			{
			*bucket = new mapEntry;
			(*bucket)->Key = entry->Key;
			(*bucket)->Value = entry->Value;
			bucket = &((*bucket)->Next);
			entry = entry->Next;
			}
		*bucket = NULL;
		}
  this->NrSlotsUsed = m->NrSlotsUsed;
  this->NrEntries = m->NrEntries;
	
	this->MaxId = m->MaxId;
}


void vtkIdMap::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkIndent innext = indent.GetNextIndent();
  mapEntry *bucket;
  
  if (this->Table)
    {
    os << indent << "NrSlots: "     << this->NrSlots << "\n";
    os << indent << "NrSlotsUsed: " << this->NrSlotsUsed << "\n";
    os << indent << "NrEntries: "   << this->NrEntries << "\n";
    os << indent << "Table: "       << "\n";
    for (vtkIdType i = 0; i < NrSlots; i++)
			{
			bucket = this->Table[i];
			while (bucket)
				{
				os << innext << (bucket->Key)   << ": " 
		                 << (bucket->Value) << "\n";
				bucket = bucket->Next;
				}
			}
    }
  else
    {
    os << indent << "Table: (null)\n";
    }
}

unsigned long vtkIdMap::GetActualMemorySize()
{
  unsigned long size 
    = this->NrSlots * sizeof(void *)
    + (this->NrEntries - this->NrSlotsUsed)*sizeof(mapEntry);
  return (unsigned long) ceil((float)size/1000.0); //kilobytes
}

void vtkIdMap::SetEntry(vtkIdType key, vtkIdType data)
{
  vtkIdType slot = Hash(key);
  mapEntry *prev   = NULL;
  mapEntry *bucket = this->Table[slot];
  
  while (bucket && (key != bucket->Key))
    {
		prev = bucket;
		bucket = bucket->Next;
		}
  if (!bucket)
    {
		bucket = new mapEntry;
		bucket->Key = key;
		bucket->Next = this->Table[slot];
		if (this->Table[slot] == NULL) 
			{
			NrSlotsUsed++;
			}
		this->Table[slot] = bucket;
		}
  bucket->Value = data;
  NrEntries++;
	if (key > this->MaxId)
		{
		this->MaxId = key;
		}
}

vtkIdType vtkIdMap::GetEntry(vtkIdType key, vtkIdType& val)
{
  vtkIdType slot = Hash(key);
  mapEntry *bucket = this->Table[slot];
  
  while (bucket && (key != bucket->Key))
		{
		bucket = bucket->Next;
		}
  if (bucket)
    {
    val = bucket->Value;
		return 1;
		}
  else
		{
    return 0;
		}
}

vtkIdType vtkIdMap::Get(vtkIdType key)
{
  vtkIdType slot = Hash(key);
  mapEntry *bucket = this->Table[slot];
  
  while (bucket && (key != bucket->Key))
		{
		bucket = bucket->Next;
		}
  if (bucket)
    {
		return bucket->Value;
		}
  else
		{
    return -1;
		}
}

int vtkIdMap::HasKey(vtkIdType key)
{
  vtkIdType notUsed;
  return GetEntry(key, notUsed);
}

void vtkIdMap::Remove(vtkIdType key)
{
  vtkIdType slot = Hash(key);
  mapEntry *prev = NULL;
  mapEntry *bucket = this->Table[slot];
  
  while (bucket && (key != bucket->Key))
    {
		prev = bucket;
		bucket = bucket->Next;
		}
  if (bucket)
    {
		if (prev)
			{
			prev->Next = bucket->Next;
			}
		else
			{
			this->Table[slot] = bucket->Next;
			}
		if (this->Table == NULL) 
			{
			NrSlotsUsed--;
			}
		NrEntries--;
		delete bucket;
		}
}

vtkIdType vtkIdMap::GetMaxId()
{
	return this->MaxId;
}

// Hash simply by taking the modulus of table size....

inline vtkIdType vtkIdMap::Hash(vtkIdType key)
{
  return (key % this->NrSlots);  
}


