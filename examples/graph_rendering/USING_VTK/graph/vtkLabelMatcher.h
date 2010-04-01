/*
 *	File: vtkLabelMatcher.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkLabelMatcher - match nodes of two graphs based on labels.
// .SECTION Description
// vtkLabelMatcher maps an input graph to an output graph, but
// re-identifies nodes of the input graph so that the labels of
// the input match up with the labels of a second graph.  So if
// the input graph has the following nodes and labels:
//
// 1 [alpha]  2 [beta]  3 [gamma]
//
// and the second graph has the following nodes/labels
//
// 5 [beta] 10 [gamma] 15 [alpha]  20 [delta]
//
// then the output graph will have the following node ids/labels:
//
// 5 [beta] 10 [gamma] 15 [alpha]
//
// Each edge of the input graph will be copied to the output,
// with the source/target nodes renamed appropriately.  If the
// input contains a node label not present in the second graph,
// this node is copied through unchanged.  If this results in
// a clash with a node that has been renamed, then an error is
// reported.
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkLabelMatcher_h
#define __vtkLabelMatcher_h

#include "vtkGraphToGraphFilter.h"
#include "vtkStringTable.h"

class VTK_EXPORT vtkLabelMatcher : public vtkGraphToGraphFilter 
{
public:
  static vtkLabelMatcher *New();
  vtkTypeRevisionMacro(vtkLabelMatcher,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Set the graph that will be used to determine node ids	
  void SetGraphToMatch(vtkGraph *graph)
  { this->vtkProcessObject::SetNthInput(1, graph); };

	// Description:
	// Set the arrays that are used to label nodes
	vtkSetStringMacro(InputLabelArrayName);
	vtkSetStringMacro(MatchLabelArrayName);
	
	// Description:
	// Set the string table used by the graph to match with
	vtkSetObjectMacro(InputNodeNames, vtkStringTable);
	vtkSetObjectMacro(MatchNodeNames, vtkStringTable);
	
protected:
  vtkLabelMatcher();
  ~vtkLabelMatcher();

  void Execute();
	
	vtkStringTable *InputNodeNames; // node labels of input graph
	vtkStringTable *MatchNodeNames; // node labels of graph to match
	
	char *InputLabelArrayName;
	char *MatchLabelArrayName;

private:
  vtkLabelMatcher(const vtkLabelMatcher&) {};
  void operator=(const vtkLabelMatcher&) {};
};

#endif	/* __vtkLabelMatcher_h */

