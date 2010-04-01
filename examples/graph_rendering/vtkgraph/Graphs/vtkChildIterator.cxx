/*
 *	File: vtkChildIterator.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkChildIterator.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkChildIterator);
vtkCxxRevisionMacro(vtkChildIterator, "1.0");

//------------------------------------------------------------

vtkChildIterator::vtkChildIterator()
{
	this->Graph = NULL;
}

vtkChildIterator::~vtkChildIterator()
{
	if (this->Graph)
		{
		this->Graph->UnRegister(this);
		}
}

void vtkChildIterator::PrintSelf(ostream& os, vtkIndent indent)
{
	this->vtkIdArrayIterator::PrintSelf(os, indent);
}

//------------------------------------------------------------

vtkIdType vtkChildIterator::GetNext()
{
	if (this->Position < this->EndPos)
		{
		return this->Graph->GetTarget(this->Array[this->Position++]);
		}
	else
		{
		vtkWarningMacro(<<"Iterating beyond last child.");
		return -1;
		}
}

