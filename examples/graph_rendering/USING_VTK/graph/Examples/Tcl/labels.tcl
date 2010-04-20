# Demonstrate selecting a reachable subset of a graph, and
# use of the vtkGraphStringMapper.

package require vtk
package require vtkinteraction

if { [catch {load libvtkGraphsTCL}] != 0 && [catch {load vtkGraphsTCL}] != 0} {
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

vtkSelectReachable reachable
  reachable SetSelectorName "marked"
  reachable SetInput [strahler GetOutput]
  reachable SetSearchModeToDirected
  reachable SetMaxDistance 1
#
# For this example, the graph is known, and we therefore
# choose a node to be selected initially.  This step need
# not (and can not) be done in general.
#
	reachable SetStartNode 1

vtkSubgraphFilter marked
  marked SetInput [reachable GetOutput]
  marked KeepRemainderOn
  marked SetSelectorName "marked"

# -------------
# Filters for marked subgraph

# Map the graph to geometry (polydata).
vtkEdgeGeometry geometry
  geometry SetInput [marked GetOutput]

vtkAssignAttribute attributes
  attributes SetInput [geometry GetOutput]
  attributes Assign "strahler" SCALARS POINT_DATA

# tubes filter is included here, but is not used; it
# could be used to map the output of "attributes" to
# obtain thicker geometry.

vtkTubeFilter tubes
  tubes SetInput [attributes GetOutput]
  tubes SetRadius 0.2
  tubes SetNumberOfSides 8

vtkPolyDataMapper mapper
  mapper SetInput [tubes GetOutput]
  mapper ScalarVisibilityOff

vtkActor actor
	actor SetMapper mapper
  [actor GetProperty] SetColor 1.0 1.0 1.0

# -------------
# Filters for unmarked subgraph

vtkEdgeGeometry rgeometry
  rgeometry SetInput [marked GetRemainder]

vtkAssignAttribute rattributes
  rattributes SetInput [rgeometry GetOutput]
  rattributes Assign "strahler" SCALARS POINT_DATA

vtkLookupTable rlut
  rlut SetNumberOfColors 64
  rlut SetHueRange .6367  .01953
  rlut SetValueRange .7773 .84375
  rlut SetSaturationRange .8 .8
  rlut Build

vtkPolyDataMapper rmapper
  rmapper SetInput [rattributes GetOutput]
  rmapper SetLookupTable rlut
  rmapper SetColorModeToMapScalars
  rmapper SetScalarRange 0.0 1.0

vtkActor ract
  ract SetMapper rmapper

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

vtkGlyph3D glyphs
  glyphs SetSource [cube GetOutput]
  glyphs SetInput [realPointGeom GetOutput]
  glyphs SetScaleFactor 0.5
  glyphs GeneratePointIdsOn

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

################

vtkGraphStringMapper strings
  strings SetInput [marked GetOutput]
  strings SetArrayName "label"
  [strings GetLabelTextProperty] SetFontSize 10

vtkActor2D labelActor
  labelActor SetMapper strings

vtkRenderer ren
  ren AddActor actor
  ren AddActor ract
  ren AddActor glyphActor
  ren AddActor2D labelActor
  ren SetBackground 0.1 0.2 0.4
  actor PickableOn
  glyphActor PickableOn

vtkRenderWindow win
  win AddRenderer ren
  win SetSize 500 500

vtkPointPicker picker
  picker AddObserver EndPickEvent HandlePick
  picker SetTolerance 0.01

vtkRenderWindowInteractor iren
  iren SetRenderWindow win
  iren AddObserver UserEvent {wm deiconify .vtkInteract}
  iren SetPicker picker
  iren Initialize

#wm withdraw .

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
					reachable SetStartNode $nodeId
					reachable Modified
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

