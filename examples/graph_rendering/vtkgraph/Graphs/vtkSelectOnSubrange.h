/*
 *	File: vtkSelectOnSubrange.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkSelectOnSubrange - select nodes based on property value
// .SECTION Description
// vtkSelectOnSubrange marks nodes as selected based on whether
// the value of a particular property (which must be stored as a
// vtkFloatArray) lies within a specified range.  The result 
// (selected or not) is attached to the output node data as
// a bit-array.
//
// .SECTION See Also
// vtkGraph vtkSubgraphFilter

#ifndef __vtkSelectOnSubrange_h
#define __vtkSelectOnSubrange_h

#include "vtkGraphToGraphFilter.h"

	
class VTK_EXPORT vtkSelectOnSubrange : public vtkGraphToGraphFilter
{
public:
  static vtkSelectOnSubrange *New();
  vtkTypeMacro(vtkSelectOnSubrange,vtkGraphToGraphFilter);
  void PrintSelf(ostream& os, vtkIndent indent);


  //---------------------------------------------------------
	// Filter parameters.
	
	// Description:
	// Set/get the name of the array used to mark nodes as selected.  
	vtkSetStringMacro(SelectorName);
	vtkGetStringMacro(SelectorName);
	
	// Description:
	// Set/get the name of the array used as the property.  
	vtkSetStringMacro(PropertyName);
	vtkGetStringMacro(PropertyName);
	
	// Description:
	// Set/get the maximum value defining the range: default VTK_FLOAT_MAX
	vtkSetMacro(MaxValue, float);
	vtkGetMacro(MaxValue, float);
	
	// Description:
	// Set/get the minimum value defining the range: default VTK_FLOAT_MAX
	vtkSetMacro(MinValue, float);
	vtkGetMacro(MinValue, float);
			
protected:
  vtkSelectOnSubrange();
  ~vtkSelectOnSubrange();
   
  void Execute();
	
	char *SelectorName; 	// name of selector array in output
	char *PropertyName; 	// name of property array in input
	float MinValue;
	float MaxValue;
	
private:
  vtkSelectOnSubrange(const vtkSelectOnSubrange&) {};
  void operator=(const vtkSelectOnSubrange&) {};
};

#endif

