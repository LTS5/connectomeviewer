/*
 *	File: vtkSubgraphFilter.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkSubgraphFilter - extract a subgraph
// .SECTION Description
// vtkSubgraphFilter extracts a subgraph of its input, using a
// bit array in the node data to determine whether or not a 
// node is present in the output graph.  An edge is present in 
// the output if and only if it connects two nodes that are in 
// the output.
//
// The filter can also produce the remainder graph.  This 
// consists of all edges that are not in the output, including
// those edges which have one node in the output graph, and one
// node excluded from this graph.  Consequently, some nodes will
// be present in both the output graph and remainder graphs, if
// the latter is generated.

#ifndef __vtkSubgraphFilter_h
#define __vtkSubgraphFilter_h

#include "vtkGraphToGraphFilter.h"

	
class VTK_EXPORT vtkSubgraphFilter : public vtkGraphToGraphFilter
{
public:
  static vtkSubgraphFilter *New();
  vtkTypeRevisionMacro(vtkSubgraphFilter,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
  
	// -------------------------------------------------------
	// Filter controls
	
	// Description:
	// Get the graph defining the "remainder", that part of the
	// input not included in the output.
	vtkGraph *GetRemainder();
 
	// Description:
	// Set the name of the data array which determines if a 
	// node is included in the output.
	vtkSetStringMacro(SelectorName);
	vtkGetStringMacro(SelectorName);
	
	// Description:
	// Set/get the flag to keep the remainder of the graph,
	// corresponding to the subgraph that does not satisfy
	// the filter requirement.
	vtkSetMacro(KeepRemainder,int);
	vtkGetMacro(KeepRemainder,int);
	vtkBooleanMacro(KeepRemainder, int);
			
protected:
  vtkSubgraphFilter();
  ~vtkSubgraphFilter();
   
  void Execute();
	
	int KeepRemainder;	// flag: generate the remainder graph?
	char *SelectorName; // name of selecting node data array 
	
private:
  vtkSubgraphFilter(const vtkSubgraphFilter&) {};
  void operator=(const vtkSubgraphFilter&) {};
};

#endif

