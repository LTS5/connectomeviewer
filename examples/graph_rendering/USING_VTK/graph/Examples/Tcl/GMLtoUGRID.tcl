# Utility script for converting from GML to unstructured grid file

package require vtk

if { [catch {load libvtkGraphsTCL}] != 0 \
  && [catch {load vtkGraphsTCL}] != 0} {
  puts "Could not load Graph package."
  exit 1
}
if {$argc < 2} {
	puts "Usage: vtk GMLtoUGRID.tcl inputfile.gml outputBasename"
	exit 1
}

vtkGMLReader reader
  reader SetFileName [lindex $argv 0]

vtkGraphWriter writer
  writer SetBaseName [lindex $argv 1]
	writer XMLFormatOn
  writer SetInput [reader GetOutput]

wm withdraw .
writer Write
exit

