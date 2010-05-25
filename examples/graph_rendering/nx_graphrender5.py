""" Graph Renderin with VTK/TVTK """

from enthought.tvtk.api import tvtk
from enthought.mayavi import mlab
import numpy as np

edges = np.array([[0,1],[1,2],[2,1]])
weights = np.array([0.5,0.2,0.8])

# alias for currently used scene
fig = mlab.figure()

# XXX_start: the following section for graph data structure generation
# might be encapsulated in another function

# create table
T = tvtk.Table()

col1 = tvtk.IntArray()
col1.name = "From"
for ele in edges:
    col1.insert_next_value(ele[0])
T.add_column(col1)

col2 = tvtk.IntArray()
col2.name = "To"
for ele in edges:
    col2.insert_next_value(ele[1])
T.add_column(col2)

col3 = tvtk.FloatArray()
col3.name = "Weight"
for ele in weights:
    col3.insert_next_value(ele)
T.add_column(col3)

# XXX_end

# build the pipeline (using TVTK)

graph = tvtk.TableToGraph()
graph.add_input_connection(T.producer_port)
#graph.AddLinkVertex("From", "Name", False)
#graph.AddLinkVertex("To", "Name", False)
graph.add_link_edge("From", "To")
#graph.SetVertexTableConnection(vertex_table.GetOutputPort())

view = tvtk.GraphLayoutView()
view.add_representation_from_input_connection(graph.output_port)
#view.vertex_label_array_name = "label"
#view.vertex_label_visibility = True
#view.vertex_color_array_name = "Age"
view.color_vertices = False
view.color_edges = True
#view.set_layout_strategy_to_simple2d()
view.set_layout_strategy_to_force_directed()
# random, pass_trhough, fast2d, simple2d, community2d, circular, clustering2d

# Add my new layout strategy
# myFoo = vtkCircularLayoutStrategy()
# view.SetLayoutStrategy(myFoo)

theme = tvtk.ViewTheme().create_mellow_theme()
theme.cell_color = (0.2,0.2,0.6)
theme.line_width = 5
theme.point_size = 10
view.apply_view_theme(theme)
view.vertex_label_font_size = 20

# the rendering part is a bit different than
    #window = vtkRenderWindow()
    #window.SetSize(600, 600)
    #view.SetupRenderWindow(window)
    #view.GetRenderer().ResetCamera()
    #window.GetInteractor().Start()
# for mayavi. maybe this can be simplified
# XXX: where is the scene interactor set?

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

# finally, add the actor to the scene2d
fig.scene.add_actor(a)
# and we are lucky, aren't we?