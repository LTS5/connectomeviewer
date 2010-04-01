/*
 *	File: vtkUnstructuredGridToGraph.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkUnstructuredGridToGraph.h"
#include "vtkObjectFactory.h"
#include "vtkIdTypeArray.h"
#include "vtkIntArray.h"
#include "vtkIdList.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkPoints.h"
#include "vtkCellType.h"

vtkStandardNewMacro(vtkUnstructuredGridToGraph);
vtkCxxRevisionMacro(vtkUnstructuredGridToGraph, "1.0");

vtkUnstructuredGridToGraph::vtkUnstructuredGridToGraph() 
{
  this->NumberOfRequiredInputs = 1;
}

//-------------------------------------------------------

void vtkUnstructuredGridToGraph::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//-------------------------------------------------------

// Specify the input data or filter.
void vtkUnstructuredGridToGraph::SetInput(vtkUnstructuredGrid *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

vtkUnstructuredGrid *vtkUnstructuredGridToGraph::GetInput()
{
  if (this->NumberOfInputs < 1)
    {
    return NULL;
    }
  
  return (vtkUnstructuredGrid *)(this->Inputs[0]);
}

void vtkUnstructuredGridToGraph::ComputeInputUpdateExtents( 
	vtkDataObject *output)
{
  this->vtkGraphSource::ComputeInputUpdateExtents(output);

  if (!this->GetInput())
    {
    return;
    }
  // assume that we cannot handle more than the requested extent.
  this->GetInput()->RequestExactExtentOn();
}

//-------------------------------------------------------

void vtkUnstructuredGridToGraph::Execute()
{
  vtkUnstructuredGrid *input  = this->GetInput();
	vtkGraph            *output = this->GetOutput();

	vtkIdType nrEdges;
	vtkIdType nrBends;
	vtkIdType nrNodes;
	vtkIdType size, i;
	vtkIdType src, dst;
	vtkIdType edge, node;
	vtkIdType nrPoints, *cellPoints;

  vtkPoints *points, *bendPoints, *nodePoints; 
	vtkFieldData *nodeData;
	vtkFieldData *edgeData;

	vtkIntArray  *nodeID = NULL;	// map from point to node id
	vtkIntArray  *edgeID = NULL;	// map from cell to edge id
	vtkIntArray  *degree = NULL;	// degree of each node

  vtkDebugMacro(<< "UnstructuredGridToGraph executing");
	if (!input)
		{
		return;
		}
	nodeData = input->GetPointData();
	edgeData = input->GetCellData();

  nodeID = vtkIntArray::SafeDownCast(nodeData->GetArray("nodeID"));
  edgeID = vtkIntArray::SafeDownCast(edgeData->GetArray("edgeID"));
  degree = vtkIntArray::SafeDownCast(nodeData->GetArray("degree"));

	if (!nodeID || !edgeID || !degree)
		{
		vtkErrorMacro(<<"Require node/edgeID arrays and degree.");
		return;
		}

	// Count number of nodes, edges and bends.
	size = nodeID->GetNumberOfTuples();
	
	// Check through and count nodes.
	for (i = 0; i < size && nodeID->GetValue(i) >= 0; i++);
	nrNodes = i;

	// Remaining entries in nodeID array should be bends
	for (; i < size && nodeID->GetValue(i) < 0; i++);
	nrBends = i - nrNodes;

	// Confirm that all entries in nodeID array are accounted for.
	if (nrBends + nrNodes < size)
		{
		vtkErrorMacro(<<"Improper organization of nodes and bends");
		return;
		}
	if (degree->GetNumberOfTuples() != nrNodes)
		{
		vtkErrorMacro(<<"Node degrees do not match.");
		}
	if (input->GetPoints()->GetNumberOfPoints() != size)
		{
		vtkErrorMacro(<<"Points do not match nodes/bends");
		}

	nrEdges = input->GetNumberOfCells();
	
	output->SetInitialNodeSize(nrNodes);
	output->SetInitialEdgeSize(nrEdges);
	if (nrBends > 0)
		{
		bendPoints = vtkPoints::New();
		bendPoints->SetNumberOfPoints(nrBends);
		output->SetBends(bendPoints);
		bendPoints->Delete();
		}
	if (nrNodes > 0)
		{
		nodePoints = vtkPoints::New();
		nodePoints->SetNumberOfPoints(nrNodes);
		output->SetLayout(nodePoints);
		nodePoints->Delete();
		}
	points = input->GetPoints();

	for (i = 0; i < nrNodes; i++)
		{
		node = nodeID->GetValue(i);
		output->CreateNode(node);
		output->SetEdgeArraySize(node, degree->GetValue(i));
		output->SetPosition(node, points->GetPoint(i));
		}

	for (i = 0; i < nrBends; i++)
		{
		output->CreateBend();
		output->SetBendPosition(i, points->GetPoint(nrNodes+i));
		}

	for (i = 0; i < nrEdges; i++)
		{
		input->GetCellPoints(i, nrPoints, cellPoints);
		if (nrPoints < 2)
			{
			vtkErrorMacro(<<"Inconsistent cell "<<i<<", skipping.");
			continue;
			}
		edge = edgeID->GetValue(i);
		src = nodeID->GetValue(cellPoints[0]);
		dst = nodeID->GetValue(cellPoints[nrPoints-1]);
		output->CreateEdge(edge, src, dst);
		for (int b = 0; b < nrPoints-2; b++)
			{
			output->InsertBend(edge, b, cellPoints[b+1]-nrNodes);
			}
		}

	output->GetEdgeData()->DeepCopy(input->GetCellData());
	output->GetEdgeData()->RemoveArray("edgeID");
	output->GetNodeData()->DeepCopy(input->GetPointData());
	output->GetNodeData()->RemoveArray("nodeID");
	output->GetNodeData()->RemoveArray("degree");

	vtkDebugMacro(<< "UnstructuredGridToGraph done.\n");
}



