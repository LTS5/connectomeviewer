/*
 *	File: vtkNodeGeometry.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkNodeGeometry - map graph to node positions
// .SECTION Description
// vtkNodeGeometry takes an input graph and produces an output
// polydata that is empty apart from a points object, which is
// the position assigned to each node.  This filter is useful
// in conjunction with vtkGlyph3D for extracting the nodes of
// a graph and visualizing them as glyphs.
//
// .SECTION See Also
// vtkGraph vtkEdgeGeometry

#ifndef __vtkNodeGeometry_h
#define __vtkNodeGeometry_h

#include "vtkGraphToPolyDataFilter.h"

class VTK_EXPORT vtkNodeGeometry : public vtkGraphToPolyDataFilter
{
public:
  static vtkNodeGeometry *New();
  vtkTypeRevisionMacro(vtkNodeGeometry,vtkGraphToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  void Execute();

protected:
  vtkNodeGeometry() {};
  ~vtkNodeGeometry() {};
	
private:
  vtkNodeGeometry(const vtkNodeGeometry&) {};
  void operator=(const vtkNodeGeometry&) {};
};

#endif	/* __vtkNodeGeometry_h */

