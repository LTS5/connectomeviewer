/*
 *	File: vtkForestToTree.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkForestToTree.h"
#include "vtkObjectFactory.h"
#include "vtkGraph.h"
#include "vtkIdIterator.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkForestToTree);
vtkCxxRevisionMacro(vtkForestToTree, "1.0");

vtkForestToTree::vtkForestToTree()
{
}

//----------------------------------------------------------------------------


void vtkForestToTree::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
	
	vtkPoints *layout;
	
	vtkIdIterator *nodes, *edges, *roots;
	vtkIdType node, index, edge, src, dst;
	vtkIdType root;
	vtkIdType nrRoots;
	
	double *tuple = NULL;
	double *rdata;
	double pos[3], loc[3];
	int nrComponents;
	int i;
	
  vtkDebugMacro(<<"ForestToTree starting.");

	nrRoots = 0;
	roots = input->GetSources();
	while (roots->HasNext())
		{
		root = roots->GetNext();
		nrRoots++;
		}

	if (nrRoots < 2)
		{
		output->ShallowCopy(input);
		return;
		}
	
	nrComponents = input->GetNodeData()->GetNumberOfComponents();
	if (nrComponents > 0)
		{
		tuple = new double[nrComponents];
		rdata = new double[nrComponents];
		for (i = 0; i < nrComponents; i++)
			{
			rdata[i] = 0.0;
			}
		output->GetNodeData()->CopyStructure(input->GetNodeData());
		}
	layout = input->GetLayout();
	if (layout)
		{
		output->SetLayout(vtkPoints::New());
		output->GetLayout()->SetNumberOfPoints(input->GetNumberOfNodes()+1);
		}
	for (i = 0; i < 3; i++)
		{
		pos[i] = 0.0;
		}

	// Must install new root and modify graph.
	output->SetInitialNodeSize(input->GetNumberOfNodes()+1);
	output->SetInitialEdgeSize(input->GetNumberOfEdges()+nrRoots);

	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		index = input->GetNodeIndex(node);
		output->CreateNode(node);
		if (tuple)
			{
			input->GetNodeData()->GetTuple(index, tuple);
			output->GetNodeData()->SetTuple(output->GetNodeIndex(node), tuple);
			}
		if (layout && index < layout->GetNumberOfPoints())
			{
			layout->GetPoint(index, loc);
			output->SetPosition(node, loc);
			if (input->GetNumberOfParents(node) == 0)
				{
				for (i = 0; i < 3; i++)
					{
					pos[i] += loc[i];
					}
				}
			}
		if (input->GetNumberOfParents(node) == 0)
			{
			output->SetEdgeArraySize(node, input->GetDegree(node)+1);
			for (i = 0; i < nrComponents; i++)
				{
				rdata[i] += tuple[i];
				}
			}
		else
			{
			output->SetEdgeArraySize(node, input->GetDegree(node));
			}
		}
	nodes->Delete();
	
	edges = input->GetEdges();
	while (edges->HasNext())
		{
		edge = edges->GetNext();
		input->GetEdge(edge, src, dst);
		output->CreateEdge(edge, src, dst);
		}
	edges->Delete();

	root = output->CreateNode(output->GetMaxNodeId()+1);
	roots->Initialize();
	while (roots->HasNext())
		{
		node = roots->GetNext();
		output->CreateEdge(output->GetMaxEdgeId()+1, root, node);
		}
	roots->Delete();
	
	if (tuple)
		{
		for (i = 0; i < nrComponents; i++)
			{
			rdata[i] /= nrRoots;
			}
		output->GetNodeData()->SetTuple(output->GetNodeIndex(root), rdata);
		delete [] tuple;
		delete [] rdata;
		}
	if (layout)
		{
		for (i = 0; i < 3; i++)
			{
			pos[i] /= nrRoots;
			}
		output->SetPosition(root, pos);
		}
		
  vtkDebugMacro(<< "ForestToTree Done.");
}


void vtkForestToTree::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

