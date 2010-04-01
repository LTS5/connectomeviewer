/*
 *	File: vtkGEMLayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGEMLayout - 2D graph layout using the GEM algorithm
// .SECTION Description
// vtkGEMLayout positions a graph using the force-directed
// placement algorithm first published as:
//
//   A. Frick, A. Ludwig, and H. Mehldau, "A fast, adaptive 
//   layout algorithm for undirected graphs", In R. Tamassia
//   and I. Tollis (Eds), Graph Drawing'94, Volume 894 of 
//   Lecture Notes in Computer Science, Springer Verlag, 1995.
//
// The implementation started life as the public-domain 
// code produced by Arne Frick and released at
// www.frick-consulting.de/publications.html
//
// The core "embedder" part of the algorithm was used by the
// developer (DJD) in the implementation of a Java plugin for 
// the CWI "Royere"  tool, and then this code was ported to C++ 
// and VTK.  The embedder algorithm described by Frick involves 
// three phases: insertion, arrangement, and optimization. Only
// the first two of these phases are included here. 
// Experiments with the Java implementation showed that the
// optimization phase consumed significantly more time than
// the first two and produced apparently marginal improvements
// in the final layout.
//
// .SECTION CAVEATS
// GEM, like other spring-embedder algorithms, is 
// computationally expensive.  It works fine for graphs with
// around 1K nodes, but on stock PC hardware (1GHz Celeron
// for example) it will take hours on graphs of 10K nodes.
// More efficient force-directed methods are now known.
//
// .SECTION See Also
// vtkSpanLayout vtkConeLayout
 
#ifndef __vtkGEMLayout_h
#define __vtkGEMLayout_h

#include "vtkGraphToGraphFilter.h"
#include "vtkBitArray.h"


class VTK_EXPORT vtkGEMLayout : public vtkGraphToGraphFilter 
{ 
public:
  static vtkGEMLayout *New();
  vtkTypeRevisionMacro(vtkGEMLayout,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
	vtkGEMLayout();
	~vtkGEMLayout();

	void Execute();
	
private: 
  vtkGEMLayout(const vtkGEMLayout&) {};
  void operator=(const vtkGEMLayout&) {};

	// Data structures used during layout; space is
	// allocated dynamically based  on graph size.
	vtkIdType *In;
	double *Ximpulse;
	double *Yimpulse;
	double *Direction;
	double *Heat;
	double *Mass;
	vtkBitArray *Mark;
	vtkIdType *Map;
	vtkIdType *Q;
  vtkIdType Head;
	vtkIdType Tail;

	// Variables recording layout state and progress.	  
	unsigned long Iteration;
	long int MaxTemp;
	double Oscillation;
	double Rotation;
	double Center[3];
	long int Temperature;
	vtkIdType NodeCount;

	// Functions used to implement the GEM layout.
     
	void SetGEMParameters();
	vtkIdType Select();
	vtkIdType Bfs(vtkGraph *graph, vtkIdType root);
	vtkIdType GraphCenter(vtkGraph *graph);
	void VertexDataInit(vtkGraph *graph, double starttemp);
	void IImpulse(vtkGraph *graph, vtkIdType v, double &ix, double &iy);
	void Insert(vtkGraph *graph);
	void Displace(vtkGraph *graph, vtkIdType v, long impx, long impy);
	void ARound(vtkGraph *graph);
	void Arrange(vtkGraph *graph);

	// GEM Run-time parameters.    
	double i_maxtemp;
	double a_maxtemp;
	double i_starttemp;
	double a_starttemp;
	double i_finaltemp;
	double a_finaltemp;
	int i_maxiter;
	int a_maxiter;
	double i_gravity;
	double i_oscillation;
	double i_rotation;
	double i_shake;
	double a_gravity;
	double a_oscillation;
	double a_rotation;
	double a_shake;

	// GEM Constants
	static long ELEN()			{ return 128L; };
	static long ELENSQR() 	{ return ELEN() * ELEN(); };
	static long MAXATTRACT(){ return 1048576L; };

	// GEM Default Parameter Values
	static double IMAXTEMPDEF()     { return (double)1.0; };
	static double ISTARTTEMPDEF()   { return (double)0.3; };
	static double IFINALTEMPDEF()   { return (double)0.05; };
	static int    IMAXITERDEF()     { return 10; };
	static double IGRAVITYDEF()     { return (double)0.05; };
	static double IOSCILLATIONDEF() { return (double)0.4; };
	static double IROTATIONDEF()    { return (double)0.5; };
	static double ISHAKEDEF()       { return (double)0.2; };
	
	static double AMAXTEMPDEF()     { return (double)1.5; };
	static double ASTARTTEMPDEF()   { return (double)1.0; };
	static double AFINALTEMPDEF()   { return (double)0.02; };
	static int    AMAXITERDEF()     { return 3; };
	static double AGRAVITYDEF()     { return(double)0.1; };
	static double AOSCILLATIONDEF() { return (double)0.4; };
	static double AROTATIONDEF()    { return (double)0.9; };
	static double ASHAKEDEF()       { return (double)0.3; };
};

#endif
