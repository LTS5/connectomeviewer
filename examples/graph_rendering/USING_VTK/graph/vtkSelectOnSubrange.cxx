/*
 *	File: vtkSelectOnSubrange.cxx
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
 
#include "vtkSelectOnSubrange.h"
#include "vtkObjectFactory.h"
#include "vtkFloatArray.h"
#include "vtkBitArray.h"


//----------------------------------------------------

vtkStandardNewMacro(vtkSelectOnSubrange);

vtkSelectOnSubrange::vtkSelectOnSubrange()
{
	this->MinValue = VTK_FLOAT_MIN;
	this->MaxValue = VTK_FLOAT_MAX;

	this->PropertyName = NULL;
	this->SelectorName = NULL;
}

vtkSelectOnSubrange::~vtkSelectOnSubrange()
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

void vtkSelectOnSubrange::Execute()
{
  vtkGraph *input = this->GetInput();
  vtkGraph *output = this->GetOutput();
	
	vtkFloatArray *property;
	vtkBitArray *selector;
	vtkIdType nrNodes;
	float prop;

  vtkDebugMacro(<<"SelectOnSubrange executing." );
	
	// Check that the property and selector names are specified.

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

	// Mark each node as selected or not, depending on value.
	for (vtkIdType i = 0; i < input->GetNumberOfNodes(); i++)
		{
		prop = property->GetValue(i);
		selector->SetValue(
			i, 
			(prop >= this->MinValue && prop <= this->MaxValue)); 
		}
	
	// Put selector into node data of output.
	output->GetNodeData()->AddArray(selector);
	
  vtkDebugMacro(<<"SelectOnSubrange done.");
}


void vtkSelectOnSubrange::PrintSelf(ostream& os, vtkIndent indent)
{
  this->vtkGraphToGraphFilter::PrintSelf(os, indent);
	os << indent << "Min value: " << this->MinValue << "\n";
	os << indent << "Max value: " << this->MaxValue << "\n";

	os << indent << "Property: ";
	os << (this->PropertyName ? this->PropertyName : "(null)") << "\n";

	os << indent << "Selector: ";
	os << (this->SelectorName ? this->SelectorName : "(null)") << "\n";
}


