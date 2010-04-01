# Demonstrate alternative tree layouts via a Tk interface.
# The Tk panel allows the user to open a GML file, select
# a layout, and for two of the layouts, Reingold-Tilford
# and Cone-Tree, alter the spacing between layers of the
# tree.

package require vtk
package require vtkinteraction

if { [catch {load libvtkGraphsTCL}] != 0 && \
     [catch {load vtkGraphsTCL}] != 0} {
  puts "Could not load Graph package."
  exit 1
}

vtkGMLReader reader

# NOTE: the cone layout filter has a Compression flag which, 
# when on, places the nodes of the tree closer together than 
# would be ideal if the underlying dataset was really a tree.
# This is useful when the cone layout is used to position 
# the spanning tree of a general graph.

set layoutRT [vtkReingoldTilfordLayout rt]
set layoutRA [vtkRadialLayout ra]
set layoutCT [vtkConeLayout ct]
  $layoutRT SetInput [reader GetOutput]
  $layoutRA SetInput [reader GetOutput]
  $layoutCT SetInput [reader GetOutput]
  $layoutCT SetSpacing 1.0

set layout $layoutRA

# To colour the graph, the Graph Strahler Metric is used; this
# differs from the standard Strahler metric which works only 
# for trees (and which will crash if fed a graph).  This is for
# illustration only, you can of course just take the layout of
# the graph and draw without colour mapping.

vtkStrahlerMetric strahler
  strahler SetInput [$layout GetOutput]
  strahler NormalizeOn

# Map the graph to geometry.  This particular filter maps 
# edges of the graph to lines.

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
  lut SetScaleToLog10
  lut SetRampToSCurve
  lut Build

vtkPolyDataMapper mapper
  mapper SetInput [attributes GetOutput]
  mapper SetLookupTable lut
  mapper SetColorModeToMapScalars
  mapper SetScalarRange 0.0 1.0

vtkLODProp3D actor
  actor AddLOD mapper 0.0
  actor PickableOff

# ------------------------------------------------------

vtkCubeSource cube
  cube SetXLength 1.0
  cube SetYLength 1.0
  cube SetZLength 1.0

vtkApplyLayout nodes
  nodes SetInput [reader GetOutput]
  nodes SetLayoutSource [$layout GetOutput]

vtkNodeGeometry points
  points SetInput [nodes GetOutput]

vtkGlyph3D glyphs
  glyphs SetInput [points GetOutput]
  glyphs SetSource [cube GetOutput]
  glyphs SetScaleFactor 0.5
  glyphs GeneratePointIdsOn

vtkPolyDataMapper glyphMapper
  glyphMapper SetInput [glyphs GetOutput]
  glyphMapper ScalarVisibilityOff

# The Glyph actor determines the color of the
# glyphs, and is set non-pickable.

vtkActor glyphActor
  glyphActor SetMapper glyphMapper
  [glyphActor GetProperty] SetColor 1.0 1.0 1.0


# ------------------------------------------------------
vtkRenderer ren
  ren SetBackground 0.1 0.2 0.4

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


# -----------------------------------------------
# Build the GUI.
# The interface is organized using the Tk grid
# packer, with two columns: the left (column 0)
# holds labels, the right (column 1) is for
# the widgets.
# -----------------------------------------------

wm title . "Tree Viewer"

# Menubar
# At the moment, the menubar is very very simple, one 
# menu with 2 items.  The menubar is organized with by pack 

frame .mbar -relief raised -bd 2
menubutton .mbar.file -text File -menu .mbar.file.menu
menu .mbar.file.menu
     .mbar.file.menu add command -label Open -command OpenFile
     .mbar.file.menu add command -label Exit -command exit
menubutton .mbar.layout -text Layout -menu .mbar.layout.menu
menu .mbar.layout.menu
     .mbar.layout.menu add radiobutton -label Reingold-Tilford \
         -variable layout -value $layoutRT -command {ChangeLayout rt}
     .mbar.layout.menu add radiobutton -label Radial \
         -variable layout -value $layoutRA -command {ChangeLayout ra}
     .mbar.layout.menu add radiobutton -label Cone-tree \
         -variable layout -value $layoutCT -command {ChangeLayout ct}
pack .mbar.file .mbar.layout -side left 

grid .mbar -columnspan 2 -sticky ew

# Tree size

set size "?"

label .size_label -text "Number of nodes:"
label .size_info  -textvariable size \
  -relief sunken \
  -anchor w
grid .size_label -row 1 -column 0 -sticky w
grid .size_info  -row 1 -column 1 -sticky ew

# Name of picked node.

set node "(none)"

label .pick_label -text "Picked node:"
label .pick_name  -textvariable node \
  -relief sunken \
  -anchor w
grid .pick_label -row 2 -column 0 -sticky w
grid .pick_name  -row 2 -column 1 -sticky ew


# Slider for spacing in layout

set spacing 0

label .spacing_label -text "Spacing:"
frame .spacing
entry .spacing.value -textvariable spacing 
button .spacing.apply -text apply -command SetSpacing
pack .spacing.value .spacing.apply -side left

proc SetSpacing {} {
  global layout spacing
  $layout SetSpacing $spacing
  $layout Modified
	ren ResetCamera
  win Render
}

grid .spacing_label -row 3 -column 0 -sticky w
grid .spacing -row 3 -column 1 -sticky ew


# Procedure to manage changing focus.  Parameter is the
# id of the node (in the dataspace input to FocusMetric)
# that is to become the focus.
 
proc ChangeLayout {name} {
  global layout spacing
  $layout Modified
  strahler SetInput [$layout GetOutput]
  nodes SetLayoutSource [$layout GetOutput]
  ren RemoveAllProps
  ren AddActor actor
  ren AddActor glyphActor
  ren ResetCamera
  win Render
  if { [string compare $name ra] } {
    set spacing [$layout GetSpacing]
  }
}

proc HandlePick {} {
  global node 
  set pid [picker GetPointId]
  if { $pid >= 0 } {
    set pids [[[glyphs GetOutput] GetPointData] GetArray "InputPointIds"]
    if { $pid < [$pids GetNumberOfTuples] } {
      set ipid [$pids GetValue $pid]
      set gr [reader GetOutput]
      if { $ipid >= 0 } {
        if { $ipid < [$gr GetNumberOfNodes] } {
          set labels [[$gr GetNodeData] GetArray label]
          set node [[reader GetStrings] GetString [$labels GetValue $ipid]]
        } else {
          set node "(none)"
        }
      }
    } else {
      set node "(none)"
    }
  }
}

# Procedure opens file and reset views

proc OpenFile {} {
  # Select a file: only .gml can be read at this point.
  global fileName size spacing layout layoutRA
  set types {
      {{GML}         {.gml}  }
      {{All Files }  *       }
  }
  set fileName [tk_getOpenFile -filetypes $types]

  if { $fileName != "" } {
		set spacing 0
    reader SetFileName $fileName
    ren RemoveAllProps
    ren AddActor actor
    ren AddActor glyphActor
    ren ResetCamera
    win Render
    set size [[reader GetOutput] GetNumberOfNodes]
  }
}

## END OF SCRIPT.

