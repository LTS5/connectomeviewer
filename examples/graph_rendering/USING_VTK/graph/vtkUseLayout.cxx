/*
 *	File: vtkUseLayout.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkUseLayout.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"

//-------------------------------------------------

vtkStandardNewMacro(vtkUseLayout);
vtkCxxRevisionMacro(vtkUseLayout, "1.0");

vtkUseLayout::vtkUseLayout()
{
}

vtkUseLayout::~vtkUseLayout()
{
}


//-------------------------------------------------

void vtkUseLayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
  vtkGraph *layout;
  vtkPoints *inPoints, *outPoints;
  
  vtkIdType node, edge, src, dst, layoutEdge;
	vtkIdType nrBends;
	vtkIdType *bends;

	vtkIdIterator *nodes, *edges;
	int copydata;
	
  // Initialize

  vtkDebugMacro(<<"UseLayout executing.");

  if (this->NumberOfInputs < 2)
    {
		vtkErrorMacro(<< "Both input and layout must be set." );
		return;
    }

  layout = (vtkGraph*)this->Inputs[1];
  inPoints = layout->GetLayout();
  
	output->SetInitialNodeSize(input->GetNumberOfNodes());
	output->SetInitialEdgeSize(input->GetNumberOfEdges());
  outPoints = vtkPoints::New();
  outPoints->SetNumberOfPoints(input->GetNumberOfNodes());
	output->SetLayout(outPoints);
	outPoints->Delete();
	
  
	// Traverse the nodes of the input, and set the position
	// of each node in the input to be the position of that 
	// node in the layout source.  Note that we cannot simply
	// assign the layout from layout source to the output, 
	// because nodes may be stored at different indexes in 
	// the two graphs.
	
	copydata = 0;
	if (input->GetNodeData()->GetNumberOfTuples() > 0)
		{
		output->GetNodeData()->CopyStructure(input->GetNodeData());
		copydata = 1;
		}

	// Traverse nodes, copying into the "pass" graph if they are
	// selectorged, and into the "fail" graph if not and the KeepRemainder
	// selector is set.
	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		node = nodes->GetNext();
		if (!layout->HasNode(node))
			{
			continue;
			}
		output->CreateNode(node);
		output->SetEdgeArraySize(node, input->GetDegree(node));
		output->SetPosition(node, layout->GetPosition(node));
		if (copydata)
			{
			output->GetNodeData()->InsertTuple(
				output->GetNodeIndex(node),
				input->GetNodeData()->GetTuple(input->GetNodeIndex(node)));
			}
		}
	nodes->Delete();

	copydata = 0;
	if (input->GetEdgeData()->GetNumberOfComponents() > 0)
		{
		output->GetEdgeData()->CopyStructure(input->GetEdgeData());
		copydata = 1;
		}

	edges = input->GetEdges();
	while (edges->HasNext())
		{
		edge = edges->GetNext();
		src = input->GetSource(edge);
		dst = input->GetTarget(edge);
		if (output->HasNode(src) && output->HasNode(dst))
			{
			output->CreateEdge(edge, src, dst);
			if (layout->HasEdge(src, dst))
				{
				layoutEdge = layout->GetEdgeId(src, dst);
				nrBends = layout->GetNumberOfBends(layoutEdge);
				bends = layout->GetBendArray(layoutEdge);
				for (int b = 0; b < nrBends; b++)
					{
					output->InsertBend(edge, b, bends[b]);
					}
				}
			if (copydata)
				{
				output->GetEdgeData()->InsertTuple(
					output->GetEdgeIndex(edge),
					input->GetEdgeData()->GetTuple(input->GetEdgeIndex(edge)));
				}
			}
		}
	edges->Delete();	
	
	output->SetBends(layout->GetBends());

	vtkDebugMacro(<<"UseLayout done.");
}

void vtkUseLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
  os << indent << "UseLayout filter.\n";
}


