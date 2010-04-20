from vtk import *

# Random graph generator
source = vtkRandomGraphSource()
source.SetNumberOfVertices(50)
source.SetNumberOfEdges(200)

# Create layout
layout = vtkGraphLayout()
layout.SetInputConnection(source.GetOutputPort())

# Use circular layout
strategy = vtkCircularLayoutStrategy()

# Apply layout
layout.SetLayoutStrategy(strategy)
layout.Update()

# Viewer
view = vtkGraphLayoutView()
view.AddRepresentationFromInputConnection(layout.GetOutputPort())
# don't forget to not apply layouting algorithm again
view.SetLayoutStrategyToPassThrough()

window = vtkRenderWindow()
window.SetSize(600, 600)
view.SetupRenderWindow(window)
view.GetRenderer().ResetCamera()
window.GetInteractor().Start()

