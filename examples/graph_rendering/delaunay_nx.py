#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
An example illustrating graph manipulation and display with Mayavi
and NetworkX.

Starting from points randomly positioned around a plane or regularly on a
sphere, we first use VTK to create the Delaunay graph, and also to plot it.
We then create a matching NetworkX graph, calling it's minimum spanning tree
function. We display it adding connections to Mayavi points.

The visualization clearly shows that the minimum spanning tree of the
points, considering all possible connections, is included in the Delaunay
graph.
"""
# Author: Gary Ruben
#         Gael Varoquaux <gael dot varoquaux at normalesup dot org>
# Copyright (c) 2009, Enthought, Inc.
# License: BSD style.

from enthought.mayavi import mlab
import numpy as np
import networkx as nx

def compute_delaunay_edges(xyz, visualize=False):
    """ Given 3-D points, returns the edges of their 
        Delaunay triangulation.

        Parameters
        -----------
        xyz: ndarray
            3 rows x N columns of x, y, z coordinates of the points

        Returns
        ---------
        points: 3 rows x N columns of x, y, z coordinates of the points
                (same coords but different assignment of points)
        edges: 2D ndarray.
            The indices of the edges of the Delaunay triangulation as a
            (2, N) array [[pair1_index1, pair2_index1, ...],
                          [pair1_index2, pair2_index2, .. ]]
    """
    x, y, z = xyz
    if visualize:
        vtk_source = mlab.points3d(x, y, z, opacity=0)
    else:
        vtk_source = mlab.points3d(x, y, z, figure=False)
    delaunay =  mlab.pipeline.delaunay3d(vtk_source)
    delaunay.filter.offset = 999    # seems more reliable than the default
    edges = mlab.pipeline.extract_edges(delaunay)
    if visualize:
        mlab.pipeline.surface(edges, opacity=0.05)

    points = edges.outputs[0].points.to_array().T
    lines = edges.outputs[0].lines.to_array()
    return points, np.array([lines[1::3], lines[2::3]])


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


def build_geometric_graph(xyz, edges):
    """ Build a NetworkX graph with xyz node coordinates and the node indices
        of the end nodes.

        Parameters
        -----------
        xyz: a 3 row by N column array of x,y,z point coordinates ordered
            by node index. We don't bother putting these into the NX graph.
            Instead the graph node is an index to the column.
        edges: the (2, N) array returned by compute_delaunay_edges()
            containing node indices of the end nodes. Weights are applied to
            the edges based on their euclidean length for use by the MST
            algorithm.

        Returns
        ---------
        g: A NetworkX undirected graph
    """
    def euclidean_dist(i,j):
        d = xyz[:,i] - xyz[:,j]
        return np.sqrt(np.dot(d, d))

    g = nx.Graph()
    g.add_weighted_edges_from([(i,j,euclidean_dist(i,j))
                                for i,j in edges.T.reshape(-1,2)])
    return g


def points_on_sphere(N):
    """ Generate N evenly distributed points on the unit sphere centered at
        the origin. Uses the 'Golden Spiral'.
        Code by Chris Colbert from the numpy-discussion list.
    """
    phi = (1 + np.sqrt(5)) / 2  # the golden ratio
    long_incr = 2*np.pi / phi   # how much to increment the longitude

    dz = 2.0 / float(N)         # a unit sphere has diameter 2
    bands = np.arange(N)        # each band will have one point placed on it
    z = bands * dz - 1 + (dz/2) # the height z of each band/point
    r = np.sqrt(1 - z*z)        # project onto xy-plane
    az = bands * long_incr      # azimuthal angle of point modulo 2 pi
    x = r * np.cos(az)
    y = r * np.sin(az)
    return np.vstack((x,y,z))


if __name__ == '__main__':
    # FLATNESS = 0.2
    # FLATNESS = 2.0
    FLATNESS = 1.01   # Exactly 1.0 causes triangulation problems on the sphere
    CULL_EDGES = False
    N = 10

    # generate some points
    np.random.seed(0)
    if 0:
        xyz = np.random.random((3, N))
    else:
        xyz = points_on_sphere(N)
        ## xyz = np.hstack((xyz, np.c_[[0.,0.,0.]]))    # add the origin
    xyz[-1, :] *= FLATNESS

    # Now get the Delaunay Triangulation from vtk via mayavi mlab. Vtk stores
    # its points in a different order so overwrite ours to match the edges
    new_xyz, edges = compute_delaunay_edges(xyz, visualize=not CULL_EDGES)
    assert(xyz.shape == new_xyz.shape)   # check triangulation got everything
    xyz = new_xyz

    # Make a NetworkX graph out of our point and edge data
    g = build_geometric_graph(xyz, edges)

    if 0:
        # remove long internal edges
        alpha = 1.5         # factor >1 that controls which edges are culled
        edge_length_cutoff = alpha*(2*np.sqrt(np.pi/N))     # r = 1
        edge_removal_list = [e for e in g.edges_iter(data=True)
                             if e[2]['weight'] > edge_length_cutoff]
        g.remove_edges_from(edge_removal_list)
        show_edges(xyz, g.edges_iter(), opacity=0.1)

    # compute minimum spanning tree
    graph_edge_gen = nx.mst(g)        # "graph_edge_gen" is an edge generator
    show_edges(xyz, graph_edge_gen)

    mlab.gcf().scene.background = (0,0,0)         # black background
    mlab.show()