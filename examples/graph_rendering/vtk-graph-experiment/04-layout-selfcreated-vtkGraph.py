
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

view = vtkGraphLayoutView()
view.AddRepresentationFromInputConnection(layout.GetOutputPort())
view.SetLayoutStrategyToPassThrough()

window = vtkRenderWindow()
window.SetSize(600, 600)
view.SetupRenderWindow(window)
view.GetRenderer().ResetCamera()
window.GetInteractor().Start()

