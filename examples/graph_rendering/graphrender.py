
def visualize_graph(scene, nodeids, labels, edges, scalars_per_edge, \
                    positions, colors, directed = False):
    """ Visualizes the given graph in 3-space
    
    Keyword arguments:
    scene -- which scene to render the graph
    nodeids -- the nodes unique identifier, array 0..(n-1)
    labels -- according to the enumeration, list (n-1)
    edges -- k edges connectivity based on nodeids (k-1)x2 for k edges
    scalars_per_edge -- scalar value for every row in the edges, list (k-1)x1
    positions -- array of positions for every node, array (n-1)x3
    colors -- list of colorcode tuples (R,G,B) for the nodeids, list (n-1)
    
    """
       
    # color points according to nodeids
    # but try to have freesurfer color labels
    # ASK: how to convert a RGB tuple to a scalar value which maps the colormap?
    
    nodes = mlab.points3d(positions[:,0], positions[:,1], positions[:,2], \
                          nodeids, scale_factor=3, scale_mode='none',\
                          color=(0,0,1), name = 'Brain Regions', \
                          figure = scene)


    # TODO: if directed -> glyphs are arrows
    
    # ALTERNATIVELY, try plot3d
    # -> it's kind of the same just that it automatically generates the Tube
    #nodes = mlab.plot3d(positions[:,0], positions[:,1], positions[:,2], \
    #                      nodeids, 
    #                      colormap='OrRd', name = 'Nodes')

    outline = mlab.pipeline.outline(nodes, color = (0,0,0), figure=scene)
    outline.outline_mode = 'cornered'
    
    #nodes.mlab_source.dataset.lines = edges
    #nodes.mlab_source.update()

    start_positions = positions[edges[:, 0], :].T
    end_positions = positions[edges[:, 1], :].T
    vectors = end_positions - start_positions

    vecs = mlab.quiver3d(start_positions[0], 
                         start_positions[1],
                         start_positions[2],
                         vectors[0],
                         vectors[1],
                         vectors[2],
                         scalars=scalars_per_edge,
                         scale_factor=1,
                         colormap='OrRd',
                         mode='cylinder',
                         transparent=True,
                         name = 'Brain Connectivity',
                         figure = scene
                         )
    
    #vecs.glyph.glyph_source.glyph_dict['cylinder_source']
    vecs.glyph.glyph_source.glyph_source.radius = 0.01
    vecs.glyph.color_mode = 'color_by_scalar'


    # visualize text
    # --------------
    
    # TODO:  get hold ooff application preferences!
    # application.preferences.get('cviewer.plugins.ui.labelload')
    # application.get_extensions('enthought.envisage.preferences')
    # get pkgfile://
    #logger.debug('Info from the preference manager' + preference_manager.cviewerui)
    
    #for i, tmplabel in enumerate(labels):
    #    # only less the 100 labels can be shown
    #    if i > 100:
    #        pass
    #    tmplabel = mlab.text3d(positions[i,0], positions[i,1], \
    #                     positions[i,2], ' ' + tmplabel,  \
    #                    scale=2,
    #                    color = (0,0,0), name='Node Label '+tmplabel)


        
    # visualize edges
    # ---------------
    # show edges with the surface module
    #mlab.pipeline.surface(nodes, color=(1, 0, 0),
    #                          representation='wireframe',
    #                          line_width=4,
    #                          name='Brain Connections')
    
    # and/or show edges with tube module
    #tubes = mlab.pipeline.tube(nodes, tube_radius=0.5, tube_sides=10)
    #tubes.filter.radius_factor = 1.
    #tubes.filter.vary_radius = 'vary_radius_by_scalar'
    #surfi = mlab.pipeline.surface(tubes, colormap='OrRd', opacity=0.9)

    # Make the 100 first points of the lut transparent
    #lut = surfi.module_manager.scalar_lut_manager.lut.table.to_array()
    #lut[:100, -1] = 100
    #surfi.module_manager.scalar_lut_manager.lut.table = lut
    #surfi.update_pipeline()
    #print lut
    
    # ASK: how to either
    # * colorcode each edge with the scalar value from scalars_per_edge
    # * and/or vary_radius_by_scalar PER EDGE, not having per node which leads to cones
    # * how to set opacity/transparent per edge?
    
    
    #import numpy
    #rc = numpy.random.randint(0,256, (256,3))
    #lut[:,:3] = rc
    #ipw.module_manager.scalar_lut_manager.lut.table = lut
    #ipw.update_pipeline()
    #
    #l = ipw.module_manager.scalar_lut_manager.lut
    #l.number_of_colors = 512
    #l.build()
    #lut = l.table.to_array()
    #lut = #randint stuff.
    #l.table = l
    
    # if directed == True:
    # ASK: i would like to draw arrows between nodes, how to do that?

    #mlab.show()

    

#from enthought.mayavi import mlab
#import numpy as np
#x,y,z = np.random.random((3,5))*10
#s = np.array([0.3,0.3,0.6,0.6,0.7])

# vary_radius='vary_radius_by_scalar'
#plot = mlab.plot3d(x,y,z,s, tube_radius=0.2)

# also possible (normally for points3d)
#pts = mlab.points3d(x, y, z, 1.5*scalars.max() - scalars,
#                                    scale_factor=0.015, resolution=10)
# points.mlab_source.dataset.lines = np.array(connections)

#from enthought.mayavi.modules.labels import Labels

#labels = mlab.pipeline.labels(mlab.line_source)
#labels.property.font_size = 40

# also text3d module to position labels?
#
#
#from enthought.mayavi import mlab
#import numpy as np
#x = np.random.random((5,))
#y = np.random.random((5,))
#z = np.random.random((5,))
#
#mlab.clf()
#points = mlab.points3d(x, y, z, mode='sphere', scale_factor=0.1,
#opacity=0.5)
#
## Label the points with their ID
#labels = mlab.pipeline.labels(points)
#labels.mapper.label_format = '%.f'
#
## Connect point 1 and 2, as well as 3 and 4
#points.mlab_source.dataset.lines = np.array(((1, 2),(2,3), (3, 4)))
#points.mlab_source.update()

# choose for a scalar value to display different modalities for edges
#
#src.image_data.point_data.add_array(np.angle(Phi).T.ravel())
## We need to give a name to our new dataset.
#src.image_data.point_data.get_array(1).name = 'angle'
## We select the 'scalar' attribute, ie the norm of Phi
#src2 = mlab.pipeline.set_active_attribute(src, point_scalars='scalar')

# And now display the lines
#tubes = mlab.pipeline.surface(mlab.pipeline.tube(points, tube_radius=0.01),
#                              colormap='jet', opacity=0.6)
#lines = mlab.pipeline.surface(points)
#

#CellToPointData

#mlab.show()
# ADD: remapping of the node ids for the edges
# ADD: labeling with text
#
##from enthought.mayavi import mlab
#import numpy as np
#x = np.random.random((5,))
#y = np.random.random((5,))
#z = np.random.random((5,))
#
#radius = np.random.random((5,))
#
#mlab.clf()
#points = mlab.points3d(x, y, z, 0.5 + radius,
#        mode='sphere', scale_factor=0.1, opacity=0.5)
## Turn off not absolute scaling
#points.glyph.glyph.clamping = False
#
#
## Label the points with their ID
#labels = mlab.pipeline.labels(points)
#labels.mapper.label_format = '%.f'
##
## Connect point 1 and 2, as well as 3 and 4
#line_connections = np.array(((1, 2), (3, 4)))
#
## The color and radius size at each vertex.
#line_scalars     = np.random.random((5,))
#
#lines = mlab.pipeline.scalar_scatter(x, y, z, line_scalars)
#lines.data.lines = line_connections
#lines.update()
#
## And now display the lines
#tubes = mlab.pipeline.tube(lines, tube_radius=0.005)
#tubes.filter.vary_radius = 'vary_radius_by_scalar'
#tube_surfaces = mlab.pipeline.surface(tubes)
