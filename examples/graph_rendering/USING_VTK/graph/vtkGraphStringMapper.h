/*
 *	File: vtkGraphStringMapper.h
 *	Graph visualization library for VTK
 *	(c) 2003 D.J. Duke
 * 
 *	This software is distributed WITHOUT ANY WARRANTY; 
 *	without even the implied warranty of MERCHANTABILITY 
 *	or FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 *	See the file copyright.txt for details.
 */
// .NAME vtkGraphStringMapper - map text stored in a graph attribute.
// .SECTION Description
// vtkGraphStringMapper is based on vtkLabeledDataMapper.  It takes
// a graph as input, and produces text based on strings indexed by
// either a node data or cell data array.  In the former case, the
// text is anchored at the location of the node.  In the latter case,
// the text is positioned midway between the source and target of
// the edge.
//
// .SECTION CAVEATS
// Ideally mapping of strings should be integrated into the whole
// VTK dataset model....
//
// .SECTION See Also
// vtkGraph vtkStringTable vtkLabeledDataMapper vtkMapper2D

#ifndef __vtkGraphStringMapper_h
#define __vtkGraphStringMapper_h

#include "vtkMapper2D.h"
#include "vtkGraph.h"

class vtkTextMapper;
class vtkTextProperty;

class VTK_EXPORT vtkGraphStringMapper : public vtkMapper2D
{
public:
  // Description:
  // Instantiate object.  Default format is %s.
  static vtkGraphStringMapper *New();
  vtkTypeRevisionMacro(vtkGraphStringMapper,vtkMapper2D);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Set/Get the format with which to print the labels. Although 
	// this mapper only prints string data, the format string is
	// settable to allow for other characters to be placed around
	// the string.
  vtkSetStringMacro(LabelFormat);
  vtkGetStringMacro(LabelFormat);


  // Description:
  // Set/Get the name of the data array that contains
	// the indices of the strings to be printed.
  vtkSetStringMacro(ArrayName);
  vtkGetStringMacro(ArrayName);
	
	// Description:
	// Set/get the flag indicating whether the data is
	// associated with nodes (the default) or with edges.
	vtkSetMacro(Location, int);
	vtkGetMacro(Location, int);
	void SetLocationToNodeData() { this->SetLocation(NODE_DATA); };
	void SetLocationToEdgeData() { this->SetLocation(EDGE_DATA); };
	
  // Description:
  // Set the input dataset to the mapper.  
  vtkSetObjectMacro(Input,vtkGraph);
  vtkGetObjectMacro(Input, vtkGraph);

  // Description:
  // Set/Get the text property.
  virtual void SetLabelTextProperty(vtkTextProperty *p);
  vtkGetObjectMacro(LabelTextProperty,vtkTextProperty);

  // Description:
  // Draw the text to the screen at each input point.
  void RenderOpaqueGeometry(vtkViewport* viewport, vtkActor2D* actor);
  void RenderOverlay(vtkViewport* viewport, vtkActor2D* actor);

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  virtual void ReleaseGraphicsResources(vtkWindow *);

protected:
  vtkGraphStringMapper();
  ~vtkGraphStringMapper();

  vtkGraph *Input;
  vtkTextProperty *LabelTextProperty;

  char *LabelFormat;
	char *ArrayName;
  vtkTimeStamp BuildTime;
	int Location;

private:
  int NumberOfLabels;
  int NumberOfLabelsAllocated;
  vtkTextMapper **TextMappers;

private:
  vtkGraphStringMapper(const vtkGraphStringMapper&);  // Not implemented.
  void operator=(const vtkGraphStringMapper&);  // Not implemented.
};

#endif	/* __vtkGraphStringMapper_h */

