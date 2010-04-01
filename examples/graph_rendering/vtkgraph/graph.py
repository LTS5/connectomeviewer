"""
Graph generation through CSV reader and TableToGraph

Crashes with "Bus Error" when using XML writer
PolyData looks okay when printed, but writer has a problem during Piece section.
Only gets this far (with blanks in NumberOf... quotes):

<?xml version="1.0"?>
<VTKFile type="PolyData" version="0.1" byte_order="LittleEndian" compressor="vtkZLibDataCompressor">
  <PolyData>
    <Piece NumberOfPoints=""                     NumberOfVerts=""                     NumberOfLines=""                     NumberOfStrips=""                     NumberOfPolys=""                    

NOTE: Doesn't crash if you switch to the legacy format writer...

"""

import vtk

filename = 'sc_small.csv'
reader = vtk.vtkDelimitedTextReader()
reader.SetFileName(filename)
reader.SetHaveHeaders(True)

num = vtk.vtkStringToNumeric()
num.SetInputConnection(reader.GetOutputPort())

tgraph = vtk.vtkTableToGraph()
tgraph.SetInputConnection(num.GetOutputPort())
tgraph.AddLinkVertex('index1','stuff',False)
tgraph.AddLinkVertex('index2','stuff',False)
tgraph.AddLinkEdge('index1', 'index2')

degree = vtk.vtkVertexDegree()
degree.SetInputConnection(tgraph.GetOutputPort())

strategy = vtk.vtkSimple2DLayoutStrategy()
layout = vtk.vtkGraphLayout()
layout.SetInputConnection(degree.GetOutputPort())
layout.SetLayoutStrategy(strategy)
layout.Update()

# print layout.GetOutputDataObject(0).GetPoints()

pd = vtk.vtkGraphToPolyData()
pd.SetInputConnection(layout.GetOutputPort())
pd.Update()

# PolyData looks okay to my eye here, but crashes on write...
print pd.GetOutputDataObject(0)

pdwriter = vtk.vtkXMLPolyDataWriter()
pdwriter.SetFileName('polytest4.vtp')
pdwriter.SetDataModeToAppended()
pdwriter.SetInputConnection(pd.GetOutputPort())
pdwriter.Write()
