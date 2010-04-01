# source http://www.vtk.org/Wiki/VTK/Examples/Graphs/VisualizeDirectedGraph

# take care, it freezes the system!!!!!!!!
from vtk import *

# Make a small random graph and graph view.
source = vtkRandomGraphSource()
view = vtkGraphLayoutView()

# Do layout manually before handing graph to the view.
# This allows us to know the positions of edge arrows.
layout = vtkGraphLayout()
strategy = vtkSimple2DLayoutStrategy()
layout.SetInputConnection(source.GetOutputPort())
layout.SetLayoutStrategy(strategy);
view.SetLayoutStrategyToPassThrough()

# Add the graph to the view. This will render vertices and edges,
# but not edge arrows.
view.AddRepresentationFromInputConnection(layout.GetOutputPort())

# Manually create an actor containing the glyphed arrows.
graphToPoly = vtkGraphToPolyData()
graphToPoly.SetInputConnection(layout.GetOutputPort())
graphToPoly.EdgeGlyphOutputOn()

# Set the position (0: edge start, 1: edge end) where
# the edge arrows should go.
graphToPoly.SetEdgeGlyphPosition(0.8)

# Make a simple edge arrow for glyphing.
arrowSource = vtkGlyphSource2D()
arrowSource.SetGlyphTypeToEdgeArrow()
arrowSource.SetScale(0.3)

# Use Glyph3D to repeat the glyph on all edges.
arrowGlyph = vtkGlyph3D()
arrowGlyph.SetInputConnection(0, graphToPoly.GetOutputPort(1))
arrowGlyph.SetInputConnection(1, arrowSource.GetOutputPort())

# Add the edge arrow actor to the view.
arrowMapper = vtkPolyDataMapper()
arrowMapper.SetInputConnection(arrowGlyph.GetOutputPort())
arrowActor = vtkActor()
arrowActor.SetMapper(arrowMapper)
view.GetRenderer().AddActor(arrowActor)

# Start interaction.
window = vtkRenderWindow()
window.SetSize(600, 600)
view.SetupRenderWindow(window)
view.GetRenderer().ResetCamera()
window.GetInteractor().Start()