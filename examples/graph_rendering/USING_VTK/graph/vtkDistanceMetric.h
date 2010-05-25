/*
 *	File: vtkDistanceMetric.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkDistanceMetric - compute graph distance metric 
// .SECTION Description
// vtkDistanceMetric calculates the shortest (unweighted)
// distance from either a given start node, or from source
// node(s) of the graph, depending on the filter mode. 
// The distances are stored in a node data array, the name 
// of which is given by the client.  The filter can be 
// configured to normalize the results.
//
// .SECTION See Also
// vtkGraph 

#ifndef __vtkDistanceMetric_h
#define __vtkDistanceMetric_h

#include "vtkGraphToGraphFilter.h"
#include "vtkFloatArray.h"

class VTK_EXPORT vtkDistanceMetric : public vtkGraphToGraphFilter
{
public:
  static vtkDistanceMetric *New();
  vtkTypeRevisionMacro(vtkDistanceMetric,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Set/get the name of the array into which the metric will
	// be stored.
	vtkSetStringMacro(ArrayName);
	vtkGetStringMacro(ArrayName);

	// Description:
	// Set the starting point for traversal; this can either
	// be an explicit node, or it can be the source node(s) 
	// of the graph.
	void SetStartModeToUseNode(vtkIdType nodeId);
	void SetStartModeToUseSources();

	// Description:
	// Set/get setting of normalize flag, which determines whether
	// or not the values are scaled into the range 0..1.
  // Default is for normalization to be OFF.
	vtkSetMacro(Normalize, int);
	vtkGetMacro(Normalize, int);
	vtkBooleanMacro(Normalize, int);

  // Description:
  // Get the maximum distance found.
  vtkGetMacro(MaxDistance,float);

protected:
  vtkDistanceMetric();
  ~vtkDistanceMetric();
   
  void Execute();

	int Normalize;
	float MaxDistance;
	char *ArrayName;
	int StartMode;					// 0 = use node, 1 = use sources
	vtkIdType StartNode;		// gives start node when StartMode = 0

private:
  vtkDistanceMetric(const vtkDistanceMetric&) {};
  void operator=(const vtkDistanceMetric&) {};
};

#endif	/* __vtkDistanceMetric_h */

