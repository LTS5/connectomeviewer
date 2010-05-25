/*
 *	File: vtkSelectReachable.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkSelectReachable - select nodes reachable from a given node
// .SECTION Description
// vtkSelectReachable selects nodes of a given graph depending on
// whether or not they are reachable from a given start node. The
// filter can be configured to treat edges as undirected, directed,
// or directed and reversed.
//
// Nodes are marked as selected via a bit-array in the output node data.  
// This class is expected to be used in conjunction with a filter such 
// as vtkSubgraphFilter that uses a selection to extract 
// a subgraph. 
//
// .SECTION See Also
// vtkSubgraphFilter vtkGraph

#ifndef __vtkSelectReachable_h
#define __vtkSelectReachable_h

#include "vtkGraphToGraphFilter.h"

//BTX
typedef enum {
  SEARCH_UNDIRECTED = 0,
  SEARCH_DIRECTED   = 1,
  SEARCH_REVERSED   = 2
	} SearchMode;
//ETX

class VTK_EXPORT vtkSelectReachable : public vtkGraphToGraphFilter
{
public:
  static vtkSelectReachable *New();
  vtkTypeRevisionMacro(vtkSelectReachable,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);


  //---------------------------------------------------------
	// Filter parameters.
	
	// Description:
	// Set/get the name of the array used to mark nodes as selected.  
	vtkSetStringMacro(SelectorName);
	vtkGetStringMacro(SelectorName);
	
	// Description:
	// Set/get the name of the node from which to start.
  void SetStartNode(vtkIdType nodeId);
  vtkIdType GetStartNode();

	// Description:
	// Set the maximum distance to consider
	void SetMaxDistance(vtkIdType dist);
	void SetMaxDistanceToInfinity();
	vtkIdType GetMaxDistance();
	
	// Description:
  // Set the mode by which edges are followed.
  vtkSetMacro(SearchMode, int);
  vtkGetMacro(SearchMode, int);
  void SetSearchModeToUndirected() {this->SetSearchMode(SEARCH_DIRECTED); };
  void SetSearchModeToDirected()   {this->SetSearchMode(SEARCH_UNDIRECTED); };
  void SetSearchModeToReverseDirected() {this->SetSearchMode(SEARCH_REVERSED); };
	
protected:
  vtkSelectReachable();
  ~vtkSelectReachable();
   
  void Execute();
	
	int SearchMode;
  vtkIdType StartNode;
  int StartNodeSet;
	char *SelectorName;
	int MaxDistance;
	int MaxAtInfinity;
	
private:
  vtkSelectReachable(const vtkSelectReachable&) {};
  void operator=(const vtkSelectReachable&) {};
};

#endif	/* __vtkSelectReachable_h */

