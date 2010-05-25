/*
 *	File: vtkEdgeGeometry.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details. 
 *
 *	Change history:
 *	2003/11/20: added flags for outputting bends and edge ids.
 *	2003/11/15:	major change to take account of edge paths.
 *	2003/11/13: added capability to reject 0-length edges.
 *							This uses new NonZeroLength flag. 
 */

#include "vtkEdgeGeometry.h"
#include "vtkObjectFactory.h"
#include "vtkIdTypeArray.h"
#include "vtkIdIterator.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkPoints.h"

vtkStandardNewMacro(vtkEdgeGeometry);
vtkCxxRevisionMacro(vtkEdgeGeometry, "1.11");

vtkEdgeGeometry::vtkEdgeGeometry()
{
	this->NonZeroLength = 1;
	this->OutputBends   = 1;
	this->OutputEdgeIds = 0;
}

//--------------------------------------------------

void vtkEdgeGeometry::Execute()
{
  vtkGraph    *input  = this->GetInput();
  vtkPolyData *output = this->GetOutput();

  vtkIdType src, dst;
	vtkIdType nrEdges;
	vtkIdType nrBends;
	vtkIdType nrNodes;
	vtkIdType nrNodeComponents = 0;
	vtkIdType nrEdgeComponents = 0;
	vtkIdType cell;
	vtkIdType edge;
	vtkIdType bend;

	vtkIdIterator *bends;

  vtkCellArray *newLines;

  vtkPoints *points, *bendPoints;
	vtkPointData *nodeData;
	vtkCellData *edgeData;

	double *edgeDataTuple; 		// for copying data to new edges.
	double *nodeDataTupleSrc;
	double *nodeDataTupleDst; 	// for copying data to (new) points.
	double *nodeDataTupleNew;
	float t;									// interpolation of data fields
	float step;
  
	double sp[3], dp[3];
	
	vtkIdTypeArray *edgeIds = NULL;	// map from line to edge ids

  vtkDebugMacro(<< "EdgeGeometry executing");
	if (!input)
		{
		return;
		}

	nrNodes = input->GetNumberOfNodes();
	nrEdges = input->GetNumberOfEdges();
	nrBends = input->GetNumberOfBends();

	nodeData = output->GetPointData();
	edgeData = output->GetCellData();

	edgeDataTuple = NULL;
	nodeDataTupleSrc = NULL;
	nodeDataTupleDst = NULL;
	nodeDataTupleNew = NULL;

	// Check if edgeIds are needed on output, if so allocate
	// the array to hold these.  Allocate size pessimistically
	// to save re-allocation.
	if (this->OutputEdgeIds)
		{
		edgeIds = vtkIdTypeArray::New();
		edgeIds->SetNumberOfTuples(nrEdges + nrBends);
		edgeIds->SetName("edgeIds");
		}

	// If the input graph has bends, and we are not ignoring
	// them, new points will have to be created in the output, 
	// as bend points are added to the node points.
	if (this->OutputBends && nrBends > 0)
		{
		// Copy the node points, then append the bend points.
		points = vtkPoints::New();
		points->DeepCopy(input->GetLayout());

		bendPoints = input->GetBends();
		for (int b = 0; b < nrBends; b++)
			{
			points->InsertPoint(nrNodes+b, bendPoints->GetPoint(b));
			}

		// Output point data will be a superset of input node data.
		nodeData->DeepCopy(input->GetNodeData());

		// Output edge data cannot be copied from input, as those
		// edges in the input with bends will not appear in the
		// output.  Instead, copy the structure; tuples will be
		// handled on an edge-by-edge basis.
		nrEdgeComponents = input->GetEdgeData()->GetNumberOfComponents();
		if (nrEdgeComponents > 0)
			{
			edgeDataTuple = new double[nrEdgeComponents];
			edgeData->CopyStructure(input->GetEdgeData());
			}

		nrNodeComponents = input->GetNodeData()->GetNumberOfComponents();
		if (nrNodeComponents > 0)
			{
			nodeDataTupleSrc = new double[nrNodeComponents];
			nodeDataTupleDst = new double[nrNodeComponents];
			nodeDataTupleNew = new double[nrNodeComponents];
			}
		}
	else
		{
		points = input->GetLayout();
		nodeData->ShallowCopy(input->GetNodeData());
		edgeData->ShallowCopy(input->GetEdgeData());
		}

	// Each graph edge maps to 3 entries in the cell array;
	// cell array space is pre-allocated.  Each graph bend
	// maps (in the worst case) to two edges, i.e. 6 cells.
  newLines = vtkCellArray::New();
	newLines->Allocate(nrEdges*3 + nrBends*6, 1000);
	
	// Copy edge at index i to ith cell.
	for (vtkIdType i = 0; i < nrEdges; i++)
		{
		edge = input->GetEdgeId(i);
		input->GetEdge(edge, src, dst); 
		src = input->GetNodeIndex(src);
		dst = input->GetNodeIndex(dst);

		// Get the data associated with this edge.  In the
		// event that we have bends, the data of all edges
		// will be copied over manually.
		if (edgeDataTuple)
			{
			input->GetEdgeData()->GetTuple(i, edgeDataTuple);
			}
		if (this->NonZeroLength)
			{
			points->GetPoint(src, sp);
			points->GetPoint(dst, dp);
			if (sp[0] == dp[0] && sp[1] == dp[1] && sp[2] == dp[2])
				{
				// Do not output this edge as a line, it has 0-length
				continue;
				}
			} 
		nrBends = input->GetNumberOfBends(edge);
		if (this->OutputBends && nrBends > 0)
			{
			// Iterate through bends, adding an edge for each segment.
			// Note that bend points need to be added to the end of 
			// the points marking node positions, as the output
			// polydata set only has one set of points.  Similarly,
			// point and cell data has to be generated for the new
			// lines and points.  The former is done by interpolating
			// between the data values of the source and target nodes,
			// the latter by simply copying the data for that edge,
			bends = input->GetBends(edge);
			
			if (nodeDataTupleNew)
				{
				input->GetNodeData()->GetTuple(src, nodeDataTupleSrc);
				input->GetNodeData()->GetTuple(dst, nodeDataTupleDst);
				}
			step = 1.0 / (nrBends+1);
			t = step;

			// Line from source to 1st bend point
			cell = newLines->InsertNextCell(2);
			newLines->InsertCellPoint(src);
			bend = nrNodes + bends->GetNext();
			newLines->InsertCellPoint(bend);
			if (this->OutputEdgeIds)
				{
				edgeIds->InsertValue(cell, edge);
				}

			if (nodeDataTupleNew)
				{
				for (int j1 = 0; j1 < nrNodeComponents; j1++)
					{
					nodeDataTupleNew[j1] = 
						t     * nodeDataTupleSrc[j1] + 
						(1-t) * nodeDataTupleDst[j1];
					}
				nodeData->InsertTuple(bend, nodeDataTupleNew);
				}
			if (edgeDataTuple)
				{
				edgeData->InsertTuple(cell, edgeDataTuple);
				}

			while (bends->HasNext())
				{
				// Line from previous bend point to next
				cell = newLines->InsertNextCell(2);
				newLines->InsertCellPoint(bend);
				bend = nrNodes + bends->GetNext();
				newLines->InsertCellPoint(bend);
				if (this->OutputEdgeIds)
					{
					edgeIds->InsertValue(cell, edge);
					}

				t += step;
				if (nodeDataTupleNew)
					{
					for (int j2 = 0; j2 < nrNodeComponents; j2++)
						{
						nodeDataTupleNew[j2] = 
							t     * nodeDataTupleSrc[j2] + 
							(1-t) * nodeDataTupleDst[j2];
						}
					nodeData->InsertTuple(bend, nodeDataTupleNew);
					}
				if (edgeDataTuple)
					{
					edgeData->InsertTuple(cell, edgeDataTuple);
					}
				}	
			// Line from final bend point to target
			cell = newLines->InsertNextCell(2);
			newLines->InsertCellPoint(bend);
			newLines->InsertCellPoint(dst);
			if (edgeDataTuple)
				{
				edgeData->InsertTuple(cell, edgeDataTuple);
				}
			if (this->OutputEdgeIds)
				{
				edgeIds->InsertValue(cell, edge);
				}
			bends->Delete();
			}
		else
			{
			cell = newLines->InsertNextCell(2);
			newLines->InsertCellPoint(src);
			newLines->InsertCellPoint(dst);
			if (edgeDataTuple)
				{
				edgeData->InsertTuple(cell, edgeDataTuple);
				}
			if (this->OutputEdgeIds)
				{
				edgeIds->InsertValue(cell, edge);
				}
			}
		}
		
	// Set lines of output polydata, and copy layout, node
	// and edge data to the points, point and cell data.
  output->SetLines(newLines);
  output->SetPoints(points);
	if (this->OutputEdgeIds)
		{
		output->GetCellData()->AddArray(edgeIds);
		}
  newLines->Delete();

	vtkDebugMacro(<< "EdgeGeometry done.\n");
}


void vtkEdgeGeometry::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "EdgeGeometry Filter.\n";
	os << indent << "Output Bends:    " << this->OutputBends << "\n";
	os << indent << "Output Edge Ids: " << this->OutputEdgeIds << "\n";
	os << indent << "Non-zero length: " << this->NonZeroLength << "\n";
}

