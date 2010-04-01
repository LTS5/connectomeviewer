"""
Using vtkGraphLayoutView for layout

NOTE: Vertex data is lost upon conversion from graph to polydata, so
XML polydata writer only writes out cell data (not point data) from graph 
(when graph0 accessed from drilling down into view)

"""
import vtk

source = vtk.vtkRandomGraphSource()
source.DirectedOff()
source.SetNumberOfVertices(50)
source.SetEdgeProbability(0.01)
source.SetUseEdgeProbability(True)
source.AllowParallelEdgesOn()
source.AllowSelfLoopsOn() 
source.SetStartWithTree(True)

view = vtk.vtkGraphLayoutView()
view.SetLayoutStrategyToSimple2D()
view.AddRepresentationFromInputConnection(source.GetOutputPort())
view.SetVertexColorArrayName("VertexDegree")
view.SetColorVertices(True)

theme = vtk.vtkViewTheme.CreateMellowTheme()
theme.SetLineWidth(5)
theme.SetPointSize(10)
theme.SetCellOpacity(1)
view.ApplyViewTheme(theme)

window = vtk.vtkRenderWindow()
window.SetSize(600, 600)
view.SetupRenderWindow(window)
view.GetRenderer().ResetCamera()

ren = view.GetRenderer()
props = ren.GetViewProps()
prop0 = props.GetItemAsObject(0)
map0 = prop0.GetMapper()
graph0 = map0.GetInputDataObject(0,0)

# graph0 contains two VertexData arrays
print graph0

pd = vtk.vtkGraphToPolyData()
pd.SetInput(graph0)
pd.Update()

# but polydata doesn't have any point data
print pd.GetOutputDataObject(0)

pdwriter = vtk.vtkXMLPolyDataWriter()
pdwriter.SetFileName('polytest2.vtp')
pdwriter.SetDataModeToAppended()
pdwriter.SetInputConnection(pd.GetOutputPort())
pdwriter.Write()

