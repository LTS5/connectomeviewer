
from vtk import *

# Create graph manually
graph = vtkMutableUndirectedGraph()
graph.AddVertex()
graph.AddVertex()
graph.AddVertex()
graph.AddGraphEdge(0, 1)
graph.AddGraphEdge(1, 2)
graph.AddGraphEdge(2, 0)

# Layout it
layout = vtkGraphLayout()
layout.SetInput(graph)

strategy = vtkCircularLayoutStrategy()

layout.SetLayoutStrategy(strategy)
layout.Update()

# Convert to poly data
poly = vtkGraphToPolyData()
poly.SetInputConnection(layout.GetOutputPort())
poly.Update()

#
# Edge
#

edgeMapper = vtkPolyDataMapper()
edgeMapper.SetInputConnection(poly.GetOutputPort())

edgeActor = vtkActor()
edgeActor.SetMapper(edgeMapper)

#
# Vertex
#

# Create glyph, or the shape of vertex
glyph = vtkGlyph3D()
glyph.SetInputConnection(0, poly.GetOutputPort())

glyphSource = vtkGlyphSource2D()
glyphSource.SetGlyphTypeToVertex()
glyph.SetInputConnection(1, glyphSource.GetOutputPort())

vertexMapper = vtkPolyDataMapper()
vertexMapper.SetInputConnection(glyph.GetOutputPort())

vertexActor = vtkActor()
vertexActor.SetMapper(vertexMapper)
vertexActor.GetProperty().SetPointSize(20)


#
# Renderer
#

ren = vtkRenderer()
ren.AddActor(edgeActor)
ren.AddActor(vertexActor)

win = vtkRenderWindow()
win.AddRenderer(ren)

# pan, zoom
style = vtkInteractorStyleImage()

iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(win)
iren.SetInteractorStyle(style)

iren.Initialize()
iren.Start()


