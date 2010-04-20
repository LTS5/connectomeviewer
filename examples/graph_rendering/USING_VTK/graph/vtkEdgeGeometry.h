/*
 *	File: vtkEdgeGeometry.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 *
 *	Change Log:
 *	2003/11/20:	Added flags to preserve edge ids and 
 *							and to allow supression of bends.
 *	2003/11/13:	Added flag to reject 0-length edges.
 */
// .NAME vtkEdgeGeometry - map graph edges to polydata lines
// .SECTION Description
// vtkEdgeGeometry is a filter that extracts geometry from a
// graph.  Edges of the graph are mapped to lines, with the
// location of the start/endpoints determined by the layout
// of the source/target nodes.  If the graph edges contain 
// bends, each edge will be mapped to multiple line segments.
// Flags can be used to surpress output of bends, and to
// prevent generation of 0-length lines.
// 
// Because of the presence of bends, there is not in general
// a relationship between edges of the input graph and lines
// in the output polydata.  Instead, a flag is provided that
// causes the filter to generate a data array of point ids,
// associating the appropriate edge ID with each line.
//
// Any node and edge data on the input is copied to point and
// cell data on the output.  For bends, the point data on the
// output is interpolated from the node data of the source and
// target node; the cell data for each line segment in the bend
// is a copy of the edge data for the corresponding edge.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkEdgeGeometry_h
#define __vtkEdgeGeometry_h

#include "vtkGraphToPolyDataFilter.h"

class VTK_EXPORT vtkEdgeGeometry : public vtkGraphToPolyDataFilter
{
public:
  static vtkEdgeGeometry *New();
  vtkTypeRevisionMacro(vtkEdgeGeometry,vtkGraphToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description
	// Set/get flag forbidding generation of 0-length lines.
	// By default, the flag is on.
	vtkSetMacro(NonZeroLength, int);
	vtkGetMacro(NonZeroLength, int);
	vtkBooleanMacro(NonZeroLength, int);
	
	// Description:
	// Set/get the flag to control generation of edge IDs.
	// The default is OFF, i.e. no IDs are generated.
	vtkSetMacro(OutputEdgeIds, int);
	vtkGetMacro(OutputEdgeIds, int);
	vtkBooleanMacro(OutputEdgeIds, int);

	// Description:
	// Set/get the flag to control output of edge bends.
	// The default is ON, i.e. bends are mapped to lines.
	vtkSetMacro(OutputBends, int);
	vtkGetMacro(OutputBends, int);
	vtkBooleanMacro(OutputBends, int);

protected:
  vtkEdgeGeometry();
  ~vtkEdgeGeometry() {};
	
  void Execute();

	int NonZeroLength;
	int OutputEdgeIds;
	int OutputBends;

private:
  vtkEdgeGeometry(const vtkEdgeGeometry&) {};
  void operator=(const vtkEdgeGeometry&) {};
};

#endif	/* __vtkEdgeGeometry_h */

