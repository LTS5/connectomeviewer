/*
 *	File: vtkLabelMatcher.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */

#include "vtkLabelMatcher.h"
#include "vtkObjectFactory.h"
#include "vtkIdIterator.h"
#include "vtkIdTypeArray.h"

//-------------------------------------------------

vtkStandardNewMacro(vtkLabelMatcher);
vtkCxxRevisionMacro(vtkLabelMatcher, "1.0");

vtkLabelMatcher::vtkLabelMatcher()
{
	this->InputNodeNames = NULL;
	this->MatchNodeNames = NULL;
	this->InputLabelArrayName = NULL;
	this->MatchLabelArrayName = NULL;
}

vtkLabelMatcher::~vtkLabelMatcher()
{
	if (this->InputNodeNames)
		{
		this->InputNodeNames->UnRegister(this);
		}
	if (this->MatchNodeNames)
		{
		this->MatchNodeNames->UnRegister(this);
		}
	if (this->InputLabelArrayName)
		{
		delete [] this->InputLabelArrayName;
		}
	if (this->MatchLabelArrayName)
		{
		delete [] this->MatchLabelArrayName;
		}
}


//-------------------------------------------------

void vtkLabelMatcher::Execute()
{
  vtkGraph *input  = this->GetInput();
  vtkGraph *output = this->GetOutput();
  vtkGraph *match  = (vtkGraph*)this->Inputs[1];
  
	vtkIdType *labelToIndex;
	vtkIdType *oldIndexToNewId;
	
	vtkIdTypeArray *inputLabelArray;
	vtkIdTypeArray *matchLabelArray;
	
	
  vtkIdType iNode, mNode, oNode;
	vtkIdType iIndex, mIndex;
	vtkIdType iLabelIndex, mLabelIndex;
	char *label;
	
	vtkIdType edge, src, dst;
	vtkIdType size;
	
	vtkIdIterator *nodes, *edges;
	
	double *tuple; 	// for copying data array tuples
	
  // Initialize and check inputs.

  vtkDebugMacro(<<"LabelMatcher executing.");

  if (this->NumberOfInputs < 2)
    {
		vtkErrorMacro(<< "Both input and match graph must be set." );
		return;
    }
	if (!(this->InputNodeNames && this->MatchNodeNames))
		{
		vtkErrorMacro(<< "String tables for input and match graphs must be set." );
		return;
    }
	if (!(this->InputLabelArrayName && this->MatchLabelArrayName))
		{
		vtkErrorMacro(<< "Label array names must be set." );
		return;
    }
		
	inputLabelArray = vtkIdTypeArray::SafeDownCast(
		input->GetNodeData()->GetArray(this->InputLabelArrayName));
	matchLabelArray = vtkIdTypeArray::SafeDownCast(
		match->GetNodeData()->GetArray(this->MatchLabelArrayName));
		
	if (!(inputLabelArray && matchLabelArray))
		{
		vtkErrorMacro(<< "A label array is missing." );
		return;
    }

	// Copy data array structure from input to output. Actual
	// data has to be copied on a node by node (edge by edge)
	// basis as re-indexing may occur.
	output->GetNodeData()->CopyStructure(input->GetNodeData());
	output->GetEdgeData()->CopyStructure(input->GetEdgeData());
	
	output->SetInitialNodeSize(input->GetNumberOfNodes());
	output->SetInitialEdgeSize(input->GetNumberOfEdges());
	
	labelToIndex = new vtkIdType[this->MatchNodeNames->GetNextId()];
	oldIndexToNewId = new vtkIdType[input->GetNumberOfNodes()];
	
	// Build up a table mapping each label index to the 
	// index of the node (if any) in the match graph that 
	// uses that label.  If more than one node uses the
	// label a warning is issued, and the most recent use
	// overrides the previous.
	for (vtkIdType i = 0; i < MatchNodeNames->GetNextId(); i++)
		{
		labelToIndex[i] = -1;
		}
	nodes = match->GetNodes();
	while (nodes->HasNext())
		{
		mNode = nodes->GetNext();
		mIndex = match->GetNodeIndex(mNode);
		mLabelIndex = matchLabelArray->GetValue(mIndex);
		if (labelToIndex[mLabelIndex] != -1)
			{
			vtkWarningMacro(<<"Warning: label used by more than one node.");
			}
		labelToIndex[mLabelIndex] = mIndex;
		}
	nodes->Delete();
	
	// For each node in the input, find the label assigned to it,
	// and then find the node index in the match graph that uses
	// that label.  The new id of the node will be the id of the
	// corresponding node in the match graph.
	
	size = input->GetNodeData()->GetNumberOfComponents();
	tuple = (size > 0) ? new double[size] : NULL;
					

	nodes = input->GetNodes();
	while (nodes->HasNext())
		{
		// find the label of the next node from the input
		iNode = nodes->GetNext();
		iIndex = input->GetNodeIndex(iNode);
		iLabelIndex = inputLabelArray->GetValue(iIndex);
		label = this->InputNodeNames->GetString(iLabelIndex);
		// by default, the node id is preserved.
		oldIndexToNewId[iIndex] = iNode;
		
		// if the label is present in the string table used by
		// the match graph
		if (this->MatchNodeNames->HasString(label))
			{
			// Find what id is used by the node that has this label.
			mLabelIndex = this->MatchNodeNames->GetKey(label);
			mIndex = labelToIndex[mLabelIndex];
			if (mIndex < 0 || mIndex >= match->GetNumberOfNodes())
				{
				vtkWarningMacro(<<"No node uses label.");
				}
			else
				{
				mNode = match->GetNodeId(mIndex);
				// map the old node index to the node id from match.
				oldIndexToNewId[iIndex] = mNode;
				}
			}
		else
			{
			vtkWarningMacro(<<"Label not used.");
			}
		// Create the corresponding node in the output graph.
		oNode = oldIndexToNewId[iIndex];
		output->CreateNode(oNode);
		output->SetEdgeArraySize(oNode, input->GetDegree(iNode));
		
		// If there is any node data, copy it
		if (tuple)
			{
			input->GetNodeData()->GetTuple(iIndex, tuple);
			output->GetNodeData()->SetTuple(output->GetNodeIndex(oNode), tuple);
			}
		}
	nodes->Delete();
	if (tuple)
		{
		delete [] tuple;
		}

	// Copy edges from input to output, renaming src and dst
	// nodes using the mapping constructed in the previous
	// step.  
	
	// Allocate space to copy edge data, if present.
	size = input->GetEdgeData()->GetNumberOfComponents();
	tuple = (size > 0) ? new double[size] : NULL;
					
	edges = input->GetEdges();
	while (edges->HasNext())
		{
		edge = edges->GetNext();
		input->GetEdge(edge, src, dst);
		output->CreateEdge(
			edge,
			oldIndexToNewId[input->GetNodeIndex(src)],
			oldIndexToNewId[input->GetNodeIndex(dst)] );
		// If there is any edge data, copy it
		if (tuple)
			{
			input->GetEdgeData()->GetTuple(input->GetEdgeIndex(edge), tuple);
			output->GetEdgeData()->SetTuple(output->GetEdgeIndex(edge), tuple);
			}
		}
	edges->Delete();

	if (tuple)
		{
		delete [] tuple;
		}
	delete [] oldIndexToNewId;
	delete [] labelToIndex;
	
	vtkDebugMacro(<<"LabelMatcher done.");
}

void vtkLabelMatcher::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkGraphToGraphFilter::PrintSelf(os,indent);
  os << indent << "LabelMatcher\n";
	os << indent << "Input Label Array: ";
	os << ((this->InputLabelArrayName) ? this->InputLabelArrayName : "(null)") << "\n";
	os << indent << "Match Label Array: ";
	os << ((this->MatchLabelArrayName) ? this->MatchLabelArrayName : "(null)") << "\n";
}


