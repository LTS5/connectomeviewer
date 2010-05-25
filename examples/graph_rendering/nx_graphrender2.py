#!/usr/bin/env python -wthread

# CONTINUE:
# weights and vertices with labels example in VTK working (was always freezing)
# try to convert it to tvtk and include it in mayavi
# use it to generate a graph from the data
# try to use string labels (see vtkusers)
# include node movements with picking

import networkx as nx
import numpy as np
from enthought.mayavi import mlab
from enthought.tvtk.api import tvtk

# good examples:
# http://www.cmake.org/Wiki/VTK/Examples#Graphs
# http://www.cmake.org/Wiki/VTK/Examples/ConstructTable
#http://www.stanford.edu/group/sonia/examples/index.html
#http://infomine.ucr.edu/projects/planning/visual.shtml
#http://kitware.com/InfovisWiki/index.php/Titan_Developers_Guide
# http://kitware.com/InfovisWiki/index.php/Main_Page
#check vtk users: http://permalink.gmane.org/gmane.comp.lib.vtk.user/
# use tvtk user defined filters!

# http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/auto/example_tvtk_segmentation.html#example-tvtk-segmentation
# http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/auto/example_tvtk_in_mayavi.html#example-tvtk-in-mayavi7

# numpy and vtk
#import vtk
#import vtk.util.numpy_support as VN
#
#reader = vtk.vtkXMLImageDataReader()
#reader.SetFileName('chombo3d_1.vti')
#reader.Update()
#image_cells = reader.GetOutput().GetCellData()
#phi_array = VN.vtk_to_numpy(image_cells.GetArray('phi'))

# convert the sourceobject arrays! nodeidx and vertices

# use ikeda graph editor to see for picking (moving nodes around)
# update interactor!

# see for layouting

# see for pipelining it with mayavi

G = nx.circular_ladder_graph(10)
#fig = mlab.figure(figure = '2D Graph Layout')
pos = nx.circular_layout(G)
positions = np.zeros((len(G.nodes()),3 ))

nodeidx = np.array([0,1,2])
labelarr = ['a', 'b', 'c']
edges = np.array([[0,1],[1,2],[2,1]])

v = mlab.figure()

# read in a table
# From,To,Attr
# 0,1,5
# 1,2,3
#inta=tvtk.IntArray()
#inta.from_array(edges)

# there is a problem with TVTK for MutuableUnidirectedGraph.add_vertex!
#et = tvtk.EdgeTable()
#for ele in edges:
#    et.insert_edge(ele[0],ele[1], str(ele))
#
#ta=tvtk.Table()
##ta.set_row()

# convert it to 
#ah = tvtk.TableToGraph()
#ah.add_input_connection(et.output_port)
#ah.add_link_edge("From", "To")


source = tvtk.MutableDirectedGraph()

import vtk
src = vtk.vtkMutableDirectedGraph()

#source.number_of_vertices = len(nodeidx)
#source.number_of_edges = len(endges)
#source.include_edge_weights = True
# adding data with loops


for ele in range(len(nodeidx)):
    #nr = source.add_vertex()
    nr = src.AddVertex()
    
for ele in edges:
    #edg = source.add_graph_edge(ele[0], ele[1])
    edg = src.AddGraphEdge(ele[0], ele[1])

source = tvtk.to_tvtk(src)

degree = tvtk.VertexDegree()
#degree.add_input_connection(source.output_port)

view = tvtk.GraphLayoutView()
#view.add_representation_from_input_connection(degree.output_port)
view = view.add_representation_from_input(source)

#view.vertex_label_array_name = "VertexDegree"
#view.vertex_label_visibility = True
#view.vertex_color_array_name = "VertexDegree"
#view.color_vertices = True
#view.edge_label_array_name = "edge weight"
#view.edge_label_visibility = True
#view.edge_color_array_name = "edge weight"
#view.color_edges = True
#view.set_layout_strategy_to_simple2d()

theme = tvtk.ViewTheme().create_mellow_theme()
theme.line_width = 10
view.apply_view_theme(theme)
theme.fast_delete()

# get renderer
ren = view.renderer
ren.reset_camera()

props = ren.view_props
props0 = props.get_item_as_object(0)
map0 = props0.mapper
graph0 = map0.get_input_data_object(0,0)

# export to polydata
pd = tvtk.GraphToPolyData()
pd.set_input(graph0)
pd.update()
# same as  pd.get_output() or pd.output
polydata = pd.get_output_data_object(0)

a = tvtk.Actor(mapper=map0)
v.scene.add_actor(a)
mlab.show()

#
## IDEA: mit Tube filter and double points for scalar coloring!
## more to learn: http://osdir.com/ml/lib.vtk.user/2002-12/msg00123.html
## http://osdir.com/ml/lib.vtk.user/2003-01/msg00149.html
#import sys
#sys.exit()
#
##################
##
## Convert NetworkX draw vtk function to TVTK
#
#
##################
## Convert NetworkX graphs into an usable Graph Data Source for VTK or TVTK
##
#
##################
## Possible Integration of VTK into the Connectome Viewer?
##
#
######################
##
## Experimenting with the VTK Graph classes, filters etc.
## by using them with TVTK
##
######################
#
#from enthought.tvtk.api import tvtk
#
#src = tvtk.RandomGraphSource()
#src.directed = True
#src.number_of_vertices = 50
#src.number_of_edges=800
#
#view = tvtk.GraphLayoutView()
## view.edit_traits()
#view.add_representation_from_input_connection(src.output_port)
##
##theme = tvtk.ViewTheme().create_neon_theme()
##theme.line_width = 5
##theme.point_size = 10
##theme.cell_opacity = 1
##view.apply_view_theme(theme)
#
#ren2=view.renderer
#props = ren2.view_props
#props0 = props.get_item_as_object(0)
#map0 = props0.mapper
#graph0 = map0.get_input_data_object(0,0)
#
## export to polydata
#pd = tvtk.GraphToPolyData()
#pd.set_input(graph0)
#pd.update()
#polydata = pd.get_output_data_object(0)
## same as  pd.get_output() or pd.output
#
#
## visualize 0
##from enthought.mayavi import mlab
##mlab.figure()
##ds = mlab.pipeline.add_dataset(polydata)
##mlab.pipeline.surface(ds)
##mlab.show()
## but edges are not colored per edge!
#
## visualize 1
#
#rw = tvtk.RenderWindow(size=(600, 600))
#ren = tvtk.Renderer(background=(0.5, 0.5, 0.5))
#rwi = tvtk.RenderWindowInteractor(render_window=rw)
#
##a = tvtk.Actor(mapper=map0)
##ren.add_actor(a)
#
##view.setup_render_window(rw)
##rwi.initialize()
##rwi.start()
#
## from tvtk_in_mayavi.py example
#from enthought.mayavi import mlab
#v = mlab.figure()
#a = tvtk.Actor(mapper=map0)
#v.scene.add_actor(a)
## probably extracting more actors, e.g. for nodes etc.
#mlab.show()
#
#
##rw.add_renderer(ren)
##rwi = tvtk.RenderWindowInteractor(render_window=rw)
##ren.add_actor(a)
##rwi.initialize()
##rwi.start()
#
##window = vtkRenderWindow()
##window.SetSize(600, 600)
##view.SetupRenderWindow(window)
##window.GetInteractor().Start()
#
#

