/*
 *	File: vtkIndexedSet.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 *
 *	Code contributed by John Shalf.
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.
 *
 */

#include "vtkIndexedSet.txx"

#ifndef VTK_NO_EXPLICIT_TEMPLATE_INSTANTIATION

template class VTK_EXPORT vtkIndexedSet<_vtkNode_s>;
template class VTK_EXPORT vtkIndexedSet<_vtkEdge_s>;

#endif

