
# Demo program for visualizing a general graph (NOT a tree)

package require vtk
package require vtkinteraction

if { [catch {load libvtkGraphsTCL}] != 0 && \
     [catch {load vtkGraphsTCL}] != 0} {
  puts "Could not load Graph package."
  exit 1
}

vtkGMLReader reader
  reader SetFileName "../../Data/fsm.gml"

# To colour the graph, the Graph Strahler Metric is used; this
# differs from the standard Strahler metric which works only 
# for trees (and which will crash if fed a graph).  This is for
# illustration only, you can of course just take the layout of
# the graph and draw without colour mapping.

vtkGraphStrahlerMetric strahler
  strahler SetInput [reader GetOutput]
  strahler NormalizeOn

# Map the graph to geometry (polydata).
# The vtkGraphSpanTree layout module requires two FILTERS as
# input (in addition to the graph).  One filter will extract
# a spanning DAG from the graph (this could of course be a
# spanning tree, a special case).  The second filter is a 
# TREE layout filter that is used to lay out a tree extracted
# from the graph via the spanning DAG filter.  These two input
# filters are constructed below.

vtkSpanningDAG sd
vtkConeLayout cl
  cl SetSpacing 1.0
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
  layout SetInput [strahler GetOutput]
  layout SetSpanningDAG sd
  layout SetTreeLayout cl
  layout SetSpacing 1.0

vtkEdgeGeometry geometry
  geometry SetInput [layout GetOutput]

vtkAssignAttribute attributes
  attributes SetInput [geometry GetOutput]
  attributes Assign "strahler" SCALARS POINT_DATA

# tubes filter is included here, but is not used; it
# could be used to map the output of "attributes" to
# obtain thicker geometry.

vtkTubeFilter tubes
  tubes SetInput [attributes GetOutput]
  tubes SetRadius 0.10
  tubes SetVaryRadiusToVaryRadiusByScalar
  tubes SetRadiusFactor 4.0
  tubes SetNumberOfSides 8

vtkLookupTable lut
  lut SetNumberOfColors 64
  lut SetHueRange .6367  .01953
  lut SetValueRange .7773 .84375
  lut SetSaturationRange .457 .457
  lut Build

vtkPolyDataMapper mapper
  mapper SetInput [tubes GetOutput]
  mapper SetLookupTable lut
  mapper SetColorModeToMapScalars
  mapper SetScalarRange 0.0 1.0

vtkActor actor
  actor SetMapper mapper

vtkRenderer ren
  ren AddActor actor
  ren SetBackground 0.1 0.2 0.4

vtkRenderWindow win
  win AddRenderer ren
  win SetSize 400 400

vtkRenderWindowInteractor iren
  iren SetRenderWindow win
  iren AddObserver UserEvent {wm deiconify .vtkInteract}
  iren Initialize

wm withdraw .

