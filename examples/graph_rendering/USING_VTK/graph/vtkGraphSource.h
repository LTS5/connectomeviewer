/*
 *	File: vtkGraphSource.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGraphSource - abstract class whose subclasses generate graphs
// .SECTION Description
// vtkGraphSource is an abstract class intended as a superclass for
// readers and filters that produce graph datasets.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkGraphSource_h
#define __vtkGraphSource_h

#include "vtkSource.h"
#include "vtkGraph.h"

class VTK_EXPORT vtkGraphSource : public vtkSource
{
public:
  vtkTypeRevisionMacro(vtkGraphSource,vtkSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  //---------------------------------------------------------
	// Access to output graph.
	
  // Description:
  // Get the output graph of this source.
  vtkGraph *GetOutput();
	
  // Description:
  // Get the ith output graph of this source.
  vtkGraph *GetOutput(int idx);
	
  // Description:
  // Set the output of this source.
  void SetOutput(vtkGraph *output);
  
protected:
  vtkGraphSource();
  ~vtkGraphSource() {};

private:
  vtkGraphSource(const vtkGraphSource&);  // Not implemented.
  void operator=(const vtkGraphSource&);  // Not implemented.
};

#endif


