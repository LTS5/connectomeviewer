# Demonstrates combining two representations with
# interactive controls.  The window shows a graph 
# displayed both by a surface (giving an overview of
# where nodes are located) and a standard node-edge
# drawing.  Pressing the "i" key brings up a sphere
# widget that can be moved around or expanded via 
# the mouse.  Inside the sphere the graph is drawn
# using the standard rep, outside only the surface
# is shown.

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
# cl SetSpacing 1.0
  cl CompressionOn
  cl SetSpacing 10.0

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

vtkAssignAttribute attributes
  attributes SetInput [geometry GetOutput]
  attributes Assign "strahler" SCALARS POINT_DATA

vtkTubeFilter tubes
  tubes SetInput [attributes GetOutput]
  tubes SetRadius 0.050
  tubes SetVaryRadiusToVaryRadiusByScalar
  tubes SetRadiusFactor 4.0
  tubes SetNumberOfSides 8

vtkLookupTable lut
  lut SetNumberOfColors 64
  lut SetHueRange .6367  .01953
  lut SetValueRange .7773 .84375
  lut SetSaturationRange .457 .457
  lut Build

vtkSphere sphere
  sphere SetRadius 20.0

vtkClipPolyData cutter
  cutter SetInput [tubes GetOutput]
  cutter SetClipFunction sphere
  cutter GenerateClippedOutputOn

vtkPolyDataMapper mapper
  mapper SetInput [cutter GetClippedOutput]
  mapper SetLookupTable lut
  mapper SetColorModeToMapScalars
  mapper SetScalarRange 0.0 1.0

vtkLODProp3D actor
  actor AddLOD mapper 0.0


vtkCubeSource cube
  cube SetXLength 1.0
  cube SetYLength 1.0
  cube SetZLength 1.0

vtkApplyLayout realPoints
  realPoints SetInput [reader GetOutput]
  realPoints SetLayoutSource [layout GetOutput]

vtkEdgeGeometry realPointGeom
  realPointGeom SetInput [realPoints GetOutput]

vtkClipPolyData glyphClipper
  glyphClipper SetInput [realPointGeom GetOutput]
  glyphClipper SetClipFunction sphere
  glyphClipper GenerateClippedOutputOn
  
vtkGlyph3D glyphs
  glyphs SetSource [cube GetOutput]
  glyphs SetInput [glyphClipper GetClippedOutput]
  glyphs SetScaleFactor 0.6

vtkPolyDataMapper glyphMapper
  glyphMapper SetInput [glyphs GetOutput]
  glyphMapper ScalarVisibilityOff

# The Glyph actor determines the color of the
# glyphs, and is set non-pickable.

vtkActor glyphActor
  glyphActor SetMapper glyphMapper
  glyphActor PickableOff
  [glyphActor GetProperty] SetColor 1.0 1.0 1.0


# ----------- splatting pipeline ------------------

set splatrad 0.05
set splatsrf 0.01

vtkGaussianSplatter splatter
  splatter SetInput [attributes GetOutput]
  splatter SetSampleDimensions 50 50 50
  splatter SetRadius $splatrad
  splatter ScalarWarpingOff

vtkContourFilter surface
  surface SetInput [splatter GetOutput]
  surface SetValue 0 $splatsrf

vtkClipPolyData surfClip
  surfClip SetInput [surface GetOutput]
  surfClip SetClipFunction sphere
# surfClip GenerateClippedOutputOn

vtkPolyDataMapper surfMapper
  surfMapper SetInput [surfClip GetOutput]
  surfMapper ScalarVisibilityOff

vtkActor surfActor
  surfActor SetMapper surfMapper
  [surfActor GetProperty] SetOpacity 0.3
  [surfActor GetProperty] SetColor 0.1 0.1 0.9

# ----------- renderer ---------------------------

vtkRenderer ren
  ren AddActor actor
  ren AddActor glyphActor
  ren AddActor surfActor
  ren SetBackground 0.1 0.2 0.4

vtkRenderWindow win
  win AddRenderer ren
  win SetSize 400 400

vtkRenderWindowInteractor iren
  iren SetRenderWindow win
  iren AddObserver UserEvent {wm deiconify .vtkInteract}

vtkSphereWidget widget
  widget SetInteractor iren
  widget SetInput [attributes GetOutput]
  widget PlaceWidget
  widget AddObserver InteractionEvent callback
  widget SetRepresentationToWireframe

proc callback {} {
  widget GetSphere sphere
}


iren Initialize

wm withdraw .

