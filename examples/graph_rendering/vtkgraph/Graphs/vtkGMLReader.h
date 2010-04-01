/*
 *	File: vtkGMLReader.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkGMLReader - Read graph dataset in GML-like format.
// .SECTION Description
// vtkGMLReader reads a graph stored in a format based on GML.
// Some changes have been made from the original: node and edge
// attributes have to be declared explicitly, there is provision
// for providing node and edge count and node degree information,
// and the graphics directive of GML is ignored.
//
// The format of a GML file is given below in BNF. Terminal
// symbols are lowercase or enclosed in single quotes.
//
// GRAPH -> graph '[' DECLARATIONS ']'
//
// DECLARATION -> DECLARATION DECLATIONS
//             |  empty
//
// DECLARATION -> node_count NUMBER
//             |  edge_count NUMBER
//             |  node_data NAME DATATYPE
//             |  edge_data NAME DATATYPE
//             |  node '[' NODE_ATTRIBUTES ']'
//             |  edge '[' EDGE_ATTRIBUTES ']'
//
// NODE_ATTRIBUTES -> NODE_ATTRIBUTE NODE_ATTRIBUTES
//                 |  empty
//
// NODE_ATTRIBUTE -> id IDVALUE
//                |  degree NUMBER 
//                |  x DOUBLE
//                |  y DOUBLE
//                |  z DOUBLE
//                |  ATTRIBUTE
//
// EDGE_ATTRIBUTES -> EDGE_ATTRIBUTE EDGE_ATTRIBUTES
//                 |  empty
//
// EDGE_ATTRIBUTE -> id IDVALUE
//                |  source IDVALUE
//                |  target IDVALUE
//                |  line '[' POINTS ']'
//                |  ATTRIBUTE 
//
// POINTS -> POINT POINTS | empty
//
// POINT -> point DOUBLE DOUBLE DOUBLE
//
// ATTRIBUTE -> NAME VALUE
//
// DATATYPE -> short  | unsigned_short
//           | int    | unsigned_int 
//           | long   | unsigned_long
//           | float  | double
//           | string | id_type
//
// VALUE -> NUMERIC | '''CHAR''' | '"'CHARS'"'
//
// NUMERIC -> Any value of a numeric type, i.e. signed or
//            unsigned short|int|long, float, double.
//
// NUMBER -> Non-negative value of type VTK_ID_TYPE
// IDVALUE -> Value of type VTK_ID_TYPE
//
// There are a number of constraints not captured in the grammar:
// 1.  Terminal symbols are case-insensitive.  Attribute names
//     are case sensitive.
// 2.  All node declarations must include an id; the "degree" is
//     optional, if present, it is used to pre-allocate space 
//     for the edges that enter/leave the node.
// 3.  All edge declarations must include an id, source, and target.
// 4.  Any attribute appearing in a (node|edge) declaration must 
//     have appeared in a (node|edge)_data declaration.
// 5.  NAME refers to the name of an attribute, and is a sequence
//     of non-whitespace characters.  NAMEs must be globally
//     unique, i.e. a node attribute and edge attribute cannot
//     use the same name.
// 6.  Any (node|edge)_data declaration must come before the
//     first node|edge declaration
// 7.  The x|y|z declarations denote the position of a node, and
//     will be stored in the graph layout.  If a component of the
//     position is omitted, that component defaults to 0.
// 8.  If both a point declaration and an x|y|z declaration 
//     appear in a node, the position of the node will be set
//     to whichever appeared later in the file.
//
// SECTION See Also
//   vtkGraph

#ifndef __vtkGMLReader_h
#define __vtkGMLReader_h

#include "vtkGraphSource.h"
#include "vtkDataArray.h"
#include "vtkStringTable.h"

#define TOKEN_TABLE_SIZE			 101
#define TOKEN_STRING_LENGTH		4096

class VTK_EXPORT vtkGMLReader : public vtkGraphSource
{
public:
  static vtkGMLReader *New();
  vtkTypeRevisionMacro(vtkGMLReader,vtkGraphSource);
  void PrintSelf(ostream& os, vtkIndent indent);   

  // Description:
  // Specify file name.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

	// Description:
	// Declare attributes.  For backward compatibility, these
	// functions still allow specification of a default value
	// or default string, but these features will be removed
	// in a future release.
	int DeclareAttribute(
		char *name, 
		int type, 
		float defaultValue,
		char *defaultString,
		int location);
	int DeclareAttribute(
		char *nameStr,
		char *typeStr,
		char *defaultStr,
		char *locationStr);
	int DeclareAttribute(
		char *nameStr,
		char *typeStr,
		char *defaultStr,
		char *locationStr,
		char *optionStr);

	void EnableAttribute(int i);

	// Description:
	// DEPRECATED: Set/get the string table in which the text of 
	// string-valued attributes will be stored.  Replaced with
	// set/get the string table on the graph.
	void SetStrings(vtkStringTable *table);
	vtkStringTable *GetStrings();
	
protected:
  vtkGMLReader();
  ~vtkGMLReader();

	vtkStringTable *Strings;
  char *FileName;
  istream *IS;
	int AbortParse;
	int NrNodes;
	int NrEdges;

	char TokenString[TOKEN_STRING_LENGTH];

  int OpenFile();
	
	// Family of functions to read a value from the input
	// stream.  This _should_ be done using a templated
	// function, but at least one compiler did not like
	// a function that a number of others accepted.
	int ReadValue(short* result);
	int ReadValue(unsigned short* result);
	int ReadValue(int* result);
	int ReadValue(unsigned int* result);
	int ReadValue(long* result);
	int ReadValue(unsigned long* result);
	int ReadValue(float* result);
	int ReadValue(double* result);

	float ReadChar();
  void ReadString(char buffer[TOKEN_STRING_LENGTH]);
  void ReadQuotedString();

	int ReadToken();
  int Expect(int token);
	void ParseError(char *message);

	//BTX
	struct _vtkGMLAttribute_s {
		char *name;
		int token;
		int type;
		char *defaultString;
		float defaultValue;
		float value;
		int location;
		vtkDataArray *array;
		};
		
	struct _vtkGMLToken_s 
		{
		char *name;
		int token;
		_vtkGMLToken_s *next;
		};

	_vtkGMLToken_s *TokenTable[TOKEN_TABLE_SIZE];
	//ETX

	void ResizeAttributeArray(int size);
	void StoreAttributeValues(vtkIdType index, int kind);
	void ResetAttributeValues();

	_vtkGMLAttribute_s *Attributes;
 	int NumberOfAttributes;
	int AttributeArraySize;

  // void ExecuteInformation();
  void Execute();
  
	// Support operations for token table management.
	void AddToken(char *name, int token);
	void RemoveToken(char *name);
	int Lookup(char *name);
	int Hash(char *name);

private:
  vtkGMLReader(const vtkGMLReader&);    // Not implemented.
  void operator=(const vtkGMLReader&);  // Not implemented.
};

#endif	/* __vtkGMLReader_h */

