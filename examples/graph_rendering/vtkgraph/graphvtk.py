from enthought.tvtk.api import tvtk

src = tvtk.RandomGraphSource()
src.directed = True
src.number_of_vertices = 500
src.number_of_edges=1000

view = tvtk.GraphLayoutView()
# view.edit_traits()
view.add_representation_from_input_connection(src.output_port)

theme = tvtk.ViewTheme().create_neon_theme()
theme.line_width = 5
theme.point_size = 10
theme.cell_opacity = 1

view.apply_view_theme(theme)

rw = tvtk.RenderWindow(size=(600, 600))
view.setup_render_window(rw)

ren2=view.renderer
props = ren2.view_props
props0 = props.get_item_as_object(0)
map0 = props0.mapper
graph0 = map0.get_input_data_object(0,0)

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
# but edges are not colored per edge!

# visualize 1
#a = tvtk.Actor(mapper=map0)
#rw = tvtk.RenderWindow(size=(600, 600))
#ren = tvtk.Renderer(background=(0.5, 0.5, 0.5))
#rw.add_renderer(ren)
#rwi = tvtk.RenderWindowInteractor(render_window=rw)
#ren.add_actor(a)
#rwi.initialize()
#rwi.start()



