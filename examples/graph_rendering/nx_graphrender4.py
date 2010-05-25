"""
This file contains Python code illustrating the creation and manipulation of
vtkTable objects.
"""

from vtk import *
import numpy as np

#----------------------------------------------------------
# Create an empty table
T = vtkTable()

# create node table not needed
# From To
# 0
# 1
# 2

# create edge table
# From To MoreProp
# 1    2
# 0    1
edges = np.array([[0,1],[1,2],[2,1]])
weights = np.array([0.5,0.2,0.8])

col1 = vtkIntArray()
col1.SetName("From")
for ele in edges:
    col1.InsertNextValue(ele[0])
T.AddColumn(col1)

col2 = vtkIntArray()
col2.SetName("To")
for ele in edges:
    col2.InsertNextValue(ele[1])
T.AddColumn(col2)

col3 = vtkFloatArray()
col3.SetName("Weight")
for ele in weights:
    col3.InsertNextValue(ele)
T.AddColumn(col3)

T.Dump(15)

#import sys
#sys.exit()

##----------------------------------------------------------
## Create Column 1 (IDs)
#col1 = vtkIntArray()
#col1.SetName("ID")
#for i in range(1, 8):
#    col1.InsertNextValue(i)
#T.AddColumn(col1)
#
##----------------------------------------------------------
## Create Column 2 (Names)
#namesList = ['Bob', 'Ann', 'Sue', 'Bill', 'Joe', 'Jill', 'Rick']
#col2 = vtkStringArray()
#col2.SetName("Name")
#for val in namesList:
#    col2.InsertNextValue(val)
#T.AddColumn(col2)
#
##----------------------------------------------------------
## Create Column 3 (Ages)
#agesList = [12, 25, 72, 11, 31, 36, 32]
#col3 = vtkIntArray()
#col3.SetName("Age")
#for val in agesList:
#    col3.InsertNextValue(val)
#T.AddColumn(col3)
#
#T.Dump(15)
#
#print "vtkTable Example 1: Finished."
#
#data_dir = "../../../../VTKData/Data/Infovis/SQLite/"
#sqlite_file = data_dir + "SmallEmailTest.db"
#
#database = vtkSQLDatabase.CreateFromURL("sqlite://" + sqlite_file)
#database.Open("")
#
#edge_query = database.GetQueryInstance()
#edge_query.SetQuery("select source, target from emails")
#
#vertex_query = database.GetQueryInstance()
#vertex_query.SetQuery("select Name, Job, Age from employee")
#
#edge_table = vtkRowQueryToTable()
#edge_table.SetQuery(edge_query)
#
#vertex_table = vtkRowQueryToTable()
#vertex_table.SetQuery(vertex_query)

graph = vtkTableToGraph()
graph.AddInputConnection(T.GetProducerPort())
graph.AddLinkVertex("From", "Name", False)
graph.AddLinkVertex("To", "Name", False)
graph.AddLinkEdge("From", "To")
#graph.SetVertexTableConnection(vertex_table.GetOutputPort())


view = vtkGraphLayoutView()
view.AddRepresentationFromInputConnection(graph.GetOutputPort())
view.SetVertexLabelArrayName("label")
view.SetVertexLabelVisibility(True)
view.SetVertexColorArrayName("Age")
view.SetColorVertices(True)
view.SetLayoutStrategyToSimple2D()

# Add my new lay strategy
# myFoo = vtkCircularLayoutStrategy()
# view.SetLayoutStrategy(myFoo)

theme = vtkViewTheme.CreateMellowTheme()
theme.SetCellColor(.2,.2,.6)
theme.SetLineWidth(5)
theme.SetPointSize(10)
view.ApplyViewTheme(theme)
view.SetVertexLabelFontSize(20)

window = vtkRenderWindow()
window.SetSize(600, 600)
view.SetupRenderWindow(window)
view.GetRenderer().ResetCamera()
window.GetInteractor().Start()



#------------------------------------------------------------------------------
# Script Entry Point (i.e., main() )
#------------------------------------------------------------------------------

if __name__ == "__main__":
    """ Main entry point of this python script """
