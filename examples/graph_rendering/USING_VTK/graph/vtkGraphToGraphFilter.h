/*
 *	File: vtkGraphToGraphFilter.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGraphToGraphFilter - abstract filter class
// .SECTION Description
// vtkGraphToGraphFilter is an abstract filter class whose
// subclasses take graphs as input and generate graphs as
// output.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkGraphToGraphFilter_h
#define __vtkGraphToGraphFilter_h

#include "vtkGraphSource.h"


class VTK_EXPORT vtkGraphToGraphFilter : public vtkGraphSource
{
public:
  vtkTypeRevisionMacro(vtkGraphToGraphFilter,vtkGraphSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  //---------------------------------------------------------
	// Access filter input.
	
  // Description:
  // Set/get the input graph.
  virtual void SetInput(vtkGraph *input);
  vtkGraph *GetInput();
  
protected:  
  vtkGraphToGraphFilter() {this->NumberOfRequiredInputs = 1;};
  ~vtkGraphToGraphFilter() {};

  void ComputeInputUpdateExtents(vtkDataObject *output);
	
private:
  vtkGraphToGraphFilter(const vtkGraphToGraphFilter&);  // Not implemented.
  void operator=(const vtkGraphToGraphFilter&);  // Not implemented.
};

#endif	/* __vtkGraphToGraphFilter_h */


