/*
 *	File: vtkIdArrayIterator.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkIdArrayIterator.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkIdArrayIterator);

//--------------------------------------------------------------

vtkIdArrayIterator::vtkIdArrayIterator()
{
	this->Array = NULL;
	this->StartPos = 0;
	this->EndPos = 0;
	this->Position = 0;
}

vtkIdArrayIterator::~vtkIdArrayIterator()
{
}

void vtkIdArrayIterator::PrintSelf(ostream& os, vtkIndent indent)
{
	if (this->Array)
		{
		os << indent << "Start:    " << this->StartPos << "\n";
		os << indent << "Position: " << this->Position << "\n";
		os << indent << "End:      " << this->EndPos << "\n";
		}
	else
		{
		os << indent << "No array set.\n";
		}
}

void vtkIdArrayIterator::Initialize()
{
	this->Position = this->StartPos;
}

//--------------------------------------------------------------

int vtkIdArrayIterator::HasNext()
{
	return (this->Position < this->EndPos);
}

vtkIdType vtkIdArrayIterator::GetNext()
{
	if (this->Position < this->EndPos)
		{
		return this->Array[this->Position++];
		}
	else
		{
		vtkWarningMacro(<<"Iterating beyond end of array.");
		return -1;
		}
}

//--------------------------------------------------------------

void vtkIdArrayIterator::SetArray(vtkIdType *array)
{ this->Array = array; };

void vtkIdArrayIterator::SetStart(vtkIdType pos)
{ this->StartPos = pos; };

void vtkIdArrayIterator::SetEnd(vtkIdType pos)
{ this->EndPos = pos; };


