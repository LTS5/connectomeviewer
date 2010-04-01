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
        edges: 2D ndarray. 
            The indices of the edges of the Delaunay triangulation as a 
            (2, N) array [[pair1_index1, pair2_index1, ...],
                          [pair1_index2, pair2_index2, .. ]]
    """
    x, y, z = xyz
    if visualize:
        vtk_source =  mlab.points3d(x, y, z, opacity=0)
    else:
        vtk_source =  mlab.points3d(x, y, z, figure=False)
    delaunay =  mlab.pipeline.delaunay3d(vtk_source)
    edges = mlab.pipeline.extract_edges(delaunay)
    if visualize:
        mlab.pipeline.surface(edges, opacity=0.05)

    lines = edges.outputs[0].lines.to_array()
    return np.array([lines[1::3], lines[2::3]])


dist = lambda b1,b2: np.sqrt((b1.x-b2.x)**2 + (b1.y-b2.y)**2 + (b1.z-b2.z)**2)


class Px:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

def prim(pxs):
    '''
    Form Minimum Spanning Tree using Prim's algorithm.
    Code from Ronald Rivest <http://people.csail.mit.edu/rivest/programs.html>
    '''
    mst_edge_list = []

    b0 = pxs[0]
    Q = pxs[1:]
    for Px in Q: 
        Px.d = dist(Px,b0)
        Px.pred = b0

    while Q != []:
        min_d = 1e20
        for Px in Q:
            if Px.d < min_d:
                min_d = Px.d
                closest_Px = Px
        Q.remove(closest_Px)
        b0 = closest_Px
        b1 = closest_Px.pred
        mst_edge_list.append((b0,b1))
        for Px in Q:
            d = dist(Px,closest_Px)
            if d<Px.d:
                Px.d = d
                Px.pred = closest_Px
    return mst_edge_list


def build_nx(edge_list):
    g = nx.Graph()
    g.add_edges_from([((i.x,i.y,i.z),(j.x,j.y,j.z)) for j,i in edge_list])
    return g


def nx_graph(xyz, edges):
    ''' Build a NetworkX graph with xyz node coordinates and the node indices of the end nodes.
    xyz is a 3 row by N column array of x,y,z point coordinates ordered by node index.
    edges is the (2, N) array returned by compute_delaunay_edges() containing node indices of the end nodes.
    weights are applied to the edges based on the euclidean length of the edge for use by the MST algorithm.
    '''
    def dist(i,j):
        d = xyz[:,i] - xyz[:,j]
        return np.sqrt(np.dot(d, d))

    g = nx.Graph()
    g.add_edges_from([(i,j,dist(i,j)) for i,j in edges.T.reshape(-1,2)])
    return g


if __name__ == '__main__':
    USE_RIVEST = False
    N = 400
    xyz = np.random.random((3, N))
    edges = compute_delaunay_edges(xyz, visualize=True)

    vecs = []
    if USE_RIVEST:
        pxs = [Px(*i) for i in xyz.T]                       # a list of pixel coordinates
        edge_list = prim(pxs)                               # form MST using Prim's algorithm
        for edge in edge_list:
            px1, px2 = edge
            vecs.append(np.hstack([px1.x, px1.y, px1.z, px2.x-px1.x, px2.y-px1.y, px2.z-px1.z]))
    else:
        g = nx_graph(xyz, edges)
        graph = nx.mst(g)
        for edge in graph:
            vecs.append(np.hstack([xyz[:,edge[0]], xyz[:,edge[1]]-xyz[:,edge[0]]]))

    x, y, z, u, v, w = np.vstack(vecs).T
    src = mlab.pipeline.vector_scatter(x, y, z, u, v, w)
    vec = mlab.pipeline.vectors(src, opacity = 0.3)
    vecgl = vec.glyph
    vecgl.glyph_source.glyph_source.glyph_type = 'dash'
    #~ vecgl.color_mode = 'no_coloring'
    vecgl.glyph.clamping = False

    mlab.gcf().scene.background = (0,0,0)         # black background
    mlab.show()
