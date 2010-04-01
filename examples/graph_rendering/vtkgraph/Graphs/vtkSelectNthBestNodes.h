/*
 *	File: vtkSelectNthBestNodes.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkSelectNthBestNodes - select nodes in order of property value.
// .SECTION Description
// vtkSelectNthBestNodes selects nodes of a given graph by ordering
// the nodes with respect to a node property, and then selecting nodes
// in one of two ways:
// 1.  taking the n "best" nodes from the sequence; or
// 2.  selecting the "best" fraction of the list.
//
// Nodes are marked as selected via a bit-array in the output node data.  
// The property on which nodes are selected must be held in a 
// float-array.  This class is expected to be used in conjunction with 
// a filter such as vtkSubgraphFilter that uses a selection to extract 
// a subgraph. 
//
// .SECTION See Also
// vtkSubgraphFilter vtkGraph

#ifndef __vtkSelectNthBestNodes_h
#define __vtkSelectNthBestNodes_h

#include "vtkGraphToGraphFilter.h"

//BTX
// Pair used for sorting nodes
struct _vtkPair_s
	{
	vtkIdType index;
	float value;
	};
//ETX
	
class VTK_EXPORT vtkSelectNthBestNodes : public vtkGraphToGraphFilter
{
public:
  static vtkSelectNthBestNodes *New();
  vtkTypeMacro(vtkSelectNthBestNodes,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);


  //---------------------------------------------------------
	// Filter parameters.
	
	// Description:
	// Set/get the name of the array used to mark nodes as selected.  
	vtkSetStringMacro(SelectorName);
	vtkGetStringMacro(SelectorName);
	
	// Description:
	// Set/get the name of the array used as the property.  
	vtkSetStringMacro(PropertyName);
	vtkGetStringMacro(PropertyName);
	
	// Description:
	// Set the mode of selection to either select a proportion 
	// of the nodes, or to select a fixed number of nodes.
	vtkSetMacro(UseProportion, int);
	vtkGetMacro(UseProportion, int);
	void SetSelectionModeToSelectProportion() { this->SetUseProportion(1); };
	void SetSelectionModeToSelectFirstN() { this->SetUseProportion(0); };
	
	// Description:
	// For selecting a proportion of nodes, set/get the propertion.
	vtkSetMacro(Proportion, float);
	vtkGetMacro(Proportion, float);
	
	// Description:
	// For selecting a fixed number of nodes, set/get the number.
	vtkSetMacro(NumberToSelect, vtkIdType);
	vtkGetMacro(NumberToSelect, vtkIdType);
	
	
protected:
  vtkSelectNthBestNodes();
  ~vtkSelectNthBestNodes();
   
  void Execute();
	
	float Proportion; 					// Proportion of nodes to select
	vtkIdType NumberToSelect; 	// Number of nodes to select
	int UseProportion;					// Selection mode.
	
	char *PropertyName; 				// name of input node property.
	char *SelectorName; 				// name of selector property in output.

	//BTX	
	// Description:
	// Auxilliary functions to implement merge-sort.
	void Merge(
		struct _vtkPair_s *array,
		struct _vtkPair_s *temp,
		vtkIdType leftPos,
		vtkIdType rightPos,
		vtkIdType rightEnd);
	
	void MergeSort(
		struct _vtkPair_s *array,
		struct _vtkPair_s *temp,
		vtkIdType left,
		vtkIdType right);
	//ETX
	
private:
  vtkSelectNthBestNodes(const vtkSelectNthBestNodes&) {};
  void operator=(const vtkSelectNthBestNodes&) {};
};

#endif	/* __vtkSelectNthBestNodes_h */

