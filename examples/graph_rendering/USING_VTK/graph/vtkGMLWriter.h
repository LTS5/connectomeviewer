/*
 *	File: vtkGMLWriter.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGMLWriter - write graph description in GML format.
// .SECTION Description
// vtkGMLWriter writes a graph in a format based on GML, with 
// some extensions, including the use of declarations for
// attributes.  The format is ASCII.
//
// .SECTION See Also
// vtkGraph vtkGMLReader

#ifndef __vtkGMLWriter_h
#define __vtkGMLWriter_h

#include "vtkWriter.h"
#include "vtkGraph.h"
#include "vtkStringTable.h"


class VTK_EXPORT vtkGMLWriter : public vtkWriter
{
public:
  static vtkGMLWriter *New();
  vtkTypeRevisionMacro(vtkGMLWriter,vtkWriter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set / get the input data or filter.
  void SetInput(vtkGraph *input);
  vtkGraph *GetInput();
              
  // Description:
  // Specify file name to write to.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

protected:
  vtkGMLWriter();
  ~vtkGMLWriter();

  void WriteData();
	char *FileName;  

	ostream *OpenGMLFile();
	void CloseGMLFile(ostream *fp);
	void WriteValue(vtkDataArray *data, ostream *fp, vtkIdType idx);
	void WriteDeclarations(ostream *fp, int location, vtkFieldData *fd);
	
private:
  vtkGMLWriter(const vtkGMLWriter&);  // Not implemented.
  void operator=(const vtkGMLWriter&);  // Not implemented.
};

#endif	/* __vtkGMLWriter_h */


