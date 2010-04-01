/*
 *	File: vtkNodeGeometry.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkNodeGeometry.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkCellData.h"

vtkStandardNewMacro(vtkNodeGeometry);
vtkCxxRevisionMacro(vtkNodeGeometry, "1.0");

//--------------------------------------------------

void vtkNodeGeometry::Execute()
{
  vtkGraph    *input  = this->GetInput();
  vtkPolyData *output = this->GetOutput();

  vtkDebugMacro(<< "NodeGeometry executing");

	// Set points of output polydata, and copy node
	// data to the point data output.
  output->SetPoints(input->GetLayout());
  output->GetPointData()->ShallowCopy(input->GetNodeData() );

	vtkDebugMacro(<< "NodeGeometry done.\n");
}


void vtkNodeGeometry::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "NodeGeometry Filter.\n";
}

