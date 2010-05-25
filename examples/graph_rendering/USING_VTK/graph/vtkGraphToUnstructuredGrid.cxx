/*
 *	File: vtkGraphToUnstructuredGrid.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkGraphToUnstructuredGrid.h"
#include "vtkUnstructuredGrid.h"
#include "vtkObjectFactory.h"
#include "vtkIdTypeArray.h"
#include "vtkIntArray.h"
#include "vtkIdIterator.h"
#include "vtkIdList.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkPoints.h"
#include "vtkCellType.h"

vtkStandardNewMacro(vtkGraphToUnstructuredGrid);
vtkCxxRevisionMacro(vtkGraphToUnstructuredGrid, "1.0");

vtkGraphToUnstructuredGrid::vtkGraphToUnstructuredGrid() 
{
  this->NumberOfRequiredInputs = 1;
  this->InterpolateNodeData = 0;
}

//-------------------------------------------------------

void vtkGraphToUnstructuredGrid::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

//-------------------------------------------------------

// Specify the input data or filter.
void vtkGraphToUnstructuredGrid::SetInput(vtkGraph *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

vtkGraph *vtkGraphToUnstructuredGrid::GetInput()
{
  if (this->NumberOfInputs < 1)
    {
    return NULL;
    }
  
  return (vtkGraph *)(this->Inputs[0]);
}

void vtkGraphToUnstructuredGrid::ComputeInputUpdateExtents( 
	vtkDataObject *output)
{
  this->vtkUnstructuredGridSource::ComputeInputUpdateExtents(output);

  if (!this->GetInput())
    {
    return;
    }
  // assume that we cannot handle more than the requested extent.
  this->GetInput()->RequestExactExtentOn();
}

//-------------------------------------------------------

void vtkGraphToUnstructuredGrid::Execute()
{
  vtkGraph            *input  = this->GetInput();
  vtkUnstructuredGrid *output = this->GetOutput();

  vtkIdType src, dst;
	vtkIdType nrEdges;
	vtkIdType nrBends;
	vtkIdType nrNodes;
	vtkIdType nrNodeComponents = 0;
	vtkIdType edge;
	vtkIdType node;
	vtkIdType *bends;

  vtkPoints *points, *bendPoints;
	vtkPointData *pointData;
	vtkIdList *ids;

	double *nodeDataTupleSrc;
	double *nodeDataTupleDst; 	// for copying data to (new) points.
	double *nodeDataTupleNew;
	float t;									// interpolation of data fields
	float step;
  	
	vtkIdTypeArray *nodeID = NULL;	// map from point to node id
	vtkIdTypeArray *edgeID = NULL;	// map from cell to edge id
	vtkIntArray    *degree = NULL; // map from point to degree (of node)

  vtkDebugMacro(<< "GraphToUnstructuredGrid executing");
	if (!input)
		{
		return;
		}

	nrNodes = input->GetNumberOfNodes();
	nrEdges = input->GetNumberOfEdges();
	nrBends = input->GetNumberOfBends();

	nodeDataTupleSrc = NULL;
	nodeDataTupleDst = NULL;
	nodeDataTupleNew = NULL;

	ids = vtkIdList::New();

	nodeID = vtkIdTypeArray::New();
	nodeID->SetNumberOfTuples(nrNodes + nrBends);
	nodeID->SetName("nodeID");

	edgeID = vtkIdTypeArray::New();
	edgeID->SetNumberOfTuples(nrEdges);
	edgeID->SetName("edgeID");

	degree = vtkIntArray::New();
	degree->SetNumberOfTuples(nrNodes);
	degree->SetName("degree");

	// If there are no bends, output points and input
	// points will be the same (and can be shared); otherwise,
	// we copy the input points and append the bend points.
	if (nrBends > 0)
		{
		points = vtkPoints::New();
		points->DeepCopy(input->GetLayout());
		bendPoints = input->GetBends();
		for (int b = 0; b < nrBends; b++)
			{
			points->InsertPoint(nrNodes+b, bendPoints->GetPoint(b));
			}
		}
	else
		{
		points = input->GetLayout();
		// Pad out with points to match number of nodes. Since
		// this changes points, we must first make a copy.
		if (points->GetNumberOfPoints() < nrNodes)
			{
			points = vtkPoints::New();
			points->DeepCopy(input->GetLayout());
			for (int p = points->GetNumberOfPoints(); p < nrNodes; p++)
				{
				points->InsertPoint(p, 0.0, 0.0, 0.0);
				}
			}
		}

	if (nrBends > 0 && this->InterpolateNodeData)
		{
		pointData = output->GetPointData();
		pointData->DeepCopy(input->GetNodeData());
		}
	else
		{
		output->GetPointData()->ShallowCopy(input->GetNodeData());
		pointData = NULL;
		}
	output->GetCellData()->ShallowCopy(input->GetEdgeData());

	// If we need to add more points, allocate space for
	// carrying out the interpolation.
	if (pointData)
		{
		nrNodeComponents = input->GetNodeData()->GetNumberOfComponents();
		if (nrNodeComponents > 0)
			{
			nodeDataTupleSrc = new double[nrNodeComponents];
			nodeDataTupleDst = new double[nrNodeComponents];
			nodeDataTupleNew = new double[nrNodeComponents];
			}
		else
			{
			nodeDataTupleNew = NULL;
			}
		}

	// Assign Ids and degrees to points
	for (vtkIdType n = 0; n < nrNodes; n++)
		{
		node = input->GetNodeId(n);
		nodeID->SetValue(n, node);
		degree->SetValue(n, input->GetDegree(node));
		}
	for (vtkIdType b = 0; b < nrBends; b++)
		{
		nodeID->SetValue(nrNodes + b, -1);
		}

	// Allocate initial space for grid.  This call is required
	// by vtkUnstructuredGrid.
	output->Allocate(nrEdges, 10);

	// Process each edge in turn, constructing a cell for the edge,
	// and if necessary interpolating any bend points.
	for (vtkIdType i = 0; i < nrEdges; i++)
		{
		edge = input->GetEdgeId(i);
		input->GetEdge(edge, src, dst); 
		src = input->GetNodeIndex(src);
		dst = input->GetNodeIndex(dst);

		// Build up a list of IDs making up the edge.
		ids->Initialize();
		ids->InsertNextId(src);
		nrBends = input->GetNumberOfBends(edge);
		bends = input->GetBendArray(edge);
		for (int b = 0; b < nrBends; b++)
			{
			ids->InsertNextId(nrNodes + bends[b]);
			}
		ids->InsertNextId(dst);

		// Create the cell corresponding to the edge.  If there
		// were any bends in the edge, it becomes a VTK_POLYLINE
		// cell, otherwise it is a VTL_LINE.
		output->InsertNextCell((nrBends == 0 ? VTK_LINE : VTK_POLY_LINE), ids);

		// See if there is node data to interpolate.
		if (nodeDataTupleNew)
			{
			input->GetNodeData()->GetTuple(src, nodeDataTupleSrc);
			input->GetNodeData()->GetTuple(dst, nodeDataTupleDst);
			step = 1.0 / (nrBends+1);
			t = step;
			for (int d = 0; d < nrBends; d++)
				{
				for (int j1 = 0; j1 < nrNodeComponents; j1++)
					{
					nodeDataTupleNew[j1] = 
						t     * nodeDataTupleSrc[j1] + 
						(1-t) * nodeDataTupleDst[j1];
					}
				pointData->InsertTuple(nrNodes + bends[d], nodeDataTupleNew);
				t += step;
				}
			}

		// Make a record of the edge Id
		edgeID->SetValue(i, edge);
		}

  output->SetPoints(points);
	output->GetPointData()->AddArray(nodeID);
	output->GetPointData()->AddArray(degree);
	output->GetCellData()->AddArray(edgeID);

	ids->Delete();
	if (nodeDataTupleNew)
		{
		delete [] nodeDataTupleNew;
		delete [] nodeDataTupleSrc;
		delete [] nodeDataTupleDst;
		}

	vtkDebugMacro(<< "GraphToUnstructuredGrid done.\n");
}



