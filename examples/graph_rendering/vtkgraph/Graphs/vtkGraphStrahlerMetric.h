/*
 *	File: vtkGraphStrahlerMetric.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGraphStrahlerMetric - compute Strahler metric for a graph
// .SECTION Description
// vtkGraphStrahlerMetric calculates the Strahler values for a graph.
// This is based on the generalization by D. Auber, LaBRI, of the
// tree-based Strahler metric of Herman et al (see vtkStrahlerMetric).
// The general Strahler metric is described in:
//
// D. Auber, "Using Strahler Numbers for Real Time Visual Exploration 
// of Huge Graphs", Intl. Journal of Applied Mathematics and Computer
// Science, 2002.
//
// Implementation of this filter is based partially on the 
// GraphStrahlerMetric module of the "Tulip" tool; see 
// www.tulip-software.org. 
//
// .SECTION See Also
// vtkGraph vtkStrahlerMetric

#ifndef __vtkGraphStrahlerMetric_h
#define __vtkGraphStrahlerMetric_h

#include "vtkGraphToGraphFilter.h"

class VTK_EXPORT vtkGraphStrahlerMetric : public vtkGraphToGraphFilter 
{
public:
  static vtkGraphStrahlerMetric *New();
  vtkTypeRevisionMacro(vtkGraphStrahlerMetric,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Set/get setting of normalize flag, which determines whether
	// or not the Strahler values are scaled into the range 0..1.
  // Default is for normalization to be OFF.
	vtkSetMacro(Normalize, int);
	vtkGetMacro(Normalize, int);
	vtkBooleanMacro(Normalize, int);

protected:
  vtkGraphStrahlerMetric();
  ~vtkGraphStrahlerMetric() {};

  void Execute();

	float MaxStrahler;
	int Normalize;

	// Arrays used for  intermediate results
	float *Strahler;				// strahler value computed for a node
	vtkIdType *Stacks;			// stacks required by node
	vtkIdType *UsedStacks;	// stacks used by subgraph
	unsigned char *Seen;
	unsigned char *Done;
	
	// Compute the graph strahler value of a node in a graph
	void ComputeGStrahler(vtkIdType node, vtkGraph *graph );

	// Auxilliary functions to implement merge-sort.
	void Merge(
		float *array,
		float *temp,
		vtkIdType leftPos,
		vtkIdType rightPos,
		vtkIdType rightEnd);
	
	void MergeSort(
		float *array,
		float *temp,
		vtkIdType left,
		vtkIdType right);

private:
  vtkGraphStrahlerMetric(const vtkGraphStrahlerMetric&) {};
  void operator=(const vtkGraphStrahlerMetric&) {};
};

#endif	/* __vtkGraphStrahlerMetric_h */

