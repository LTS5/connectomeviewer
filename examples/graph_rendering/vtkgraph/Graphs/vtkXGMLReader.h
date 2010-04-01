/*
 *	File: vtkXGMLReader.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.  
 */
// .NAME vtkXGMLReader - Read graph dataset in an extended GML format.
// .SECTION Description
// vtkXGMLReader reads a graph from a GML file.  Not all components 
// of the original GML representation are handled, for example node 
// coordinates and edge widths are ignored.
//
// The basic format of an XGML file is as follows.  Lowercase 
// symbols are used for tokens, uppercase for placeholders, while
// optional items are enclosed in {}'s.
//
// {declare [
//   ATTRIBUTE_NAME TYPE_NAME DEFAULT TYPE_OF_ATTRIB OPTIONALITY
//   ...
// ] }
// graph [
//   {node_count NUMBER}
//   {edge_count NUMBER}
// node {NUMBER} [
//   {id NODEID}
//   {label "A STRING"}
//   {ATTRIBUTE_NAME ATTRIBUTE_VALUE}
//   ...
//   {ATTRIBUTE_NAME ATTRIBUTE_VALUE}
// ]
// ...
// edge [
//   {id EDGEID}
//   source NODEID_OF_SOURCE
//   target NODEID_OF_TARGET
//   {ATTRIBUTE_NAME ATTRIBUTE_VALUE}
//   ...
//   {ATTRIBUTE_NAME ATTRIBUTE_VALUE}
// ]
// ...
// ]
//
//
// NODEIDs and EDGEIDs are values of type vtkIdType that 
// uniquely identify a node/edge.  If they are omitted, the 
// reader assigns the next available node/edge id.  Leaving
// out node ids however is not particularly useful, as the
// Id of the source and target node are needed to define an
// edge.
//
// The optional number between the "node" and "[" tokens,
// when present, is taken to be the degree of the node in this
// graph.  This feature is useful in that the reader can 
// exactly allocate the number of edges associated with the
// node.  Similarly, the optional "node_count" and "edge_count"
// declarations specify the number of nodes/edges in the graph.
//
// Nodes and edges can also be associated with a number of
// further attributes, of any of the types supported by VTK.
// Attributes must either be declared using the DeclareAttribute 
// operation described in the interface below, or via the 
// "declare [ ... ]" section at the start of the file.  The parts
// of a declaration are:
// -  the attribute name (a string)
// -  the attribute type; this is a string corresponding to the
//    name of a constant in vtkSystemIncludes.h that identifies
//    a VTK data type, for example VTK_INT, VTK_ID_TYPE, or
//    VTK_STRING.
// -  a default value: this value is assigned to nodes/edges if
//    no value has been given explicitly for this attribute.
//    The value must be a valid value of the type.
// -  The type of attribute: this must be NODE_DATA or EDGE_DATA.
// -  Optionality, one of OPTIONAL or MANDATORY.  An optional
//    attribute will only be included in the output graph if it 
//    has been used by some node/edge.
//
// Two cases require special mention:
//
// 1. For string-valued attributes (including labels), the actual
// string is recorded in a vtkStringTable object associated
// with the reader.  The corresponding array in the node/edge
// data of the graph is a vtkIdTypeArray, whose value for a 
// given node/edge is the id of the corresponding string in the
// string table.  Strings in the file should be delimited by 
// double-quotes "".  The string table used by the reader can
// be set and retrieved using public methods.
//
// 2. For char-valued attributes, the character taken as the
// attribute value should be delimited by single quotes.
//
// .SECTION WARNING
// This class is being replaced by vtkGMLReader.
//
// .SECTION See Also
// vtkGraph vtkGMLReader

#ifndef __vtkXGMLReader_h
#define __vtkXGMLReader_h

#include "vtkGraphSource.h"
#include "vtkDataArray.h"
#include "vtkStringTable.h"

//BTX
typedef int XGMLOptionMode;
#define XGML_OPTIONAL			0
#define	XGML_MANDATORY		1
//ETX

class VTK_EXPORT vtkXGMLReader : public vtkGraphSource
{
public:
  static vtkXGMLReader *New();
  vtkTypeRevisionMacro(vtkXGMLReader,vtkGraphSource);
  void PrintSelf(ostream& os, vtkIndent indent);   

  // Description:
  // Specify file name.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

	// Description:
	// Declare attributes that can be read from file; the parameters
	// are the name, the type (uring numerical VTK type ids, and a
	// default value cast as a float.
	void DeclareAttribute(
		char *attributeName, 
		int valueType, 
		float defaultValue,
		char *defaultString,
		int dataAttributeType,
		XGMLOptionMode optionalFlag);
		
	void DeclareAttribute(
		char *attributeName, 
		char *valueType, 
		char *defaultValue,
		char *dataAttributeType,
		char *optionalFlag);

	// Description:
	// Set/get the string table in which the text of string-valued
	// attributes will be stored.
	vtkSetObjectMacro(Strings,vtkStringTable);
	vtkGetObjectMacro(Strings,vtkStringTable);
	
protected:
  vtkXGMLReader();
  ~vtkXGMLReader();

  int OpenFile();
  void ReadString(char result[256]);
  void ReadQuotedString(char result[256]);
  int Expect(const char *expected);

	vtkStringTable *Strings;
  char *FileName;
  istream *IS;

	//BTX
	struct _vtkXGMLAttribute_s {
		char *name;
		int typeInfo;
		char *defaultString;
		float defaultValue;
		float value;
		int attributeType;
		char *format;
		bool seen;
		bool optional;
		vtkDataArray *array;
		};
		
	struct _vtkDataType_s
		{
		char *name;
		int typeId;
		char *format;
		};
		
	static _vtkDataType_s DataTypes[];
	static char *DataAttributeTypeNames[2];
	static char *DataAttributeOptionNames[2];
	//ETX

	_vtkXGMLAttribute_s *Attributes;
 	int NumberOfAttributes;
	int AttributeArraySize;

  // void ExecuteInformation();
  void Execute();
	void ResizeAttributeArray(int size);
  
private:
  vtkXGMLReader(const vtkXGMLReader&);    // Not implemented.
  void operator=(const vtkXGMLReader&);  // Not implemented.
};

#endif
