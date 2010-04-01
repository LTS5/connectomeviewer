#!/usr/bin/env python


def visualize_graph(nodeids, labels, edges, scalars, \
                    positions, colors, edgecolors, directed = False ):
    """ Visualizes the given graph in 3-space
    
    Keyword arguments:
    nodeids -- the nodes unique identifier, list 0..(n-1)
    labels -- according to the enumeration, list (n-1)
    edges -- k edges connectivity based on nodeids (k-1)x2 for k edges
    scalars_per_edge -- scalar value for every row in the edges, list (k-1)x1
    positions -- array of positions for every node, array (n-1)x3
    colors -- list of colorcode tuples (R,G,B) for the nodeids, list (n-1)
    
    """
    from enthought.mayavi import mlab
    from enthought.tvtk.api import tvtk
    
    #from vtk.util import colors
    
    from enthought.tvtk.pyface import actors
    import random
    
    fig = mlab.figure(1)
    
    # normalizing colors (R,G,B) from 0 to 255 -> float
    colors_normalized = []
    for ctp in colors:
        colors_normalized.append( (ctp[0]/255.,ctp[1]/255., ctp[2]/255.) )
        
    # add nodes as sphere actor with
    # color@position, default radius and resolution
    logger.info(nodeids)
    logger.info('hello')
    actors_list = [actors.sphere_actor(center = (positions[l,0], positions[l,1], positions[l,0]), \
                                       radius=0.05, color=colors_normalized[l] ) for l in nodeids]
    
    for i in actors_list:
        # acctors don't show up as source!
        # maybe create Glyph sources??
        fig.scene.add_actor( i )
    
    # add lines / tubes, arrow_actors
#def axes_actor(origin=(0, 0, 0), scale_factor=1.0, radius=0.02,
#28 	               sides=12):
#29 	    """Creates a simple axes actor and returns a tvtk.Actor object."""
#30 	    axes = tvtk.Axes(origin=origin, scale_factor=scale_factor, symmetric=1)
#31 	    tube = tvtk.TubeFilter(radius=radius, number_of_sides=sides,
#32 	                           vary_radius='vary_radius_off',
#33 	                           input=axes.output)
#34 	    mapper = tvtk.PolyDataMapper(input=tube.output)
#35 	    actor = tvtk.Actor(mapper=mapper)

    

    edge_colors_normalized = []
    for ctp in edgecolors:
        edge_colors_normalized.append( (ctp[0]/255.,ctp[1]/255., ctp[2]/255.) )
        
    # Create a line between the two spheres
    for i,edge in enumerate(edges):
        line = tvtk.LineSource(point1=positions[edge[0],:], point2=positions[edge[1],:])
        logger.info(line)
        tube = tvtk.TubeFilter(radius=scalars[i], number_of_sides=12,\
                              vary_radius='vary_radius_off',\
                              #scalar=scalars,
                              input=line.output)
        mapper = tvtk.PolyDataMapper(input=tube.output)
        p = tvtk.Property(color=edge_colors_normalized[i])
        tubeactor=tvtk.Actor(mapper=mapper,property=p)
        fig.scene.add_actor(tubeactor)
    
    
    # add text/labels
    
    fig.scene.reset_zoom()    
    mlab.show()
    
   
    # mix mlab and tvtk?
     
    
    # try picking example
    
    # try to get hold of th actors
    
    # try to change the attributes of the connections
    
    
    
    


if __name__ == '__main__':
    
    import numpy as np
    import logging
    logger = logging.getLogger()
    logger.setLevel(logging.info)
    
    nodeids = [0, 1, 2, 3]
    labels = ['Node 1', 'Node 2', 'Node 3', 'Node 3']
    rgbs = [(255,0,0), (0,255,0), (0,0,255), (100,100,100)]
    #positions = np.array([ [2,5,6], [2,6,7], [5,6,6], [1,4,3] ])
    positions = np.array([[0,0,0],[1,1,1],[2,2,2],[1,2,3]])
    
    edges = np.array([ [0,3], [1,2], [2,3]])
    scalars_per_edge = [0.01, 0.01,0.03]
    edgecolors= [(255,0,0), (0,255,0), (0,0,255)]

    # ready to visualize :-)
    visualize_graph(nodeids=nodeids, labels=labels, edges=edges, \
                    scalars=scalars_per_edge, positions=positions, \
                    colors=rgbs,edgecolors=edgecolors)
    
