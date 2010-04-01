/*
 *	File: vtkGraphToUnstructuredGrid.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGraphToUnstructuredGrid - convert graph to unstructured grid
// .SECTION Description
// vtkGraphToUnstructuredGrid is a filter that converts a graph to an
// unstructured grid:
//
//  +  nodes are mapped to points
//  +  node ids are conveyed by a new point data array
//  +  edges with no bends are mapped to lines
//  +  edges with bends are mapped to polylines
//  +  bend points are appended to points representing nodes
//  +  node and bend position is conveyed by points; nodes 
//     can be distinguised by their nodeId data array entry
//  +  node data is mapped to point data
//  +  edge data is mapped to cell data
//
// By default, points in the output that correspond to edge bends 
// have no associated point or data.  New data can be constructed 
// by setting a flag.  There is no such issue with edges, as each
// edge of the input graph is mapped to a single cell of the
// output.
//
// .SECTION See Also
// vtkGraph vtkUnstructuredGrid

#ifndef __vtkGraphToUnstructuredGrid_h
#define __vtkGraphToUnstructuredGrid_h

#include "vtkUnstructuredGridSource.h"
#include "vtkGraph.h"


class VTK_EXPORT vtkGraphToUnstructuredGrid : public vtkUnstructuredGridSource
{
public:
  static vtkGraphToUnstructuredGrid *New();
  vtkTypeRevisionMacro(vtkGraphToUnstructuredGrid,vtkUnstructuredGridSource);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Set/get the flag to control interpolation of bend point data
	// from node data.  Default is off.  When interpolation is on,
	// the data values for the ith bend point between src and dst
	// are interpolated linearly from data values of src and dst.
	vtkSetMacro(InterpolateNodeData, int);
	vtkGetMacro(InterpolateNodeData, int);
	vtkBooleanMacro(InterpolateNodeData, int);
	
	
  //---------------------------------------------------------
	// Access filter input.
	
  // Description:
  // Set/get the input graph.
  virtual void SetInput(vtkGraph *input);
  vtkGraph *GetInput();
  
protected:  
  vtkGraphToUnstructuredGrid();
  ~vtkGraphToUnstructuredGrid() {};

	int InterpolateNodeData;
	
	void Execute();

  void ComputeInputUpdateExtents(vtkDataObject *output);
	
private:
  vtkGraphToUnstructuredGrid(const vtkGraphToUnstructuredGrid&);  // Not implemented.
  void operator=(const vtkGraphToUnstructuredGrid&);  // Not implemented.
};

#endif	/* __vtkGraphToUnstructuredGrid_h */


