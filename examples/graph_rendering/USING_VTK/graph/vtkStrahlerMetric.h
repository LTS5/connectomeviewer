/*
 *	File: vtkStrahlerMetric.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkStrahlerMetric - compute Strahler metric for a tree
// .SECTION Description
// vtkStrahlerMetric calculates the Strahler values for a tree
// (or a set of trees, a forest). The Strahler metric is a 
// value assigned to each node of a tree that, in a sense, 
// captures the structural complexity of the sub-tree rooted 
// at that node.  Full details of the metric and the rationale 
// for using it can be found in the following paper:
// 
// Tree Visualization and Navigation Clues for Information
// Visualization, I. Herman, M. Delest, and G. Melancon,
// Computer Graphics Forum, Vol 17(2), Blackwell, 1998.
//
// It is the caller's responsibilty to ensure that the 
// input graph is a tree or forest.  If the input is a
// general graph, vtkGraphStrahlerMetric should be used.
// The implementation here is based in part on a module 
// of the "Latour" tool, written by I. Herman, G. Melancon 
// and M.S. Marshall from CWI Amsterdam: see 
// www.cwi.nl/ projects/InfoVisu/OldStuff/
//
// .SECTION See Also
// vtkGraph vtkGraphStrahlerMetric


#ifndef __vtkStrahlerMetric_h
#define __vtkStrahlerMetric_h

#include "vtkGraphToGraphFilter.h"
#include "vtkFloatArray.h"

class VTK_EXPORT vtkStrahlerMetric : public vtkGraphToGraphFilter
{
public:
  static vtkStrahlerMetric *New();
  vtkTypeRevisionMacro(vtkStrahlerMetric,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Set/get setting of normalize flag, which determines whether
	// or not the Strahler values are scaled into the range 0..1.
  // Default is for normalization to be OFF.
	vtkSetMacro(Normalize, int);
	vtkGetMacro(Normalize, int);
	vtkBooleanMacro(Normalize, int);

  // Description:
  // Get the maximum strahler value for the tree.
  vtkGetMacro(MaxStrahler,float);

protected:
  vtkStrahlerMetric();
  ~vtkStrahlerMetric() {};
   
  void Execute();

	int Normalize;
	float MaxStrahler;

  float CalculateStrahler(
		vtkIdType root, 
		vtkFloatArray *metric,
		vtkGraph *graph);
  
private:
  vtkStrahlerMetric(const vtkStrahlerMetric&) {};
  void operator=(const vtkStrahlerMetric&) {};
};

#endif

