/*
 *	File: vtkUnstructuredGridToGraph.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkUnstructuredGridToGraph - construct graph from unstructured grid
// .SECTION Description
// vtkUnstructuredGridToGraph is a filter that converts an unstructured
// grid to a graph.  Only certain unstructured grids describe a graph;
// the grid must satisfy the following criteria:
//
//  +  point data includes a vtkIdType array called nodeID
//  +  cell data includes a vtkIdType array called edgeID
//  +  cells are either of type polyline or type line.
//  +  the two point ids in a line cell, and the first and
//     and last point id in a polyline cell, are of points
//     that have non-negative values for nodeID
//  +  all other points have nodeID = -1
//
// The mapping to a graph is performed as follows:
//
//  +  points with nodeID >= 0 are mapped to nodes.
//  +  points with nodeID = -1 are mapped to bends.
//  +  each cell is mapped to an edge
//  +  point data is mapped to node data; any point data for
//     points with nodeID = -1 is lost.
//  +  cell data is mapped to edge data
//
// .SECTION See Also
// vtkGraph vtkUnstructuredGrid vtkGraphToUnstructuredGrid

#ifndef __vtkUnstructuredGridToGraph_h
#define __vtkUnstructuredGridToGraph_h

#include "vtkGraphSource.h"
#include "vtkGraph.h"
#include "vtkUnstructuredGrid.h"

class VTK_EXPORT vtkUnstructuredGridToGraph : public vtkGraphSource
{
public:
  static vtkUnstructuredGridToGraph *New();
  vtkTypeRevisionMacro(vtkUnstructuredGridToGraph,vtkGraphSource);
  void PrintSelf(ostream& os, vtkIndent indent);

	
  //---------------------------------------------------------
	// Access filter input.
	
  // Description:
  // Set/get the input graph.
  virtual void SetInput(vtkUnstructuredGrid *input);
  vtkUnstructuredGrid *GetInput();
  
protected:  
  vtkUnstructuredGridToGraph();
  ~vtkUnstructuredGridToGraph() {};

	void Execute();

  void ComputeInputUpdateExtents(vtkDataObject *output);
	
private:
  vtkUnstructuredGridToGraph(const vtkUnstructuredGridToGraph&);  // Not implemented.
  void operator=(const vtkUnstructuredGridToGraph&);  // Not implemented.
};

#endif	/* __vtkUnstructuredGridToGraph_h */


