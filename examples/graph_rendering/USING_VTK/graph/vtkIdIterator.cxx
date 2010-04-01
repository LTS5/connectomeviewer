/*
 *	File: vtkIdIterator.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkIdIterator.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkIdIterator);

//----------------------------------------------------------

void vtkIdIterator::PrintSelf(ostream& os, vtkIndent indent)
{
	this->vtkObject::PrintSelf(os, indent);
}

void vtkIdIterator::Initialize()
{
}

//----------------------------------------------------------
	
int vtkIdIterator::HasNext()
{
	return -1;
}

vtkIdType vtkIdIterator::GetNext()
{
	return -1;
}


