/*
 *	File: vtkDualGeometry.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkDualGeometry - map graph to polydata of dual (work in progress)
// .SECTION Description
// vtkDualGeometry takes a graph to the polydata representation of a
// dual graph.  Each edge of the graph corresponds to a point in the 
// dual: at present, points in the original graph are not mapped into
// outputs.  The filter provides a number of opertions to control 
// properties of the dual: output points can be positioned at the 
// source, midpoint, or target of the edge that they represent, and
// the data associated with points can be obtained by copying or
// averaging the node data of the endpoints of the original edge. 
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkDualGeometry_h
#define __vtkDualGeometry_h

#include "vtkGraphToPolyDataFilter.h"

#define MODE_IGNORE 0
#define MODE_COPY_SOURCE	1
#define MODE_COPY_TARGET	2
#define MODE_AVERAGE 3

#define DUAL_AT_MIDPOINT	0
#define DUAL_AT_SOURCE		1
#define DUAL_AT_TARGET		2

//BTX
struct _vtkDualDataOption_s 
	{
	char *name; 	// array name
	int mode; 		// node data mode.
	};
//ETX

class VTK_EXPORT vtkDualGeometry : public vtkGraphToPolyDataFilter
{
public:
  static vtkDualGeometry *New();
  vtkTypeRevisionMacro(vtkDualGeometry,vtkGraphToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Determine where to position dual point relative to cell (edge)
	// Options are:
	// 1. At midpoint of cell (default)
	// 2. At source node
	// 3. At target node
	// Note that in the case of (2) and (3), there will probably be 
	// co-incident points in the output.
	void SetDualPointPositionToMidpoint();
	void SetDualPointPositionToSource();
	void SetDualPointPositionToTarget();
	
	// Description:
	// Set/get the dual point position mode.
	vtkSetMacro(DualPointPosition, int);
	vtkGetMacro(DualPointPosition, int);
	
	// Description:
	// Decide what to do about node data.  For any given node
	// data array, there are four options:
	// 1. ignore it (the default)
	// 2. set the dual point value to the source node value
	// 3. set the dual point value to the target node value
	// 4. set the dual point value to the average of src/tgt values.
	void SetNodeDataModeToCopySource(char *name);
	void SetNodeDataModeToCopyTarget(char *name);
	void SetNodeDataModeToAverage(char *name);
	void SetNodeDataModeToIgnore(char *name);
	void SetNodeDataMode(char *name, int mode);
	
protected:
  vtkDualGeometry();
  ~vtkDualGeometry();
	
  void Execute();

	//BTX
	_vtkDualDataOption_s *Mode; 	// array of mode settings
	//ETX
	int ModeArraySize;						// size of array (= nr of entries)
	int DualPointPosition;				// where to locate dual points
private:
  vtkDualGeometry(const vtkDualGeometry&) {};
  void operator=(const vtkDualGeometry&) {};
};

void vtkDualGeometry::SetNodeDataModeToCopySource(char *name)
{ this->SetNodeDataMode(name, MODE_COPY_SOURCE); };

void vtkDualGeometry::SetNodeDataModeToCopyTarget(char *name)
{ this->SetNodeDataMode(name, MODE_COPY_TARGET); };

void vtkDualGeometry::SetNodeDataModeToAverage(char *name)
{ this->SetNodeDataMode(name, MODE_AVERAGE); };

void vtkDualGeometry::SetNodeDataModeToIgnore(char *name)
{ this->SetNodeDataMode(name, MODE_IGNORE); };

// Dual-point option settings -----------------------

void vtkDualGeometry::SetDualPointPositionToMidpoint()
{ this->SetDualPointPosition(DUAL_AT_MIDPOINT); };

void vtkDualGeometry::SetDualPointPositionToSource()
{ this->SetDualPointPosition(DUAL_AT_SOURCE); };

void vtkDualGeometry::SetDualPointPositionToTarget()
{ this->SetDualPointPosition(DUAL_AT_TARGET); };


#endif	/* __vtkDualGeometry_h */

