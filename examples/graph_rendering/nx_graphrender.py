#!/usr/bin/env python -wthread

import networkx as nx
import numpy as np
# execute in iPython console by
# checking first the path: pwd, cd
#   execfile('nx_graphrender.py')

# get cfile and network graph
#G = cfile.networks[0].graph

G = nx.circular_ladder_graph(10)



#fig = mlab.figure(figure = '2D Graph Layout')

# layouting

pos = nx.circular_layout(G)
#print pos

positions = np.zeros((len(G.nodes()),3 ))

#mlab.points3d()

# IDEA: mit Tube filter and double points for scalar coloring!
# more to learn: http://osdir.com/ml/lib.vtk.user/2002-12/msg00123.html
# http://osdir.com/ml/lib.vtk.user/2003-01/msg00149.html

#################
#
# Convert NetworkX draw vtk function to TVTK


#################
# Convert NetworkX graphs into an usable Graph Data Source for VTK or TVTK
#

#################
# Possible Integration of VTK into the Connectome Viewer?
#

#####################
#
# Experimenting with the VTK Graph classes, filters etc.
# by using them with TVTK
#
#####################

from enthought.tvtk.api import tvtk

src = tvtk.RandomGraphSource()
src.directed = True
src.number_of_vertices = 50
src.number_of_edges=800

view = tvtk.GraphLayoutView()
# view.edit_traits()
view.add_representation_from_input_connection(src.output_port)
#
#theme = tvtk.ViewTheme().create_neon_theme()
#theme.line_width = 5
#theme.point_size = 10
#theme.cell_opacity = 1
#view.apply_view_theme(theme)

ren2=view.renderer
props = ren2.view_props
props0 = props.get_item_as_object(0)
map0 = props0.mapper
graph0 = map0.get_input_data_object(0,0)

# export to polydata
pd = tvtk.GraphToPolyData()
pd.set_input(graph0)
pd.update()
polydata = pd.get_output_data_object(0)
# same as  pd.get_output() or pd.output


# visualize 0
#from enthought.mayavi import mlab
#mlab.figure()
#ds = mlab.pipeline.add_dataset(polydata)
#mlab.pipeline.surface(ds)
#mlab.show()
# but edges are not colored per edge!

# visualize 1

rw = tvtk.RenderWindow(size=(600, 600))
ren = tvtk.Renderer(background=(0.5, 0.5, 0.5))
rwi = tvtk.RenderWindowInteractor(render_window=rw)

#a = tvtk.Actor(mapper=map0)
#ren.add_actor(a)

#view.setup_render_window(rw)
#rwi.initialize()
#rwi.start()

# from tvtk_in_mayavi.py example
from enthought.mayavi import mlab
v = mlab.figure()
a = tvtk.Actor(mapper=map0)
v.scene.add_actor(a)
# probably extracting more actors, e.g. for nodes etc.
mlab.show()


#rw.add_renderer(ren)
#rwi = tvtk.RenderWindowInteractor(render_window=rw)
#ren.add_actor(a)
#rwi.initialize()
#rwi.start()

#window = vtkRenderWindow()
#window.SetSize(600, 600)
#view.SetupRenderWindow(window)
#window.GetInteractor().Start()



