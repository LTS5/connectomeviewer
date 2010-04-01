/*
 *	File: vtkSpanningDAG.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkSpanningDAG.h"
#include "vtkObjectFactory.h"

//--------------------------------------------------------------------------

vtkStandardNewMacro(vtkSpanningDAG);
vtkCxxRevisionMacro(vtkSpanningDAG, "1.0");

vtkSpanningDAG::vtkSpanningDAG()
{
}


//--------------------------------------------------------------------------

void vtkSpanningDAG::Traverse(
  vtkIdType node, 
  vtkGraph *graph,
  vtkGraph *output)
{
  vtkIdType neighborIndex, neighborId, edgeId, edgeIndex;
  vtkIdType nodeIndex = graph->GetNodeIndex(node);
	vtkIdIterator *edges;
  
  this->Seen[nodeIndex] = 1;
	
  edges = graph->GetOutgoingEdges(node);  
  while (edges->HasNext())
    {
		edgeId = edges->GetNext();
		edgeIndex = graph->GetEdgeIndex(edgeId);
  	neighborId = graph->GetTarget(edgeId);
		neighborIndex = graph->GetNodeIndex(neighborId);
	
		if (this->Seen[neighborIndex])
	  	{
	  	if (this->Done[neighborIndex]) 
	    	{
	    	output->CreateEdge(edgeId, node, neighborId);
        }
	  	}
		else
	  	{
	  	output->CreateEdge(edgeId, node, neighborId);
	  	this->Traverse(neighborId, graph, output);
	  	}
		}
  this->Done[nodeIndex] = 1;
	edges->Delete();
}


void vtkSpanningDAG::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
   
	vtkIdIterator *nodes;

  int   nrNodes, node, nodeIx;
  
  // Initialize
  //
  vtkDebugMacro(<<"SpanningDAG executing.");

  nrNodes  = input->GetNumberOfNodes();
	if (nrNodes == 0)
		{
		vtkWarningMacro(<<"Empty input graph.");
		return;
		}
		
	this->Seen = new unsigned char[nrNodes];
	this->Done = new unsigned char[nrNodes];
	
  // Copy nodes to output and initialize arrays.
  nodes = input->GetNodes();
  for (vtkIdType i = 0; nodes->HasNext(); i++)
    {
		node = nodes->GetNext();
		output->CreateNode(node);
		this->Seen[i] = 0;
		this->Done[i] = 0;
		}
		
  nodes->Initialize();
  while (nodes->HasNext())
    {
		node = nodes->GetNext();
		nodeIx = input->GetNodeIndex(node);
    if (!this->Seen[nodeIx])
			{
	  	this->Traverse(node, input, output);
			}
		}
	nodes->Delete();
	delete [] this->Seen;
	delete [] this->Done;
	
	output->GetNodeData()->ShallowCopy(input->GetNodeData());
	output->GetEdgeData()->ShallowCopy(input->GetEdgeData());
  
	vtkDebugMacro(<<"SpanningDAG done.");
}



void vtkSpanningDAG::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
  os << indent << "Spanning DAG: " << "\n";
}
