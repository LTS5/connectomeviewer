/*
 *	File: vtkApplyLayout.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkApplyLayout.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"

//-------------------------------------------------

vtkStandardNewMacro(vtkApplyLayout);
vtkCxxRevisionMacro(vtkApplyLayout, "1.0");

vtkApplyLayout::vtkApplyLayout()
{
}

vtkApplyLayout::~vtkApplyLayout()
{
}


//-------------------------------------------------

void vtkApplyLayout::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
  vtkGraph *layout;
  vtkPoints *inPoints, *outPoints;
  
  vtkIdType node;
	vtkIdIterator *nodes;
	
  // Initialize

  vtkDebugMacro(<<"ApplyLayout executing.");

  if (this->NumberOfInputs < 2)
    {
		vtkErrorMacro(<< "Both input and layout must be set." );
		return;
    }

  layout = (vtkGraph*)this->Inputs[1];
  inPoints = layout->GetLayout();
  
  output->ShallowCopy(input);
  outPoints = vtkPoints::New();
  outPoints->SetNumberOfPoints(input->GetNumberOfNodes());
  
	// Traverse the nodes of the input, and set the position
	// of each node in the input to be the position of that 
	// node in the layout source.  Note that we cannot simply
	// assign the layout from layout source to the output, 
	// because nodes may be stored at different indexes in 
	// the two graphs.
	
  nodes = input->GetNodes();
  while (nodes->HasNext())
    {
		node = nodes->GetNext();
		outPoints->InsertPoint(
			output->GetNodeIndex(node), 
			layout->GetPosition(node));
		}
	nodes->Delete();
  output->SetLayout(outPoints);
  outPoints->Delete();
	
	vtkDebugMacro(<<"ApplyLayout done.");
}

void vtkApplyLayout::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
  os << indent << "ApplyLayout filter.\n";
}


