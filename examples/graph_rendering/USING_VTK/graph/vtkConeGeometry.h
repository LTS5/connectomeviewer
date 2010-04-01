/*
 *	File: vtkConeGeometry.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkConeGeometry - map a forest to a fan-based polygonal dataset.
// .SECTION Description
// vtkConeGeometry converts a forest into a set of fans (triangles) 
// and lines in the output vtkPolyData.  The input to the filter is
// a graph dataset which assigns a position to each node, i.e. the
// graph has been laid out using a filter such as vtkConeLayout.
//
// Each edge in the input forest between a node with one child and
// the parent is represented by a sinle line in the polydata.
//
// For nodes with more than one child, we consider each successive
// pair of children in turn.  If the angle between the children (at 
// the parent) is less than a given threshold (BladeAngle), a
// triangle is output whose vertices are the parent and the two
// children.  If the angle between the children is greater than the
// threshold, the next pair of children are considered, and the 
// link between the first child and parent is represented, if
// necessary, by a sinle line in the output. 
//
// The input need not be a forest, but in this case the output may
// be difficult to interpret.
//
// Each point in the output polydata corresponds to a node of the
// input graph, and the index of the point will be the same as the
// index of the node.  However, there is no simple mapping between
// output lines and input edges; if this is required, use
// vtkEdgeGeometry.
//
// .SECTION See Also
// vtkGraph


#ifndef __vtkConeGeometry_h
#define __vtkConeGeometry_h

#include "vtkGraphToPolyDataFilter.h"

class VTK_EXPORT vtkConeGeometry : public vtkGraphToPolyDataFilter
{
public:
  // Description:
  // Standard facilities for creation, type-casting and printing.
  static vtkConeGeometry *New();
  vtkTypeRevisionMacro(vtkConeGeometry,vtkGraphToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set / get the threshold that determines whether successive
  // pairs of children will be linked by a polygon (i.e. will
  // continue a fan) or whether a new fan should be started.  
  vtkSetMacro(MaxBladeAngle,float);
  vtkGetMacro(MaxBladeAngle,float);

protected:
  vtkConeGeometry();
  ~vtkConeGeometry() {};
  
  void Execute();
  
	// Description:
  // Calculate the angle (in degrees) between the edges c->p 
	// and c->q, based on the position of these points given 
	// by layout assigned to the graph.
  double Angle(
		vtkIdType c, 
		vtkIdType p, 
		vtkIdType q, 
		vtkGraph *graph);

  float MaxBladeAngle;  // Threshold angle to make a new fan.

private:
  vtkConeGeometry(const vtkConeGeometry&) {};
  void operator=(const vtkConeGeometry&) {};
};

#endif


