/*
 *	File: vtkHDELayout.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkHDELayout.h"
#include "vtkObjectFactory.h"
#include "vtkGraph.h"
#include "vtkFloatArray.h"
#include "vtkIdIterator.h"
#include "vtkPoints.h"


//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkHDELayout);
vtkCxxRevisionMacro(vtkHDELayout, "1.0");

vtkHDELayout::vtkHDELayout()
{
	this->M = 50;
	this->C = 3;
}

//----------------------------------------------------------------------------

void vtkHDELayout::PositionFrom(
	int axis,
	double *coord,
	vtkGraph *graph)
{
	vtkIdType node, neighbor;
	vtkIdIterator *neighbors;

	for (vtkIdType i = 0; i < graph->GetNumberOfNodes(); i++)
		{
		coord[i] = -1;
		}
	node = this->Centers[axis];
	coord[node] = 0;
	this->Distance[node] = 0;
	this->Mean[axis] = 0;
	this->Head = 0;
	this->Tail = 0;
	this->Queue[this->Head++] = node;

	while (this->Head != this->Tail)
		{
		node = this->Queue[this->Tail++];
		neighbors = graph->GetNeighbors(graph->GetNodeId(node));
		while (neighbors->HasNext())
			{
			neighbor = graph->GetNodeIndex(neighbors->GetNext());
			if (coord[neighbor] < 0)
				{
				coord[neighbor] = coord[node] + 1;
				this->Mean[axis] += coord[neighbor];
				if (this->Distance[neighbor] > coord[neighbor])
					{
					this->Distance[neighbor] = (vtkIdType)coord[neighbor];
					}
				this->Queue[this->Head++] = neighbor;
				}
			}
		neighbors->Delete();
		}
}


void vtkHDELayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
	
	vtkIdType nrNodes;
	vtkPoints *layout;
	vtkIdType pnode, pdist;
	double *axis;
	double norm, dot;
	double *ui, *uihat;
	double pos[3];

	vtkIdType m, i, r;

  vtkDebugMacro(<<"Starting HDE Layout.");

	if (!input)
		{
		return;
		}

	// Copy graph structure to output and allocate space for
	// new layout.
	nrNodes = input->GetNumberOfNodes();
	output->ShallowCopy(input);
	layout = vtkPoints::New();
	layout->SetNumberOfPoints(nrNodes);
	output->SetLayout(layout);

	if (nrNodes < this->M)
		{
		vtkErrorMacro(<<"Insufficient nodes");
		return;
	//	this->M = nrNodes;
		}

	this->Centers  = new vtkIdType[this->M];
	this->Mean     = new double[this->M];
	this->Known    = new char[nrNodes];
	this->Pivot		 = new char[nrNodes];
	this->Distance = new vtkIdType[nrNodes];
	this->Queue    = new vtkIdType[nrNodes];
	this->X				 = new double[this->M * nrNodes];
	this->S				 = new double[this->M * this->M];
	this->U				 = new double[this->C * this->M];

	ui = new double[this->M];
	uihat = new double[this->M];
	for (i = 0; i < nrNodes; i++)
		{
		// Using VTK_LONG_MAX is reasonable, as we are computing
		// path lengths in the graph.  If the maximum path 
		// length is longer than this, we will not in fact have
		// sufficient Ids to identify the individual nodes.
		this->Distance[i] = VTK_LONG_MAX;
		this->Pivot[i] = 0;
		this->Known[i] = 0;
		}

	this->Centers[0] = 0;
	this->Pivot[0] = 1;
	axis = this->X;

	// Find coordinate of nodes relative to first pivot.
	this->PositionFrom(0, axis, output);

	// For the remaining pivot points ...
	for (m = 1; m < this->M; m++)
		{
		// Find which point to use as pivot by searching 
		// distances for a non-pivot node at the furthest
		// distance from that of any previous pivot.
		pdist = 0;
		pnode = 0;
		for (int n = 0; n < nrNodes; n++)
			{
			// choose node if its distance is at least that of the 
			// current best candidate, and it is not already a pivot.
			if (this->Distance[n] >= pdist && !(this->Pivot[n]))
				{
				pnode = n;
				pdist = this->Distance[n];
				}
			// clear known flag for next round
			this->Known[n] = 0;
			}
		// Mark the chosen node as a pivot
		this->Centers[m] = pnode;
		this->Pivot[pnode] = 1;

		// View the graph relative to this new pivot.
		axis += nrNodes;
		this->PositionFrom(m, axis, output);
		}
	
	// Center coordinates by subtracting mean
	axis = this->X;
	for (int a = 0; a < this->M; a++)
		{
		this->Mean[a] /= nrNodes;
		for (int v = 0; v < nrNodes; v++)
			{
			axis[v] -= this->Mean[a];
			}
		axis += nrNodes;
		}
	
	// compute the covariance matrix: S = X*X^T
	for (r = 0; r < this->M; r++)
		{
		for (int c = 0; c < this->M; c++)
			{
			this->S[r*this->M + c] = 0;
			for (int v = 0; v < nrNodes; v++)
				{
				this->S[r*this->M + c] +=
					this->X[r*nrNodes + v]*this->X[c*nrNodes + v];
				}
			this->S[r*this->M + c] /= nrNodes;
			}
		}

	// Compute eigenvectors.
	for (int u = 0; u < this->C; u++)
		{
		// initialize uihat to a random vector
		norm = 0.0;
		for (r = 0; r < this->M; r++)
			{
			uihat[r] = (rand() % 1000);
			norm += uihat[r]*uihat[r];
			}
		norm = sqrt(norm);
		for (int s = 0; s < this->M; s++)
			{
			uihat[s] /= norm;
			}

		do 
			{
			for (i = 0; i < this->M; i++)
				{
				ui[i] = uihat[i];
				}
			// orthogonalize against previous eigenvectors
			for (int j = 0; j < u; j++)
				{
				// compute dot product ui*U[j]
				dot = 0;
				for (m = 0; m < this->M; m++)
					{
					dot += ui[m]*this->U[j*this->M + m];
					}
				norm = 0;
				// subtract (ui.uj)uj
				for (m = 0; m < this->M; m++)
					{
					ui[m] -= dot*this->U[j*this->M + m];
					norm += ui[m]*ui[m];
					}
				for (m = 0; m < this->M; m++)
					{
					ui[m] /= norm;
					}
				}

			// uihat = S*ui
			for (i = 0; i < this->M; i++)
				{
				uihat[i] = 0.0;
				for (int j = 0; j < this->M; j++)
					{
					uihat[i] += this->S[i*this->M + j]*ui[j];
					}
				}

			// normalize uihat and compute uihat(normed)*ui
			norm = 0.0;
			for (r = 0; r < this->M; r++)
				{
				norm += uihat[r]*uihat[r];
				}
			norm = sqrt(norm);

			dot = 0.0;
			for (r = 0; r < this->M; r++)
				{
				uihat[r] /= norm;
				dot += uihat[r]*ui[r];
				}
			}
		while (dot < (1 - 0.001));
		// ui := uihat
		for (m = 0; m < this->M; m++)
			{
			this->U[u*this->M + m] = uihat[m];
			}
		}	// For

	// Now compute actual coordinates!
	// Initialize coordinates in case C < 3.
	for (i = 0; i < 3; i++)
		{
		pos[i] = 0;
		}
	
	// pos = X * U^T
	for (vtkIdType n = 0; n < nrNodes; n++)
		{
		for (int c = 0; c < this->C; c++)
			{
			pos[c] = 0.0;
			for (m = 0; m < this->M; m++)
				{
				pos[c] += this->X[m*nrNodes + n] * this->U[c*this->M + m];
				}
			}
		output->GetLayout()->SetPoint(n, pos);
		}

	// Remove allocated arrays.
	delete [] this->Centers;
	delete [] this->Mean;
	delete [] this->Known;
	delete [] this->Pivot;
	delete [] this->Distance;
	delete [] this->Queue;
	delete [] this->X;
	delete [] this->S;
	delete [] this->U;
	delete [] ui;
	delete [] uihat;

  vtkDebugMacro(<< "HDE Layout Done.");
}


void vtkHDELayout::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

