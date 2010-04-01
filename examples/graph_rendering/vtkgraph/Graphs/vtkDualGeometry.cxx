/*
 *	File: vtkDualGeometry.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkDualGeometry.h"
#include "vtkObjectFactory.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include <math.h>

vtkStandardNewMacro(vtkDualGeometry);
vtkCxxRevisionMacro(vtkDualGeometry, "1.0");

vtkDualGeometry::vtkDualGeometry()
{
	this->Mode = NULL;
	this->ModeArraySize = 0;
	this->DualPointPosition = DUAL_AT_MIDPOINT;
}

vtkDualGeometry::~vtkDualGeometry()
{
	if (this->Mode)
		{
		for (int i = 0; i < this->ModeArraySize; i++)
			{
			delete [] this->Mode[i].name;
			}
		delete [] this->Mode;
		}
}


typedef struct _vtkDualDataOption_s _vtkDDO;

void vtkDualGeometry::SetNodeDataMode(char *name, int mode)
{
	int i = 0;
	_vtkDualDataOption_s *array;
	
	while (i < this->ModeArraySize && strcmp(this->Mode[i].name, name))
		{
		i++;
		}
	if (i == this->ModeArraySize)
		{
		array = new _vtkDDO[this->ModeArraySize+1];
		for (int j = 0; j < this->ModeArraySize; j++)
			{
			array[j] = this->Mode[j];
			}
		this->Mode = array;
		this->ModeArraySize++;
		this->Mode[i].name = new char[strlen(name)+1];
		strcpy(Mode[i].name, name);
		}
	this->Mode[i].mode = mode;
}


//--------------------------------------------------

void vtkDualGeometry::Execute()
{
  vtkGraph    *input  = this->GetInput();
  vtkPolyData *output = this->GetOutput();
  vtkPoints *points, *dualPoints;
  vtkFloatArray *vectors;
  vtkIdType src, dst;
	vtkIdType nrEdges;
	vtkDataArray *nodeArray, *dualArray;
	
	double p[3], q[3], m[3];	
	double *srct, *dstt;
	double norm;
	
  vtkDebugMacro(<< "DualGeometry executing");

	points = input->GetLayout();
	nrEdges = input->GetNumberOfEdges();
	dualPoints = vtkPoints::New();
	dualPoints->SetNumberOfPoints(nrEdges);
	
	vectors = vtkFloatArray::New();
	vectors->SetNumberOfComponents(3);
	vectors->SetNumberOfTuples(nrEdges);
	vectors->SetName("direction");
	
	// Each input edge is mapped to a point in the output
	// polydata, specifically the mid-point of the edge.
	// Output point data is extended with an array of
	// unit vectors, one per point, giving the direction
	// of the corresponding edge.

	for (vtkIdType i = 0; i < nrEdges; i++)
		{
		input->GetEdge(input->GetEdgeId(i), src, dst); 
		input->GetPosition(src, p);
		input->GetPosition(dst, q);
		norm = 0;
		for (int j = 0; j < 3; j++)
			{
			m[j] = (p[j] + q[j])/2.0;
			norm += (p[j] - q[j])*(p[j] - q[j]);
			}
		norm = sqrt(norm);
		switch(this->DualPointPosition)
			{
			case DUAL_AT_MIDPOINT:
				{
				dualPoints->SetPoint(i, m);
				}
				break;
			case DUAL_AT_SOURCE:
				{
				dualPoints->SetPoint(i, p);
				}
				break;
			case DUAL_AT_TARGET:
				{
				dualPoints->SetPoint(i, q);
				}
				break;
			default:
				{
				vtkErrorMacro(<<"Unknown dual-point position mode.");
				}
				break;
			}
		if (norm == 0)
			{
			vtkWarningMacro(<<"DualPoints: 0-length edge in input, ignored.");
			}
		else
			{
			for (int c = 0; c < 3; c++)
				{
				vectors->SetComponent(i, c, (q[c]-p[c])/norm);
				}
			}
		}
		
	// Set output polydata points to the dual points. Copy edge
	// data to output cell data, and add the vectors.
  output->SetPoints(dualPoints);
  output->GetPointData()->ShallowCopy(input->GetEdgeData() );
	output->GetPointData()->SetVectors(vectors);
  
	// Process any node data arrays that are to be transferred
	// as edge data using a declared mode.
	for (int mi = 0; mi < this->ModeArraySize; mi++)
		{
		if (this->Mode[mi].mode == MODE_IGNORE)
			{
			continue;
			}
		nodeArray = input->GetNodeData()->GetArray(this->Mode[mi].name);
		if (!nodeArray)
			{
			vtkWarningMacro(<<"DualGeometry: missing node data array");
			continue;
			}
		dualArray = vtkDataArray::CreateDataArray(nodeArray->GetDataType());
		dualArray->SetNumberOfComponents(nodeArray->GetNumberOfComponents());
		dualArray->SetNumberOfTuples(nrEdges);
		dualArray->SetName(nodeArray->GetName());
		switch(this->Mode[mi].mode)
			{
			case MODE_COPY_SOURCE:
				{
				srct = new double[dualArray->GetNumberOfComponents()];
				for (vtkIdType si = 0; si < nrEdges; si++)
					{
					nodeArray->GetTuple(
							input->GetNodeIndex(
								input->GetSource(input->GetEdgeId(si))), srct);
					dualArray->SetTuple(si, srct);
					}
				delete [] srct;
				}
				break;
			case MODE_COPY_TARGET:
				{
				dstt = new double[dualArray->GetNumberOfComponents()];
				for (vtkIdType di = 0; di < nrEdges; di++)
					{
					nodeArray->GetTuple(
							input->GetNodeIndex(
								input->GetTarget(input->GetEdgeId(di))), dstt);
					dualArray->SetTuple(di, dstt);
					}
				delete [] dstt;
				}
				break;
			case MODE_AVERAGE:
				{
				srct = new double[dualArray->GetNumberOfComponents()];
				dstt = new double[dualArray->GetNumberOfComponents()];
				for (vtkIdType ai = 0; ai < nrEdges; ai++)
					{
					nodeArray->GetTuple(
						input->GetNodeIndex(
							input->GetSource(input->GetEdgeId(ai))), srct);
					nodeArray->GetTuple(
						input->GetNodeIndex(
							input->GetTarget(input->GetEdgeId(ai))), dstt);
					for (int tc = 0; tc < dualArray->GetNumberOfComponents(); tc++)
						{
						srct[tc] = (srct[tc] + dstt[tc]) / 2.0;
						}
					dualArray->SetTuple(ai, srct);
					}
				delete [] srct;
				delete [] dstt;
				}
				break;
			default:
				// NULL-OP
				break;
			}
		output->GetPointData()->AddArray(dualArray);
		dualArray->Delete();
		}
	
	dualPoints->Delete();
	vectors->Delete();	
	vtkDebugMacro(<< "DualGeometry done.\n");
}


void vtkDualGeometry::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "DualGeometry Filter.\n";
}
