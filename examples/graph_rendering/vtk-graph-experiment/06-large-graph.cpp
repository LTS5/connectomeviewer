#include "vtkSmartPointer.h"
#include "vtkRandomGraphSource.h"
#include "vtkFast2DLayoutStrategy.h"
#include "vtkRandomLayoutStrategy.h"
#include "vtkGraphLayout.h"
#include "vtkGraphToPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkGlyph2D.h"
#include "vtkGlyphSource2D.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkInteractorStyleImage.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkRenderWindowInteractor.h"
#include <iostream>

#include <time.h>
#include <stdio.h>

using namespace std;

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#define SW(str) \
  printf("%s: %ld\n", str, time(NULL) - last); last = time(NULL);

int main() {

	time_t last = 0;
	last = time(NULL);

	/*
	 * Graph
	 */

	VTK_CREATE(vtkRandomGraphSource, source);
	source->SetNumberOfVertices(30000);
	source->SetNumberOfEdges(100000);

	SW("random");

	VTK_CREATE(vtkRandomLayoutStrategy, strategy);

	VTK_CREATE(vtkGraphLayout, layout);
	layout->SetInputConnection(source->GetOutputPort());
	layout->SetLayoutStrategy(strategy);
	layout->Update();

	SW("layout");

	VTK_CREATE(vtkGraphToPolyData, poly);
	poly->SetInputConnection(layout->GetOutputPort());
	poly->Update();

	SW("convert to poly");

	/*
	 * Edge
	 */

	VTK_CREATE(vtkPolyDataMapper, edgeMapper);
	edgeMapper->SetInputConnection(poly->GetOutputPort());

	VTK_CREATE(vtkActor, edgeActor);
	edgeActor->SetMapper(edgeMapper);

	SW("edge actor");

	/*
	 * Vertex
	 */

	VTK_CREATE(vtkGlyphSource2D, glyphSource);
	glyphSource->SetGlyphTypeToVertex();

	VTK_CREATE(vtkGlyph3D, glyph);
	glyph->SetInputConnection(0, poly->GetOutputPort());
	glyph->SetInputConnection(1, glyphSource->GetOutputPort());
	
	VTK_CREATE(vtkPolyDataMapper, vertexMapper);
	vertexMapper->SetInputConnection(glyph->GetOutputPort());

	VTK_CREATE(vtkActor, vertexActor);
	vertexActor->SetMapper(vertexMapper);
	vertexActor->GetProperty()->SetPointSize(5);

	SW("vertex actor");

	/*
	 * Renderer
	 */

	VTK_CREATE(vtkRenderer, ren);
	ren->AddActor(edgeActor);
	ren->AddActor(vertexActor);

	VTK_CREATE(vtkRenderWindow, win);
	win->AddRenderer(ren);

	VTK_CREATE(vtkInteractorStyleSwitch, style);

	VTK_CREATE(vtkRenderWindowInteractor, iren);
	iren->SetRenderWindow(win);
	iren->SetInteractorStyle(style);

	iren->Initialize();
	iren->Start();
 
}
