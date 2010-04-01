# Demo program for drawing a graph using higher-dimensional
# embedding filter.

package require vtk
package require vtkinteraction

if { [catch {load libvtkGraphsTCL}] != 0 && [catch {load vtkGraphsTCL}] != 0} {
  puts "Could not load Graph package."
  exit 1
}


vtkGMLReader reader
  reader SetFileName "../../Data/fsm.gml"

vtkHDELayout layout
  layout SetInput [reader GetOutput]

# Color using te graph strahler metric
vtkGraphStrahlerMetric strahler
  strahler SetInput [layout GetOutput]
  strahler NormalizeOn

# Map the graph to geometry (polydata).
vtkEdgeGeometry geometry
  geometry SetInput [strahler GetOutput]

vtkAssignAttribute attributes
  attributes SetInput [geometry GetOutput]
  attributes Assign "strahler" SCALARS POINT_DATA

vtkLookupTable lut
  lut SetNumberOfColors 64
  lut SetHueRange .6367  .01953
  lut SetValueRange .7773 .84375
  lut SetSaturationRange .457 .457
  lut Build

vtkPolyDataMapper mapper
  mapper SetInput [attributes GetOutput]
  mapper SetLookupTable lut
  mapper SetColorModeToMapScalars
  mapper SetScalarRange 0.0 1.0

vtkActor actor
  actor SetMapper mapper
  actor PickableOff
  
# ------------------------------------------------------
# Glyphs

vtkCubeSource cube
  cube SetXLength 1.0
  cube SetYLength 1.0
  cube SetZLength 1.0

vtkNodeGeometry points
  points SetInput [layout GetOutput]

vtkGlyph3D glypher
  glypher SetInput [points GetOutput]
  glypher SetSource [cube GetOutput]
  glypher SetScaleFactor 0.1
  glypher GeneratePointIdsOn

vtkPolyDataMapper glyphMapper
  glyphMapper SetInput [glypher GetOutput]
  glyphMapper ScalarVisibilityOff

# The Glyph actor determines the color of the
# glyphs, and is set non-pickable.

vtkActor glyphActor
  glyphActor SetMapper glyphMapper
  glyphActor PickableOff
  [glyphActor GetProperty] SetColor 0.0 0.0 0.0

vtkActor glyphs
  glyphs SetMapper glyphMapper
  glyphs PickableOn
  
# Rendering and interaction

vtkRenderer ren
  ren AddActor actor
  ren AddActor glyphs
  ren SetBackground 1 1 1

vtkRenderWindow win
  win AddRenderer ren
  win SetSize 400 400

vtkRenderWindowInteractor iren
  iren SetRenderWindow win
  iren AddObserver UserEvent {wm deiconify .vtkInteract}
  iren Initialize

wm withdraw .

