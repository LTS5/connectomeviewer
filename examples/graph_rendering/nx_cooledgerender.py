""" Show all edges with a low opacity """

import numpy as np
import networkx as nx
from enthought.mayavi import mlab

def show_edges(xyz, graph_edge_gen, opacity=0.8):
    """ Show the graph edges using Mayavi

        Parameters
        -----------
        xyz: ndarray
            3 rows x N columns of x,y,z coordinates of the points.
        graph_edge_gen:
            An iterator that returns the edges to be shown. The first two
            values in each edge tuple contain the indices into the xyz data.
        opacity:
            The opacity of the edges
    """
    vecs = []
    for edge in graph_edge_gen:
        vecs.append(
            np.hstack([xyz[:,edge[0]], xyz[:,edge[1]]-xyz[:,edge[0]]]))
    vecs = np.vstack(vecs)          # Data columns are x, y, z, u, v, w 

    src = mlab.pipeline.vector_scatter(*vecs.T)
    vec = mlab.pipeline.vectors(src, opacity=opacity)
    vecgl = vec.glyph
    vecgl.glyph_source.glyph_source.glyph_type = 'dash'
    vecgl.glyph.clamping = False

a=np.array(scalarscatter.data.points)

# select the graph of the first network
g = cfile.networks[0].graph

def nstrip(str):
    return int(str.lstrip('n'))-1

H = nx.relabel_nodes(g, nstrip)

# only select edges with high adc

show_edges(a.T, H.edges_iter(), opacity=0.1)


