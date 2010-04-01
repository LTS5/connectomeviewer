/*
 *	File: vtkGraphWriter.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGraphWriter - Write a graph as an unstructured grid and strings.
// .SECTION Description
// vtkGraphWriter writes a graph into two files.  The structure and data
// attributes are stored as an unstrucured grid, written either using the
// original vtkUnstructuredGrid format, or the more recent XML version;
// the choice is determined by a flag on the filter.  Strings indexed by
// node or edge data are written to a separate file.
//
// SECTION See Also
// vtkGraph vtkUnstructuredGridWriter vtkXMLUnstructuredGridWriter


#ifndef __vtkGraphWriter_h
#define __vtkGraphWriter_h

#include "vtkWriter.h"
#include "vtkGraph.h"


class VTK_EXPORT vtkGraphWriter : public vtkWriter
{
public:
  static vtkGraphWriter *New();
  vtkTypeRevisionMacro(vtkGraphWriter,vtkWriter);
  void PrintSelf(ostream& os, vtkIndent indent);   

  // Description:
  // Set / get the input data or filter.
  void SetInput(vtkGraph *input);
  vtkGraph *GetInput();
              
  // Description:
  // Specify the base name of the file(s).
  vtkSetStringMacro(BaseName);
  vtkGetStringMacro(BaseName);

	// Description:
	// Specify whether or not to use XML encoding for the
	// graph structure and data.
	vtkSetMacro(XMLFormat,int);
	vtkGetMacro(XMLFormat,int);
	vtkBooleanMacro(XMLFormat,int);

protected:
  vtkGraphWriter();
  ~vtkGraphWriter();

	void WriteData();
	
  char *BaseName;
  ostream *OS;
	int XMLFormat;

private:
  vtkGraphWriter(const vtkGraphWriter&);    // Not implemented.
  void operator=(const vtkGraphWriter&);  // Not implemented.
};

#endif	/* __vtkGraphWriter_h */


