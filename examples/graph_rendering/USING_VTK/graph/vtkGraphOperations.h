/*
 *	File: vtkGraphOperations.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 *
 *	Change Log:
 *	2003/12/12:	Modified implementation so that data arrays of the
 *              union are those common to both inputs, while data
 *              arrays of the intersection are those that occur in
 *              either graph.  Similarly, the filter now manages 
 *              string arrays appropriately.
 */
// .NAME vtkGraphOperations - perform set-based operations on graphs.
// .SECTION Description
// vtkGraphOperations implements the union, intersection and
// difference operations on graphs.  It assumes that the nodes and
// edges of two graphs can be equated using their node ids, i.e.
// if nodes of the two graphs have the same node id, they are the
// same node, and likewise for edges.  Node operations restrict
// the effect of edge operations: for example, given inputs
// GA(VA, EA) and GB(VB, EB), setting the node operation to 
// intersection and the edge operation to union will give a graph
// whose nodes are VA intersect VB, but whose edges are the 
// subset of (EA union EB) that connect nodes in (VA intersect VB).
//
// .SECTION See Also
// vtkGraph

#ifndef __vtkGraphOperations_h
#define __vtkGraphOperations_h

#include "vtkGraphToGraphFilter.h"
#include "vtkDataArrayCollection.h"
#include "vtkFieldData.h"

#define VTK_COPY_A 0
#define VTK_COPY_B 1
#define VTK_UNION 2
#define VTK_INTERSECT 3
#define VTK_DIFFERENCE 4

class VTK_EXPORT vtkGraphOperations : public vtkGraphToGraphFilter 
{
public:
  static vtkGraphOperations *New();
  vtkTypeRevisionMacro(vtkGraphOperations,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Set the graphs on which the operations are performed.
	// Note that graph A is identified with the filter input.	
  void SetGraphA(vtkGraph *graph)
  { this->vtkProcessObject::SetNthInput(0, graph); };
	
  void SetGraphB(vtkGraph *graph)
  { this->vtkProcessObject::SetNthInput(1, graph); };

	// Description:
	// Set/get the node operation.
	vtkSetMacro(NodeOperation, int);
	vtkGetMacro(NodeOperation, int);
	void SetNodeOperation(char *opName);
	
	// Description:
	// Set/get the edge operation.
	vtkSetMacro(EdgeOperation, int);
	vtkGetMacro(EdgeOperation, int);
	void SetEdgeOperation(char *opName);
	
	
protected:
  vtkGraphOperations();
  ~vtkGraphOperations();

  void Execute();
	
	int NodeOperation;
	int EdgeOperation;
	
	//BTX
	struct _vtkOpEntry_s
		{
		char *opName;
		int opCode;
		};
	static _vtkOpEntry_s GraphOps[];
	//ETX
	
	void FieldIntersect(
		vtkFieldData *dataA, 
		vtkFieldData *dataB,
		vtkFieldData *dataO,
		vtkDataArrayCollection *set,
		double **tuple);
		
	void FieldUnion(
		vtkFieldData *dataA, 
		vtkFieldData *dataB,
		vtkFieldData *dataO,
		vtkDataArrayCollection *fromA, 
		vtkDataArrayCollection *fromB,
		double **tuple);

	void EnableStringArrays(
		vtkGraph *gA,
		vtkGraph *gB,
		vtkGraph *gO,
		int location,
		int mode);
		
private:
  vtkGraphOperations(const vtkGraphOperations&) {};
  void operator=(const vtkGraphOperations&) {};
};

#endif	/* __vtkGraphOperations_h */

