
# subgraph.tcl
# This script demonstrates the extraction of a subgraph (more
# specifically, a sub-tree).  In this example, the subgraph 
# extracted is based on ordering the tree nodes by their strahler 
# values, and the subgraph is then used to provide an alternative
# representation of the full graph during interaction.

package require vtk
package require vtkinteraction

if { [catch {load libvtkGraphsTCL}] != 0 && \
     [catch {load vtkGraphsTCL}] != 0} {
  puts "Could not load Graph package."
  exit 1
}

# ------------------------------------------------------
# Start of pipeline: reader.

vtkGMLReader reader
  reader SetFileName "../../Data/files.gml"

vtkConeLayout layout
  layout SetInput [reader GetOutput]
  layout SetSpacing 0.2

vtkStrahlerMetric strahler
  strahler SetInput [layout GetOutput]
  strahler NormalizeOn

# --------------------------------------------------------
# Filtering.
#
# Filter output of strahler, marking nodes with strahler
# values above 45 as selected.
# Then use filter output to select a subgraph.

vtkSelectOnSubrange filter
  filter SetInput [strahler GetOutput]
  filter SetPropertyName "strahler"
  filter SetSelectorName "simple"
  filter SetMinValue 0.005

vtkSubgraphFilter subgraph
  subgraph SetInput [filter GetOutput]
  subgraph SetSelectorName "simple"
  subgraph KeepRemainderOff

# ---------------------------------------------------------
# Non-interactive representation: show individual edges.

vtkEdgeGeometry edges
   edges SetInput [strahler GetOutput]

vtkAssignAttribute colour
  colour SetInput [edges GetOutput]
  colour Assign "strahler" SCALARS POINT_DATA

vtkLookupTable lut
  lut SetNumberOfColors 64
  lut SetHueRange .6367  .01953
  lut SetValueRange .7773 .84375
  lut SetSaturationRange .457 .457
  lut SetScaleToLog10
  lut SetRampToSCurve
  lut Build

vtkPolyDataMapper mapper
  mapper SetInput [colour GetOutput]
  mapper SetLookupTable lut
  mapper SetColorModeToMapScalars
  mapper SetScalarRange 0.0 1.0

# ---------------------------------------------------------
# Interactive representation: draw cones

vtkConeGeometry cones
  cones SetInput [subgraph GetOutput]
  cones SetMaxBladeAngle 45

vtkAssignAttribute colour2
  colour2 SetInput [cones GetOutput]
  colour2 Assign "strahler" SCALARS POINT_DATA

vtkPolyDataMapper mapper2
  mapper2 SetInput [colour2 GetOutput]
  mapper2 SetLookupTable lut
  mapper2 SetColorModeToMapScalars
  mapper2 SetScalarRange 0.0 1.0

# ---------------------------------------------------------
# Glyph pipeline.

vtkCubeSource cube
  cube SetXLength 1.0
  cube SetYLength 1.0
  cube SetZLength 1.0

vtkGlyph3D glyphs
  glyphs SetInput [edges GetOutput]
  glyphs SetSource [cube GetOutput]
  glyphs SetScaleFactor 40

vtkPolyDataMapper gmapper
  gmapper SetInput [glyphs GetOutput]
  gmapper ScalarVisibilityOff

# ---------------------------------------------------------
# LOD Management
# For the graph edges, we set up a LOD actor that has two
# representations, the edges, and the cones.  Automatic
# selection is turned off, and initially level 0 is selected.

vtkLODProp3D graphActor
  set ac0 [graphActor AddLOD mapper  0.0]
  set ac1 [graphActor AddLOD mapper2 0.0]
  graphActor SetLODLevel $ac0 0
  graphActor SetLODLevel $ac1 1
  graphActor AutomaticLODSelectionOff
  graphActor SetSelectedLODID $ac0 

# For the nodes, the level 0 representation will be the
# glyphs.  The level 1 (interactive) representation
# should be empty; this is fudged here by creating a
# simple cube with the property that it is fully 
# transparent.  This property, and the property for the
# glyph colour, are created explicitly.

vtkProperty gcolour
  gcolour SetColor 1.0 0.0 0.0

vtkProperty invisible
  invisible SetOpacity 0.0

vtkCubeSource nullCube
vtkPolyDataMapper nullMapper
  nullMapper SetInput [nullCube GetOutput]

vtkLODProp3D glyphActor
  set ga0 [glyphActor AddLOD gmapper gcolour 0.0]
  set ga1 [glyphActor AddLOD nullMapper invisible 0.0]
  glyphActor SetLODLevel $ga0 0
  glyphActor SetLODLevel $ga1 1
  glyphActor AutomaticLODSelectionOff
  glyphActor SetSelectedLODID $ga0 
	
# ---------------------------------------------------------
# Rendering and interface

vtkRenderer ren
  ren AddActor graphActor
  ren AddActor glyphActor
  ren SetBackground 0.1 0.2 0.4

vtkRenderWindow win
  win AddRenderer ren
  win SetSize 400 400

proc startup {} {
  global ac1 ga1
  graphActor SetSelectedLODID $ac1
  glyphActor SetSelectedLODID $ga1
  win Render
}

proc finish {} {
  global ac0 ga0
  graphActor SetSelectedLODID $ac0
  glyphActor SetSelectedLODID $ga0
  win Render
}

vtkRenderWindowInteractor iren
  iren SetRenderWindow win
  iren SetDesiredUpdateRate 50.0
  iren SetStillUpdateRate 0.01
  iren AddObserver UserEvent {wm deiconify .vtkInteract}
  iren AddObserver LeftButtonPressEvent startup
  iren AddObserver MiddleButtonPressEvent startup
  iren AddObserver RightButtonPressEvent startup
  iren AddObserver LeftButtonReleaseEvent finish
  iren AddObserver MiddleButtonReleaseEvent finish
  iren AddObserver RightButtonReleaseEvent finish
  iren Initialize

wm withdraw .


vtkRendererSource rsource
  rsource SetInput ren
  rsource WholeWindowOn

vtkJPEGWriter jpeg
  jpeg SetInput [rsource GetOutput]

