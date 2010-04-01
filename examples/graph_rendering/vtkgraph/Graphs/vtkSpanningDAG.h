/*
 *	File: vtkSpanningDAG.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkSpanningDAG - select a spanning DAG of a graph
// .SECTION Description
// vtkSpanningDAG extracts a spanning DAG (directed acyclic
// subgraph) from its input graph.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkSpanningDAG_h
#define __vtkSpanningDAG_h

#include "vtkGraphToGraphFilter.h"
#include "vtkBitArray.h"

class VTK_EXPORT  vtkSpanningDAG : public vtkGraphToGraphFilter 
{
public:
  static vtkSpanningDAG *New();
  vtkTypeRevisionMacro(vtkSpanningDAG,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkSpanningDAG();
  ~vtkSpanningDAG() {};

  void Execute();

  void Traverse(
		vtkIdType node,
		vtkGraph *graph, 
		vtkGraph *output);

	// Arrays used to implement traversal of graph, declared
	// as instance variables to save passing between calls.	
	unsigned char *Seen;
	unsigned char *Done;

private:
  vtkSpanningDAG(const vtkSpanningDAG&) {};
  void operator=(const vtkSpanningDAG&) {};
};

#endif

