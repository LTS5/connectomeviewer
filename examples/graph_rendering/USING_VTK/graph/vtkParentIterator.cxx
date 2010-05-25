/*
 *	File: vtkParentIterator.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkParentIterator.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkParentIterator);
vtkCxxRevisionMacro(vtkParentIterator, "1.0");

//--------------------------------------------------------------

vtkParentIterator::vtkParentIterator()
{
	this->Graph = NULL;
}

vtkParentIterator::~vtkParentIterator()
{
}

void vtkParentIterator::PrintSelf(ostream& os, vtkIndent indent)
{
	this->vtkIdArrayIterator::PrintSelf(os, indent);
}

//--------------------------------------------------------------

vtkIdType vtkParentIterator::GetNext()
{
	if (this->Position < this->EndPos)
		{
		return this->Graph->GetSource(this->Array[this->Position++]);
		}
	else
		{
		vtkWarningMacro(<<"Iterating beyond last parent.");
		return -1;
		}
}

