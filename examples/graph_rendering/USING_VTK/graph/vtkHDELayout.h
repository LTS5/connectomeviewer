/*
 *	File: vtkHDELayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkHDELayout - layout using higher dimensional embedding
// .SECTION Description
// vtkHDELayout is an implementation of the algorithm described
// in the following paper:
//
//   D. Harel and Y. Koren, "Graph Drawing by High-Dimensional
//   Embedding", Proceedings of Graph Drawing 2002, Volume
//   2528 of Lecture Notes in Computer Science, pp. 207-219,
//   Springer Verlag, 2002.
//
// The algorithm requires only two parameters: the dimensionality
// of the space in which the embedding is performed (default 40),
// and the dimension of the space in which the drawing is done
// (effectively either 2 or 3).  The size of the embedding space
// must be no larger that the number of nodes; for a small graph,
// a small space is required.
//
// .SECTION Caveats
// The graph is assumed to be connected; if there are disconnected
// components the output will not be correct.  For DAGs, a
// connected graph can be constructed by using vtkForestToTree.
// The algorithm has the capability to provide for "zooming in"
// to regions of interest, but this has yet to be implemented.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkHDELayout_h
#define __vtkHDELayout_h

#include "vtkGraphToGraphFilter.h"
#include "vtkGraph.h"

class VTK_EXPORT  vtkHDELayout : public vtkGraphToGraphFilter
{
public:
  static vtkHDELayout *New();
  vtkTypeRevisionMacro(vtkHDELayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Set the dimensionality of the embedding space (default 40)
	vtkSetMacro(M,int);
	vtkGetMacro(M,int);
	
	// Description:
	// Set the dimensionality of the coordinate space (default 3).
	vtkSetMacro(C,int);
	vtkGetMacro(C,int);
	
protected:
  vtkHDELayout();
  ~vtkHDELayout() {};

  void Execute();

	int M;	// dimensionality of embedding space
	int C;	// dimensionality of drawing space

	vtkIdType *Centers;
	char			*Known;
	char			*Pivot;
	vtkIdType *Distance;
	vtkIdType *D;
	double *X;	// coordinates of each node relative to pivot
	double *S;	// covariance matrix of X
	double *U;	// first K eigenvectors of S
	double *Mean;

	// Queue for use in breadth-first traversal
	vtkIdType *Queue;
	vtkIdType Head;
	vtkIdType Tail;

	void PositionFrom(int axis, double *coord, vtkGraph *graph);

private:
  vtkHDELayout(const vtkHDELayout&) {};
  void operator=(const vtkHDELayout&) {};
};

#endif	/* vtkHDELayout. */

