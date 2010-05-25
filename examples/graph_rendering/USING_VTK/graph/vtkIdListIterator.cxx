/*
 *	File: vtkIdListIterator.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkIdListIterator.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkIdListIterator);

//-----------------------------------------------------------

vtkIdListIterator::vtkIdListIterator()
{
	this->List = NULL;
	this->Position = 0;
}

vtkIdListIterator::~vtkIdListIterator()
{
	if (this->List)
		{
		this->List->UnRegister(this);
		}
}

void vtkIdListIterator::PrintSelf(ostream& os, vtkIndent indent)
{
	if (this->List)
		{
		os << indent << "Position:  " << this->Position << "\n";
		os << indent << "List size: " << this->List->GetNumberOfIds() << "\n";
		}
	else
		{
		os << indent << "No List.\n";
		}
}

void vtkIdListIterator::Initialize()
{
	this->Position = 0;
}

//-----------------------------------------------------------

int vtkIdListIterator::HasNext()
{
	return (this->Position < this->List->GetNumberOfIds());
}

vtkIdType vtkIdListIterator::GetNext()
{
	if (this->HasNext())
		{
		return this->List->GetId(this->Position++);
		}
	else
		{
		vtkWarningMacro(<<"Iterating beyond end of list.");
		return -1;
		}
}

