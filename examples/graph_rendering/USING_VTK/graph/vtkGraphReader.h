/*
 *	File: vtkGraphReader.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGraphReader - Read graph stored via unstructured grid
// .SECTION Description
// vtkGraphReader reads a graph whose description is in two files.
// The first contains the graph structure and data attributes, and
// is stored as a VTK unstructured grid, either in the basic VTK
// format, or in the newer XML format.  Any strings indexed by
// data attributes are read from a separate file.
//
// .SECTION CAVEATS
// Strings are currently not compressed.  There is a maximum
// string length of 4096 (including null terminator).  This can be
// via a constant in this header file.
// 
// SECTION See Also
//   vtkGraph vtkUnstructuredGridReader vtkXMLUnstructuredGridReader
//   vtkStringTable


#ifndef __vtkGraphReader_h
#define __vtkGraphReader_h

#include "vtkGraphSource.h"


#define MAX_STRING_LENGTH		4096

class VTK_EXPORT vtkGraphReader : public vtkGraphSource
{
public:
  static vtkGraphReader *New();
  vtkTypeRevisionMacro(vtkGraphReader,vtkGraphSource);
  void PrintSelf(ostream& os, vtkIndent indent);   

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
  vtkGraphReader();
  ~vtkGraphReader();

  char *BaseName;
	int XMLFormat;
	istream *IS;
	char String[MAX_STRING_LENGTH];
	
	void Execute();
	
	int AbortParse;
	void Expect(const char *token);
	void ReadQuotedString();
	int ReadNumber();
	void ReadString();

private:
  vtkGraphReader(const vtkGraphReader&);    // Not implemented.
  void operator=(const vtkGraphReader&);  // Not implemented.
};

#endif	/* __vtkGraphReader_h */


