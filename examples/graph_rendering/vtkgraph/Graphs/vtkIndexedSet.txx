/*
 *	File: vtkIndexedSet.txx
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
 *	2003/10/30: template reorganization from John Shalf 
 *	            to allow porting to Irix and other systems.
 */
 
#include "vtkIndexedSet.h"
#include "vtkObjectFactory.h"
#include "vtkSystemIncludes.h"
#include "vtkIdArrayIterator.h"

#define INIT_ARRAY_SIZE	40000
#define RESIZE_SCALE 1.2

template<class T>
vtkIndexedSet<T> *vtkIndexedSet<T>::New()
{
	return new vtkIndexedSet<T>();
}

//---------------------------------------------------------

template<class T>
vtkIndexedSet<T>::vtkIndexedSet()
{
	this->Array = NULL;
	this->ArraySize = 0;
	this->NumberOfElements = 0;
	this->InitialSize = INIT_ARRAY_SIZE;

	this->IndexToId = vtkIdTypeArray::New();
	this->IdToIndex = vtkIdMap::New();
	
	this->FirstFreeId = 0;
}

template<class  T>
vtkIndexedSet<T>::~vtkIndexedSet()
{	
	this->Initialize();
	this->IndexToId->UnRegister(this);
	this->IdToIndex->UnRegister(this);
}

// Print method is null due to difficulties in compiling using
// MSVC, still to be resolved.
template<class  T>
void vtkIndexedSet<T>::PrintSelf(ostream& os, vtkIndent indent)
{
//	os << indent << "vtkIndexedSet:\n";
}

template<class  T>
void vtkIndexedSet<T>::Initialize()
{
	if (this->Array)
		{
		delete [] this->Array;
		this->Array = NULL;
		}
	this->NumberOfElements = 0;
	this->ArraySize = 0;

	this->IndexToId->Initialize();
	this->IdToIndex->Initialize();
	
	this->FirstFreeId = 0;
}

template<class  T>
void vtkIndexedSet<T>::DeepCopy(vtkIndexedSet<T> *src)
{
	if (src) 
		{
		this->Initialize();

		// Copy local data
		if (src->NumberOfElements > 0)
			{
			this->Resize(src->NumberOfElements);
  		for (vtkIdType i = 0; i < src->NumberOfElements; i++)
    		{
				this->Array[i] = src->Array[i];
				// NB: if T contains pointer structures, it is up
				// to the user of this class to ensure that any
				// sub-structures are deep-copied.
				}
			}
		this->NumberOfElements = src->NumberOfElements;
		this->ArraySize = src->NumberOfElements;

		// Deep copy internal data structures.
		this->IndexToId->DeepCopy(src->IndexToId);
		this->IdToIndex->DeepCopy(src->IdToIndex);
		this->FirstFreeId = src->FirstFreeId;
		}
}

template<class  T>
void vtkIndexedSet<T>::Resize(vtkIdType size)
{
	T *array;

	// Don't allow resizing below what we are committed
	// to store.
	if (size >= this->NumberOfElements)
		{
		array = NULL;
		if (size > 0 && !(array = new T[size]))
			{
			vtkErrorMacro(<<"Could not allocate new array.");
			return;
			}
		for (vtkIdType i = 0; i < this->NumberOfElements; i++)
			{
			array[i] = this->Array[i];
			}
		if (this->Array)
			{
			delete [] this->Array;
			}
		this->Array = array;
		this->ArraySize = size;
		}
	this->IndexToId->Resize(size);
}

template<class  T>
void vtkIndexedSet<T>::SetIndexSize(vtkIdType size)
{
	this->IdToIndex->SetIndexSize(size);
}

template<class  T>
vtkIdType vtkIndexedSet<T>::AllocateNextIndex()
{
	vtkIdType index = this->NumberOfElements;

	if (this->ArraySize == index)
		{
		if (!this->Array)
			{
			this->Resize(this->InitialSize);
			}
		else
			{
			this->Resize((vtkIdType)(this->ArraySize * RESIZE_SCALE));
			}
		}
	this->NumberOfElements++;
	return index;
}

template<class  T>
void vtkIndexedSet<T>::Squeeze()
{
	this->Resize(this->NumberOfElements);
}

//---------------------------------------------------------

template<class  T>
vtkIdType vtkIndexedSet<T>::Create()
{
	this->Create(this->FirstFreeId);
	return this->FirstFreeId++;
}

template<class  T>
void vtkIndexedSet<T>::Create(vtkIdType id)
{
	vtkIdType ix;

	if (!this->IdToIndex->HasKey(id))
		{
		// Id is free.
		ix = this->AllocateNextIndex();
		this->IdToIndex->SetEntry(id, ix);
		this->IndexToId->InsertValue(ix, id);
		}
	else
		{
		// id is NOT free: issue a warning.
		vtkWarningMacro(<<id<<" is already in use.");
		}	
	if (id > this->FirstFreeId)
		{
		this->FirstFreeId = id+1;
		}
}

// NB: Caller is responsible for removing any components 
// of elements.
template<class  T>
int vtkIndexedSet<T>::Delete(vtkIdType id, vtkIdType &movedIx)
{
	vtkIdType deletedIx = this->IdToIndex->Get(id);
	vtkIdType movedId;

	// if a node is moved, it is always the last node
	movedIx = this->NumberOfElements - 1;

	// Add the id to the end of the free id chain
	this->IdToIndex->Remove(id);
	this->NumberOfElements--;

	// Unless the element was at the end of the array,
	// move the element from the end into the hole created.
	if (deletedIx < movedIx)
		{
		this->Array[deletedIx] = this->Array[movedIx];
		movedId = this->IndexToId->GetValue(movedIx);
		this->IndexToId->SetValue(deletedIx, movedId);
		this->IdToIndex->SetEntry(movedId, deletedIx);
		return 1;
		}
	else
		{
		return 0;
		}
}

//---------------------------------------------------------

template<class  T>
vtkIdType vtkIndexedSet<T>::GetNumberOfElements()
{ return this->NumberOfElements; }

template<class  T>
T *vtkIndexedSet<T>::GetElementWithId(vtkIdType id)
{
	return this->Array + (this->IdToIndex->Get(id));
}

template<class  T>
T *vtkIndexedSet<T>::GetElementAtIndex(vtkIdType ix)
{
	return this->Array + ix;
}

template<class  T>
int vtkIndexedSet<T>::Contains(vtkIdType oid)
{
	return this->IdToIndex->HasKey(oid);
}

template<class  T>
vtkIdType vtkIndexedSet<T>::IndexOfId(vtkIdType id)
{
	return this->IdToIndex->Get(id);
}

 template<class  T>
vtkIdIterator *vtkIndexedSet<T>::GetIds()
{
	vtkIdArrayIterator *iter = vtkIdArrayIterator::New();

	// Traverse the array of IDs stored in the
	// collection, using the data array of the index to id
	// map.
	iter->SetArray(this->IndexToId->GetPointer(0));
	iter->SetStart(0);
	iter->SetEnd(this->GetNumberOfElements());
	iter->Initialize();
	
	return iter;
}

