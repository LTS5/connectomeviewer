# Demonstrate picking of nodes and display of node labels

package require vtk
package require vtkinteraction

if { [catch {load libvtkGraphsTCL}] != 0 && \
     [catch {load vtkGraphsTCL}] != 0} {
  puts "Could not load Graph package."
  exit 1
}

vtkGMLReader reader
  reader SetFileName "../../Data/fsm.gml"

# The vtkGraphSpanTree layout module requires two FILTERS as
# input (in addition to the graph).  One filter will extract
# a spanning DAG from the graph (this could of course be a
# spanning tree, a special case).  The second filter is a 
# TREE layout filter that is used to lay out a tree extracted
# from the graph via the spanning DAG filter.  These two input
# filters are constructed below.

vtkSpanningDAG sd
vtkConeLayout cl
  cl CompressionOn

# NOTE: YOU MAY HAVE TO PLAY WITH THE SPACING VALUE TO GET A
# SENSIBLE OUTPUT.

# NOTE: the ConeLayout filter has a Spacing parameter that gives
# a space between layers of its tree, and a Compression flag
# which, when on, places the nodes of the tree closer together
# than would be ideal if the underlying dataset was really a tree.

# Now define the layout filter for graph.

vtkSpanLayout layout
  layout SetInput [reader GetOutput]
  layout SetSpanningDAG sd
  layout SetTreeLayout cl
  layout SetSpacing 1.0

# To colour the graph, the Graph Strahler Metric is used; this
# differs from the standard Strahler metric which works only 
# for trees (and which will crash if fed a graph).  This is for
# illustration only, you can of course just take the layout of
# the graph and draw without colour mapping.

vtkGraphStrahlerMetric strahler
  strahler SetInput [layout GetOutput]
  strahler NormalizeOn

# Map the graph to geometry (polydata).
vtkEdgeGeometry geometry
  geometry SetInput [strahler GetOutput]

vtkAssignAttribute colour
  colour SetInput [geometry GetOutput]
  colour Assign "strahler" SCALARS POINT_DATA
  colour ReleaseDataFlagOn

vtkLookupTable lut
  lut SetNumberOfColors 64
  lut SetHueRange .6367  .01953
  lut SetValueRange .7773 .84375
  lut SetSaturationRange .457 .457
  lut Build

vtkPolyDataMapper mapper
  mapper SetInput [colour GetOutput]
  mapper SetLookupTable lut
  mapper SetColorModeToMapScalars
  mapper SetScalarRange 0.0 1.0

vtkActor actor
  actor SetMapper mapper
# [actor GetProperty] SetColor 0.0 0.0 1.0
# [actor GetProperty] SetOpacity 0.3

# GLYPHING PIPELINE -------------------------------

vtkCubeSource cube
  cube SetXLength 1.0
  cube SetYLength 1.0
  cube SetZLength 1.0

vtkApplyLayout realPoints
  realPoints SetInput [reader GetOutput]
  realPoints SetLayoutSource [layout GetOutput]

vtkNodeGeometry realPointGeom
  realPointGeom SetInput [realPoints GetOutput]
  realPointGeom ReleaseDataFlagOn

vtkGlyph3D glyphs
  glyphs SetSource [cube GetOutput]
  glyphs SetInput [realPointGeom GetOutput]
  glyphs SetScaleFactor 0.4
  glyphs GeneratePointIdsOn
  glyphs SetScaleModeToDataScalingOff

# Glyphs will not be colored from scalar data
vtkPolyDataMapper glyphMapper
  glyphMapper SetInput [glyphs GetOutput]
  glyphMapper ScalarVisibilityOff

# The Glyph actor determines the color of the
# glyphs, and is set non-pickable.

vtkActor glyphActor
  glyphActor SetMapper glyphMapper
  glyphActor PickableOff
  [glyphActor GetProperty] SetColor 1.0 0.0 0.0

vtkRenderer ren
  ren AddActor actor
  ren AddActor glyphActor
  ren SetBackground 0.1 0.2 0.4
  actor PickableOff
  glyphActor PickableOn

vtkRenderWindow win
  win AddRenderer ren
  win SetSize 400 400

vtkPointPicker picker
  picker AddObserver EndPickEvent HandlePick
  picker SetTolerance 0.01

vtkRenderWindowInteractor iren
  iren SetRenderWindow win
  iren AddObserver UserEvent {wm deiconify .vtkInteract}
  iren SetPicker picker
  iren Initialize

# wm withdraw .

set nodeId ""
set nodeName ""
set pid 0

wm title . "Data Explorer"

label .idlabel -text "Node id:"
label .nmlabel -text "Node name:"
label .name  -textvariable nodeName \
  -relief sunken \
  -width 50 \
  -anchor w
label .id  -textvariable nodeId \
  -relief sunken \
  -width 10 \
  -anchor w
grid .idlabel -row 0 -column 0 -sticky w
grid .nmlabel -row 1 -column 0 -sticky w
grid .id      -row 0 -column 1 -sticky w
grid .name    -row 1 -column 1 -sticky w

proc HandlePick {} {
  global nodeName nodeId
  set pid [picker GetPointId]
  if { $pid >= 0 } {
    set pids [[[glyphs GetOutput] GetPointData] GetArray "InputPointIds"]
    if { $pid < [$pids GetNumberOfTuples] } {
      set ipid [$pids GetValue $pid]
      set gr [reader GetOutput]
      if { $ipid >= 0 } {
        if { $ipid < [$gr GetNumberOfNodes] } {
          set nodeId [$gr GetNodeId $ipid]
          set labels [[$gr GetNodeData] GetArray label]
          set nodeName [[$gr GetStrings] GetString [$labels GetValue $ipid]]
        } else {
          set nodeId ""
          set nodeName "unknown object"
        }
      }
    } else {
      set focusName "not a glyph"
    }
  }
}

vtkRendererSource rsource
  rsource SetInput ren
  rsource WholeWindowOn

vtkJPEGWriter jpeg
  jpeg SetInput [rsource GetOutput]

