# Utility script for converting from the old to new GML format

package require vtk

if { [catch {load libvtkGraphsTCL}] != 0 \
  && [catch {load vtkGraphsTCL}] != 0} {
  puts "Could not load Graph package."
  exit 1
}
if {$argc < 2} {
	puts "Usage: vtk XGMLtoGML.tcl inputfile.gml outputfile.gml"
	exit 1
}

vtkXGMLReader reader
  reader SetFileName [lindex $argv 0]

vtkGMLWriter writer
  writer SetFileName [lindex $argv 1]
  writer SetInput [reader GetOutput]

wm withdraw .
writer Write
exit

