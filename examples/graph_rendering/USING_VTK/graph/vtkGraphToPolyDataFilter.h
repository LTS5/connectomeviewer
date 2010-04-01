/*
 *	File: vtkGraphToPolyDataFilter.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGraphToPolyDataFilter - abstract filter class
// .SECTION Description
// vtkGraphToPolyDataFilter is an abstract filter class whose
// subclasses take graphs as input and generate polygonal 
// data as output.
//
// .SECTION See Also
// vtkGraph vtkPolyData

#ifndef __vtkGraphToPolyDataFilter_h
#define __vtkGraphToPolyDataFilter_h

#include "vtkPolyDataSource.h"
#include "vtkGraph.h"


class VTK_EXPORT vtkGraphToPolyDataFilter : public vtkPolyDataSource
{
public:
  vtkTypeRevisionMacro(vtkGraphToPolyDataFilter,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  //---------------------------------------------------------
	// Access filter input.
	
  // Description:
  // Set/get the input graph.
  virtual void SetInput(vtkGraph *input);
  vtkGraph *GetInput();
  
protected:  
  vtkGraphToPolyDataFilter() {this->NumberOfRequiredInputs = 1;};
  ~vtkGraphToPolyDataFilter() {};

  void ComputeInputUpdateExtents(vtkDataObject *output);
	
private:
  vtkGraphToPolyDataFilter(const vtkGraphToPolyDataFilter&);  // Not implemented.
  void operator=(const vtkGraphToPolyDataFilter&);  // Not implemented.
};

#endif	/* __vtkGraphToPolyDataFilter_h */


