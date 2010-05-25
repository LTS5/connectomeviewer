/*
 *	File: vtkSelectNthBestNodes.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkSelectNthBestNodes.h"
#include "vtkObjectFactory.h"
#include "vtkFloatArray.h"
#include "vtkBitArray.h"

//-------------------------------------------------


vtkStandardNewMacro(vtkSelectNthBestNodes);

vtkSelectNthBestNodes::vtkSelectNthBestNodes()
{
	this->Proportion     = 0.5;
	this->NumberToSelect = 50;
	this->UseProportion  = 1;

	this->PropertyName = NULL;
	this->SelectorName = NULL;
}

vtkSelectNthBestNodes::~vtkSelectNthBestNodes()
{
	if (this->PropertyName)
		{
		delete [] this->PropertyName;
		}
	if (this->SelectorName)
		{
		delete [] this->SelectorName;
		}
}


//-------------------------------------------------
// Mergesort implementation, based on code from
// M.A. Weiss, Data Structures and Algorithms in C++.

void vtkSelectNthBestNodes::Merge(
	struct _vtkPair_s *array,
	struct _vtkPair_s *temp,
	vtkIdType leftPos,
	vtkIdType rightPos,
	vtkIdType rightEnd)
{
	vtkIdType leftEnd = rightPos-1;
	vtkIdType tmpPos = leftPos;
	vtkIdType size = rightEnd - leftPos + 1;
	
	while (leftPos <= leftEnd && rightPos <= rightEnd)
		{
		if (array[leftPos].value >= array[rightPos].value)
			{
			temp[tmpPos++] = array[leftPos++];
			}
		else
			{
			temp[tmpPos++] = array[rightPos++];
			}
		} 
	while (leftPos <= leftEnd)
		{
		temp[tmpPos++] = array[leftPos++];
		}
	while (rightPos <= rightEnd)
		{
		temp[tmpPos++] = array[rightPos++];
		}
	for (vtkIdType i = 0; i < size; i++, rightEnd--)
		{
		array[rightEnd] = temp[rightEnd];
		}
}

void vtkSelectNthBestNodes::MergeSort(
	struct _vtkPair_s *array,
	struct _vtkPair_s *temp,
	vtkIdType left,
	vtkIdType right)
{
	vtkIdType center;
	
	if (left < right)
		{
		center = (left + right)/2;
		this->MergeSort(array, temp, left, center);
		this->MergeSort(array, temp, center+1, right);
		this->Merge(array, temp, left, center+1, right);
		}
}


//-------------------------------------------------
// Filter execution

void vtkSelectNthBestNodes::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkGraph *output = this->GetOutput();
	
	vtkFloatArray *property;
	vtkBitArray *selector;
	vtkIdType nrNodes, nrToSelect, i;

	struct _vtkPair_s *array;
	struct _vtkPair_s *temp;
	
  vtkDebugMacro(<<"SelectNthBestNodes executing." );
	
	// Now create a model object to access the fields

	if (!(this->PropertyName && this->SelectorName))
		{
		vtkErrorMacro(<<"Property and selector names must be set.");
		return;
		}

	property = vtkFloatArray::SafeDownCast(
		input->GetNodeData()->GetArray(this->PropertyName));
		
	if (!property)
		{
		vtkErrorMacro(<<"Property not found in input nodes.\n");
		return;
		}

	nrNodes = input->GetNumberOfNodes();
	if (nrNodes == 0)
		{
		vtkWarningMacro(<<"No nodes in graph.");
		return;
		}

	// create the selector array.
	selector = vtkBitArray::New();
	selector->SetNumberOfTuples(nrNodes);
	selector->SetName(this->SelectorName);
	
	// Shallow-copy graph to output.
	output->ShallowCopy(input);


	// To select the nodes in decreasing order of property, we first need
	// to sort the node ids according to this order. Here we use a merge
	// sort.  First the nodes and property are copied into an array, in
	// which the sorting will be carried out.
	
	array = new _vtkPair_s[nrNodes];
	temp  = new _vtkPair_s[nrNodes];
	for (vtkIdType n = 0; n < nrNodes; n++)
		{
		array[n].index = n;
		array[n].value = property->GetValue(n);
		}
	this->MergeSort(array, temp, 0, nrNodes-1);
	delete [] temp;
	
	// Determine number of nodes to select.
	if (this->UseProportion)
		{
		if (this->Proportion < 0.0)
			{
			this->Proportion = 0.0;
			}
		if (this->Proportion > 1.0)
			{
			this->Proportion = 1.0;
			}
		nrToSelect = (vtkIdType)(nrNodes * this->Proportion);
		}
	else
		{
		nrToSelect = this->NumberToSelect;
		if (nrToSelect > nrNodes)
			{
			nrToSelect = nrNodes;
			}
		}
		
	// Count out required number of nodes, and mark each as
	// selected.
	for (i = 0; i < nrToSelect; i++)
		{
		selector->SetValue(array[i].index, 1);
		}
	// Mark any remaining nodes as non-selected.
	while (i < nrNodes)
		{
		selector->SetValue(array[i++].index, 0);
		}
		
	// Put selector into node data of output.
	output->GetNodeData()->AddArray(selector);
	
	delete [] array;

  vtkDebugMacro(<<"SelectNthBestNodes done.");
}


void vtkSelectNthBestNodes::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkGraphToGraphFilter::PrintSelf(os, indent);
	os << indent << "Property: ";
	os << (this->PropertyName ? this->PropertyName : "(null)") << "\n";

	os << indent << "Selector: ";
	os << (this->SelectorName ? this->SelectorName : "(null)") << "\n";

	os << indent << "Selecting ";
	if (this->UseProportion)
		{
		os << "proportion "<<this->Proportion << "\n";
		}
	else 
		{
		os << "number "<<this->NumberToSelect << "\n";
		}
}


