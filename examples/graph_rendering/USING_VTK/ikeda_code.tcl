package require vtk
package require vtkinteraction
#if { [catch {load libvtkGraphsTCL}] != 0 && \
#   [catch {load vtkGraphsTCL}] != 0} {
#   puts "Could not load Graph package."
#   exit 1
#}
#Read a file
#vtkGMLReader reader
#   reader SetFileName "/home/stephan/Daten/Python/connectomeviewer/examples/graph_rendering/USING_VTK/test.gml "
vtkSpanningDAG sd
vtkConeLayout c1
   c1 CompressionOn
#Define the layout filter for graph
vtkSpanLayout layout
   layout SetInput [reader GetOutput]
   layout SetSpanningDAG sd
   layout SetTreeLayout c1
   layout SetSpacing 1.0
set graph [layout GetOutput]
#Set the colour (Graph strashler Metric is used)
vtkGraphStrahlerMetric strahler
   strahler SetInput $graph
   strahler NormalizeOn
   strahler Update
set gsource [$graph GetSource]
$graph SetSource ""
# Map the edges of the graph to geometry (Edge Part)
vtkEdgeGeometry geometry
  geometry SetInput [strahler GetOutput]
vtkAssignAttribute colourEdge
  colourEdge SetInput [geometry GetOutput]
  colourEdge Assign "strahler" SCALARS POINT_DATA
vtkLookupTable lut
  lut SetNumberOfColors 64
  lut SetHueRange .6367 .01953
  lut SetValueRange .7773 .84375
  lut SetSaturationRange .457 .457
  lut Build
vtkPolyDataMapper mapper
  mapper SetInput [geometry GetOutput]
  mapper SetLookupTable lut
  mapper SetColorModeToMapScalars
  mapper SetScalarRange 0.0 1.0
vtkActor actor
  actor SetMapper mapper
  [actor GetProperty] SetColor 0.6 0.6 1.0
#GLYPHING PIPELINE ---------------------------------
vtkCubeSource cube
  cube SetXLength 1.0
  cube SetYLength 1.0
  cube SetZLength 1.0
# Map the graph to geometry ( Node Part)
vtkNodeGeometry nodes
  nodes SetInput $graph
# Selection feedback--------------------------------------------
# Create an array to store the colour inforamtion
# The default value is 0 == red.
# All nodes are represented red cubes.
global colour graph
vtkIntArray colour
  colour SetNumberOfTuples [$graph GetNumberOfNodes]
   colour SetName "colour"
   for {set i 0 } {$i < [$graph GetNumberOfNodes]} {incr i } {
      colour InsertValue $i 0
   }
   [$graph GetNodeData] AddArray colour
vtkAssignAttribute colourNode
   colourNode SetInput [nodes GetOutput]
   colourNode Assign "colour" SCALARS POINT_DATA
#------------------------------------------------------------------
# To map graphical representation to nodes, this case "cube"
# The colour is determied by Scalar
vtkGlyph3D glyphs
   glyphs SetSource [cube GetOutput]
   glyphs SetInput [colourNode GetOutput]
   glyphs SetColorModeToColorByScalar
   glyphs SetScaleFactor 0.5
   glyphs GeneratePointIdsOn
   glyphs SetScaleModeToDataScalingOff
# To map scalar values into rgba
# (red-green-blue-alpha transparency) color specification
vtkLookupTable soTable
   soTable SetNumberOfColors 2
   soTable SetTableRange 0 1
# Colours are Red Green Blue Alpha
   soTable SetTableValue 0 1 0 0 1
   soTable SetTableValue 1 1 1 0 1
   soTable Build
vtkPolyDataMapper glyphMapper
   glyphMapper SetInput [glyphs GetOutput]
   glyphMapper SetColorModeToMapScalars
   glyphMapper SetLookupTable soTable
   glyphMapper SetScalarModeToUsePointFieldData
   glyphMapper SelectColorArray colour
   glyphMapper SetScalarRange 0 1
# The Glyph actor determines the color of the
# Glyphs, and is set non-pickable.
vtkActor glyphActor
   glyphActor SetMapper glyphMapper
   glyphActor PickableOff
# Pickable setting----------------------------------------------------------
# The plane widget is used probe the dataset.
global x y z
#Set Pointwidget. Its centre and bound is dependent on node (geometry) data
vtkPointWidget pointWidget
   pointWidget SetInput [nodes GetOutput]
   pointWidget AllOff
   pointWidget PlaceWidget
vtkPolyData point
   pointWidget GetPolyData point
#---------------------------------------------------------------------
#Window and interaction setting --------------------------------------
vtkRenderer ren
   ren AddActor actor
   ren AddActor glyphActor
   ren SetBackground 0.1 0.2 0.4
# Picker will apply only the nodes but not the edges
   actor PickableOff
   glyphActor PickableOn
vtkRenderWindow renWin
  renWin AddRenderer ren
  renWin SetSize 800 600
# Picker function
vtkPointPicker picker
   picker AddObserver EndPickEvent HandlePick
   picker SetTolerance 0.1
vtkGenericRenderWindowInteractor iren
   iren AddObserver UserEvent {wm deiconify .vtkInteract}
   iren SetRenderWindow renWin
   pointWidget SetInteractor iren
   pointWidget EnabledOn
# For undo action, record the history of functions implemented.
# The list to store parent node(Source node)
# The list to store child node (Target node)
set hist [list]
set parentList [list]
set childList [list]
#------------------------------------------------------------------
# Handle pick Function
# This is invoked when "P" Key is pressed on a Node.
# It activate the node and make a graph available to modify
# x, y, z coordinate of the centre of the pointWidget ( Default xp, yp,zp)
# The node Id picked one step before. Used to selection feedback.
set x xp
set y yp
set z zp
set oldSelection " "
proc HandlePick {} {
   global graph colour carray
   global nodeName nodeId x y z
   global addEdgeFlag delEdgeFlag delFlag
   global srcNode desNode
   global menustatus
   global memory oldSelection
# pid is a point in the rendering window
# picked by user. This is a point on a glyph
# check if the point is picked
   set pid [picker GetPointId]
   if { $pid >= 0 } {
      set pids [[[glyphs GetOutput] GetPointData] GetArray "InputPointIds"]
      if { $pid < [$pids GetNumberOfTuples] } {
         set ipid [$pids GetValue $pid]
         set memory 0
# Check the point Id if it is within the bound of the array
# Lookup the array to get the id of the point.
# ipid is the index of the node in the graph
# Find nodeID and set "nodeId "
         if { $ipid >= 0 } {
            if { $ipid < [$graph GetNumberOfNodes] } {
               set nodeId [$graph GetNodeId $ipid]
               set labels [[$graph GetNodeData] GetArray label]
               set nodeName [[$graph GetStrings] GetString [$labels GetValue $ipid]]
               set carray [[$graph GetNodeData] GetArray colour]
               if {$oldSelection != " " && [$graph HasNode $oldSelection]} {
                  $carray SetValue [$graph GetNodeIndex $oldSelection] 0
               }
               $carray SetValue [$graph GetNodeIndex $nodeId] 1
               set oldSelection $nodeId
# Callback of "DeleteNode"
               DeleteNode
#Callback of "Edge" function which convey both Adding and deleting an edge.
               Edge
# Tell the glyphs is modified ( colour)
# Render the pipeline so that the selected node will be shown as yellow.
               glyphs Modified
               renWin Render
               set x [$graph GetX $nodeId]
               set y [$graph GetY $nodeId]
               set z [$graph GetZ $nodeId]
               pointWidget SetPosition $x $y $z
              } else {
                 set nodeId " "
                 set nodeName "unknown object"
              }
          }
      } else {
          set focusName "not a glyph"
      }
    }
}
# Move Node function -----------------------------------------------
# The callback proc invoked when the "Move" button is sunken.
# It checks that a valid node has been selected (picked)
# where Move function is active. If so, store the coordinate of a new position
# and set the node position into it.
# No input arguments. No output.
proc MovePoint {} {
    global graph hist memory
    global x y z oldId
    global nodeId moveFlag
    global histBar
    pointWidget GetPolyData point
    set arr [point GetPoint 0]
    set x [lindex $arr 0]
    set y [lindex $arr 1]
    set z [lindex $arr 2]
 if { $nodeId != " " && $moveFlag == 1 } {
       if { $memory == 0 } {
           set x [$graph GetX $nodeId]
           set y [$graph GetY $nodeId]
           set z [$graph GetZ $nodeId]
           lappend hist "MoveNode $nodeId $x $y $z"
        $histBar.text insert end "MoveNode $nodeId \n"
           set memory 1
       } else {
         set oldId $nodeId
         $graph Print
         $graph SetPosition $nodeId $x $y $z
         $graph DataHasBeenGenerated
#Callback updateGraph
         UpdateGraph
      }
   }
}
# Move Function.----------------------------------------------------
# The callback proc invoked when the "Move" button is pressed.
# The default value is 0 where move mode is not active. Once it is pressed,
# the mode becomes active. When it is pressed again, it becomes unactive.
# No input arguments. No output.
set moveFlag 0
set memory 0
proc Move {} {
   global moveFlag moveNode_button
   global menustatus nodeId graph oldId
   if {$moveFlag == 1} {
      set moveFlag 0
      $moveNode_button configure -relief raised
      set memory 0
    } else {
      Reset
      set moveFlag 1
      $moveNode_button configure -relief sunken
      set menustatus "Select the node you want to move"
      set oldId " "
    }
}
#Add Node Function.----------------------------------------------
# The callback proc invoked when the "AddNode" button is pressed.
# Get the position picked by pointWidget and store the position into an array.
# Create a new node to the position.
# CreateNode function return nodeId.Also new node has to have a label.
# No input arguments. No output.
set n 1
proc AddNode {} {
   global graph hist h
   global n addFlag carray
   global menustatus histBar
   Reset
   set menustatus "Move the pointwidege and press"
   set arr [point GetPoint 0]
   set x [lindex $arr 0]
   set y [lindex $arr 1]
   set z [lindex $arr 2]
   set nodeId [$graph CreateNode]
   $graph SetPosition $nodeId $x $y $z
   set name [[$graph GetStrings] AddString "<new node$n>"]
   incr n
   set larray [[$graph GetNodeData] GetArray label]
   $larray InsertValue [$graph GetNodeIndex $nodeId] $name
   set carray [[$graph GetNodeData] GetArray colour]
   $carray InsertValue [$graph GetNodeIndex $nodeId] 0
   lappend hist "AddNode $nodeId "
   puts "store addfunction "
  $histBar.text insert end "AddNode $nodeId \n"
   UpdateGraph
   set addFlag 0
}
global plist clist
#DeleteNode--------------------------------------------------------------------
proc DeleteNode {} {
   global graph nodeId hist h
   global sourceList targetList
   global menustatus histBar
   global delFlag
  if { $delFlag == 1 } {
          set menustatus "choose node to delete by pressing P "
      if { [$graph HasNode $nodeId]} {
          set x [$graph GetX $nodeId]
          set y [$graph GetY $nodeId]
          set z [$graph GetZ $nodeId]
          set parent [$graph GetParents $nodeId]
          set plist [list]
          while {[$parent HasNext]} {
             set src [$parent GetNext]
             lappend plist $src
         }
          lappend sourceList $plist
          set child [$graph GetChildren $nodeId]
          set clist [list]
          while {[$child HasNext]} {
             set des [$child GetNext]
             lappend clist $des
          }
          lappend targetList $clist
          $graph DeleteNode $nodeId
          lappend hist "DeleteNode $nodeId $x $y $z"
          puts "store deletefunction "
          $histBar.text insert end "DeleteNode $nodeId \n"
          UpdateGraph
      }
   }
}
#-------------------------------------------------------------------------------
set delFlag 0
proc Delete {} {
  global delFlag delNode_button moveNode_button moveFlag
  global menustatus
  if {$delFlag == 1} {
     set delFlag 0
     $delNode_button configure -relief raised
   } else {
     Reset
     set delFlag 1
     $delNode_button configure -relief sunken
     set menustatus "Select the node you want to delete"
   }
}
# Edge add&delete Function --------------------------------------------
proc Edge {} {
  global graph hist h
  global nodeName nodeId x y z
  global addEdgeFlag delEdgeFlag delFlag
  global srcNode desNode
  global menustatus histBar
  if { $addEdgeFlag == 1 || $delEdgeFlag == 1 } {
     if { $srcNode == "none" } {
         set srcNode $nodeId
         set menustatus "Source Node is $srcNode. Now, choose target Node"
      } else {
       if { $nodeId != $srcNode } {
            set desNode $nodeId
            if { $addEdgeFlag == 1} {
               $graph CreateEdge $srcNode $desNode
               set menustatus " The edge between $srcNode and $desNode was created. If you want to add another edge,
choose source Node"
               lappend hist "AddEdge $srcNode $desNode"
         $histBar.text insert end "AddEdge between $srcNode and $desNode \n"
            } else {
               if { [$graph HasEdge $srcNode $desNode]} {
                  $graph DeleteEdge $srcNode $desNode
                  set menustatus "The edge between $srcNode & $desNode has been deleted "
                  lappend hist "DeleteEdge $srcNode $desNode "
                   $histBar.text insert end "DeleteEdge between $srcNode and $desNode \n"
               } elseif { [$graph HasEdge $desNode $srcNode]} {
                  $graph DeleteEdge $desNode $srcNode
                  set menustatus "The edge between $srcNode & $desNode has been deleted "
                  lappend hist "DeleteEdge $desNode $srcNode "
                  $histBar.text insert end "DeleteEdge between $desNode and $srcNode \n"
               } else {
                  set menustatus "It does not have any edges with $srcNode. Try again"
               }
            }
            UpdateGraph
            set srcNode "none"
         }
      }
   }
}
#Set add/deleteEdgeFlag On and Off ---------------------------------------------------------------------------------
set addEdgeFlag 0
set delEdgeFlag 0
proc EdgeFlag { e } {
   global addEdgeFlag addEdge_button nodeId
   global delEdgeFlag delEdge_button
   global srcNode nodeId menustatus
   if { $e == "add" } {
      if {$addEdgeFlag == 1 } {
         set addEdgeFlag 0
         $addEdge_button configure -relief raised
      } else {
         Reset
         set addEdgeFlag 1
         $addEdge_button configure -relief sunken
      }
    } elseif { $e == "delete" } {
       if {$delEdgeFlag == 1 } {
          set delEdgeFlag 0
          $delEdge_button configure -relief raised
       } else {
          Reset
          set delEdgeFlag 1
          $delEdge_button configure -relief sunken
       }
    }
    set srcNode "none"
    set menustatus "Choose source Node"
}
#------------------------------------------------------------------
# Refresh the view when the data is modified.
proc UpdateGraph {} {
    global graph
    $graph Modified
    renWin Render
}
# Reset button --------------------------------------------------------
proc Reset {} {
   global moveFlag addEdgeFlag delEdgeFlag delFlag
   global moveNode_button addEdge_button delEdge_button delNode_button
   global menustatus
      set moveFlag 0
      $moveNode_button configure -relief raised
      set addEdgeFlag 0
      $addEdge_button configure -relief raised
      set delEdgeFlag 0
      $delEdge_button configure -relief raised
      set delFlag 0
      $delNode_button configure -relief raised
 }
#------------------------------------------------------------------------
proc Undo {} {
  global hist
  global sourceList targetList
  global graph histBar
  set index [expr [llength $hist]-1]
  set last [lindex $hist $index]
  scan $last "%s" nm
  if { $nm == "AddNode" } {
     scan $last "%s %d" nm id
     $graph DeleteNode $id
     $histBar.text insert end "Undo:AddNode $id \n"
  } elseif { $nm == "DeleteNode" } {
     scan $last "%s %d %f %f %f" nm id x y z
     $graph CreateNode $id
     $graph SetPosition $id $x $y $z
     set name [[$graph GetStrings] AddString "$id"]
     set larray [[$graph GetNodeData] GetArray label]
     $larray InsertValue [$graph GetNodeIndex $id] $name
     set plist [lindex $sourceList end]
     foreach i $plist {
        $graph CreateEdge $i $id
     }
     set clist [lindex $targetList end]
     foreach j $clist {
        $graph CreateEdge $id $j
     }
     set sourceList [lreplace $sourceList end end]
     set targetList [lreplace $targetList end end ]
     $histBar.text insert end "Undo:DeleteNode $id \n"
  } elseif { $nm == "AddEdge" } {
     scan $last "%s %d %d" nm sid did
      $graph DeleteEdge $sid $did
      $histBar.text insert end "Undo:AddEdge between $sid and $did \n"
   } elseif { $nm == "DeleteEdge" } {
      scan $last "%s %d %d" nm sid did
      $graph CreateEdge $sid $did
      $histBar.text insert end "Undo:DeleteEdge between $sid and $did \n"
   } elseif { $nm == "MoveNode" } {
      scan $last "%s %d %f %f %f" nm id x y z
      $graph SetPosition $id $x $y $z
         $histBar.text insert end "Undo:MoveNode $id \n"
      Reset
  }
   UpdateGraph
   set hist [lreplace $hist end end]
}
proc OpenFile {} {
   global graph gsource
   global renWin
   set types {
      {{GML file}        {.gml} }
      {{All Files }    *     } }
   set filename [tk_getOpenFile -filetypes $types]
   if {$filename != " " } {
      if { [string match *.gml $filename] } {
         $graph SetSource $gsource
         ren RemoveAllProps
         ren AddActor actor
         ren AddActor glyphActor
         reader SetFileName $filename
         reader Update
         set graph [reader GetOutput]
         set gsource [$graph GetSource]
         strahler SetInput $graph
         nodes SetInput $graph
         $graph SetSource ""
         renWin Render
         wm title . $filename
      } else {
         puts " Can't read this file"
         return
      }
   }
}
proc SaveFile {} {
   global graph
   set types {
      {{GML file}         {.gml} }
      {{All Files }     *    }
   }
   set filename [tk_getSaveFile -filetypes $types]
   if { $filename != "" } {
      if { [string match *.gml $filename] } {
         vtkGMLWriter writer
         writer SetFileName $filename
         writer SetInput $graph
         writer Write
         writer Delete
      } else {
         puts "Can't write this file"
         return
      }
   }
}
vtkRendererSource rsource
  rsource SetInput ren
  rsource WholeWindowOn
##########################################################################################
# Create the GUI                                                 | _________________________________________|
# Constructed of 6 widgets.                                      |                      .top.menuBar                               |
#                                                                 |-----------------------------|---------------------------------|
# 1:menuBar--- .top.mbar                                          |         .top.f.f1             | .top.f.f2.coord                |
# 2:Function Buttons--- .top.f.f1                                 |    ( Function button) |---------------------------------|
                                                                                                 | .top.f.f2.property              |
# 3:Showing xyz coordinate--- .top.f.f2.coord                       |
# 4:Showing nodeId & nodeName---.top.f.f2.property                |-----------------------------|----------------------------------|
# 5:vtkRenderWidget--- .top.win                                  |                                                                  |
# 6:statusBar--- .top.statusBar                                  |                       .top.window                                |
# 7:histryBar--- .top.histBar                                   |                                                                    |
#                                                                 |                 ( VTK renderWindow)                              |
#                                                                 |----------------------------------------------------------------|
#                                                                 |                    .top.statusBar                                |
#                                                                 |---------------------------------------------------------------|
#                                                                 |                     .top.histBar                               |
#                                                               |___________________________________________|
#
wm withdraw .
toplevel .top -visual best
wm title .top "Graph Editor"
wm protocol .top WM_DELETE_WINDOW ::vtk::cb_exit
set nodeId " "
set nodeName " "
set pid 0
set menustatus "Press any of function buttons to start manipulation "
# Menu bar (mbar) -------------------------------------------------------------
# File --- OpenFile, SaveFile, Exit
# Edit ---
# View
# Option
# Help
frame .top.mbar -relief raised -bd 2
foreach m {File Edit View Help} {
   set lower [string tolower $m]
   menubutton .top.mbar.$lower -text $m \
          -menu .top.mbar.$lower.menu
   pack .top.mbar.$lower -side left
}
menu .top.mbar.file.menu
  .top.mbar.file.menu add command -label Open -command OpenFile
  .top.mbar.file.menu add command -label Save -command SaveFile
#-state disabled
  .top.mbar.file.menu add command -label Exit -command exit
menu .top.mbar.edit.menu
  .top.mbar.edit.menu add command -label Clean -command Clean -state disabled
  .top.mbar.edit.menu add command -label "Undo/Redo" -command Undo
menu .top.mbar.view.menu
  .top.mbar.view.menu add radiobutton -label Front -variable view -value Front \
        -command FrontView
proc FrontView {} {
   ren ResetCamera
   renWin Render
}
menu .top.mbar.help.menu
  .top.mbar.help.menu add command -label "Help" -command Help
proc Help {} {
  toplevel .top.help -visual best
  wm title .top.help "Help"
  wm protocol .top.help WM_DELETE_WINDOW ::vtk::cb_exit
  text .top.help.text -relief sunken -bd 2 -yscrollcommand ".top.help.scroll set" -setgrid 1 \
    -height 30 -undo 1 -autosep 1
   scrollbar .top.help.scroll -command ".top.help.text yview"
   pack .top.help.scroll -side right -fill y
   pack .top.help.text -expand yes -fill both
   .top.help.text tag configure big -font {Courier 14 bold}
   .top.help.text tag configure bold -font {Courier 12 bold}
.top.help.text insert 0.0 {Basic functions are following.} big
.top.help.text insert end {
Press "j"/"t" --- Toggle between joystick and trackball styles.
Press "p" key --- Picking
Button1 ( mouse left) --- Rotate
Button2 ( shift + mouse middle) --- Pan
Button2 ( shift + mouse right) --- Zoom in/up
}
.top.help.text insert end {Add Node} bold
.top.help.text insert end {
1.Move the centre of pointWidget to where you want to position node
2.Press "AddNode" button
}
.top.help.text insert end {Delete Node} bold
.top.help.text insert end {
1.Press "DeleteNode" button
2.Choose the node you want to delete
}
.top.help.text insert end {Add Edge} bold
.top.help.text insert end {
1.Press "AddEdge" button
2.Choose the first node to be source Node
3.Choose the second node to be destination Node
}
.top.help.text insert end {Delete Edge} bold
.top.help.text insert end {
1.Press "DeleteEdge" button
2.Choose the first node to be source Node
3.Choose the second node to be destination Node
}
.top.help.text insert end {Move} bold
.top.help.text insert end {
1.Press "P" key to choose the Node you want to move
2.Drag node }
.top.help.text mark set insert 0.0
}
#if { [llength $hist] < 0 } {
#   .top.mbar.edit.menu entryconfigure 1 -state disabled
#   .top.mbar.edit.menu entryconfigure 1 -state disabled
#if { [llength $hist] > 0 } {
#   .top.mbar.file.menu -state active
#   .top.mbar.edit.menu -state active
#} elseif { [llength $hist] == 0 } {
#   .top.mbar.edit.menu -state disabled
#
#}
#######################################################################
# Function button (f1) ----------------------------------------------------------
set w .top.f
set a "add"
set d "delete"
frame $w
frame $w.f1
labelframe $w.f1.node -pady 4 -text "Node" -font {Courier 12 bold} -padx 4
labelframe $w.f1.edge -pady 4 -text "Edge" -font {Courier 12 bold} -padx 4
pack $w.f1.node $w.f1.edge -side top -expand yes -pady 4 -padx 4 -anchor w
set addNode_button [button $w.f1.node.add -text "Add" -font {Courier 12 bold} -bg PeachPuff1 -command AddNode]
set delNode_button [button $w.f1.node.del -text "Delete" -font {Courier 12 bold} -bg PeachPuff1 -command Delete]
set addEdge_button [button $w.f1.edge.add -text "Add" -font {Courier 12 bold} -bg LightBlue1 -command " EdgeFlag
$a " ]
set delEdge_button [button $w.f1.edge.del -text "Delete" -font {Courier 12 bold} -bg LightBlue1 -command " EdgeFlag
$d " ]
set moveNode_button [button $w.f1.node.moven -text "Move" -font {Courier 12 bold} -bg PeachPuff1 -command Move
]
pack $addNode_button $delNode_button $moveNode_button \
    -side left -pady 2 -anchor w -fill x
pack $addEdge_button $delEdge_button -side left -pady 2 -anchor w -fill x
# xyz coordinate (.top.f.f2.coord)-----------------------------------------------------------
frame $w.f2
global x y z
labelframe $w.f2.coord -pady 4 -text "Coordinate" -font {Courier 12 bold} -padx 4
foreach l {X Y Z} {
   set lower [string tolower $l]
   label $w.f2.coord.$lower-label -text $l
   label $w.f2.coord.$lower-Position -textvariable $lower \
       -bg LemonChiffon \
       -relief sunken \
       -width 10 \
       -anchor w
}
grid $w.f2.coord.x-label     -row 0 -column 0 -sticky w -pady 3m
grid $w.f2.coord.x-Position -row 0 -column 1 -sticky w -padx 2m -pady 3m
grid $w.f2.coord.y-label     -row 0 -column 2 -sticky w -pady 3m
grid $w.f2.coord.y-Position -row 0 -column 3 -sticky w -padx 2m -pady 3m
grid $w.f2.coord.z-label     -row 0 -column 4 -sticky w -pady 3m
grid $w.f2.coord.z-Position -row 0 -column 5 -sticky w -padx 2m -pady 3m
# Showing nodeId and nodeName (.top.f.f2.label) -----------------------------------------------
labelframe $w.f2.property -pady 4 -text "Property" -font {Courier 12 bold} -padx 4
foreach m { Id Name } {
   set lower [string tolower $m]
   label $w.f2.property.node$m-label -text " $m :"
   label $w.f2.property.node$m-text -textvariable node$m \
       -bg LemonChiffon \
       -relief sunken \
       -width 17 \
       -anchor w
}
grid $w.f2.property.nodeId-label -row 0 -column 0 -sticky w
grid $w.f2.property.nodeName-label -row 0 -column 2 -sticky w
grid $w.f2.property.nodeId-text        -row 0 -column 1 -sticky w
grid $w.f2.property.nodeName-text -row 0 -column 3 -sticky w
pack $w.f2.coord $w.f2.property -side top
pack $w.f1 $w.f2 -side left -expand yes -pady 2 -padx .5c -anchor w
# Render Window (window) ---------------------------------------------------------
frame .top.win
vtkTkRenderWidget .top.win.window -width 600 -height 400 -rw renWin
scrollbar .top.win.xscr -orient vertical
scrollbar .top.win.yscr -orient horizontal
grid .top.win.window .top.win.xscr -sticky nsew
grid .top.win.yscr -sticky nsew
# StatusBar (statusBar) -----------------------------------------------------------------
# This shows current status and next process.
# It is located in the bottom of screen.
frame .top.statusBar
label .top.statusBar.label -textvariable menustatus -relief sunken -bd 1 \
          -bg Lightgreen -font "Helvetica 10" -anchor w
pack .top.statusBar.label -side left -padx 2 -expand yes -fill both
# History Bar --------------------------------------------------------------------------
global histBar
set histBar .top.histBar
frame $histBar
text $histBar.text -relief sunken -bd 2 -yscrollcommand "$histBar.scroll set" -setgrid 1 \
     -height 5 -undo 1 -autosep 1
scrollbar $histBar.scroll -command "$histBar.text yview"
pack $histBar.scroll -side right -fill y
pack $histBar.text -expand yes -fill both
$histBar.text insert 0.0 " History \n"
#######################################################################################
pack .top.mbar -side top -fill x
pack $w -side top
#pack .top.f3 -side top
pack .top.win -side top -expand t -fill both
pack .top.histBar .top.statusBar -side bottom -fill x -pady 2
::vtk::bind_tk_render_widget .top.win.window
bind .top.win.window <KeyPress> { };
#[renWin GetInteractor] StartPickCallback "OnLeftButtonDown "
#[renWin GetInteractor] EndPickCallback "OnLeftButtonUp"
[renWin GetInteractor] SetPicker picker
#pointWidget SetInteractor [renWin GetInteractor]
pointWidget AddObserver InteractionEvent MovePoint
renWin Render
tkwait window .

