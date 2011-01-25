surfscript = """
# Imports
from enthought.mayavi import mlab

# Retrieve data from connectome file
surface_file_vertices = cfile.obj.get_by_name("%s")
vertices = surface_file_vertices.data.darrays[%s].data

surface_file_faces = cfile.obj.get_by_name("%s")
faces = surface_file_faces.data.darrays[%s].data

# Ensure that (triangluar) faces have dimension (N,3)
if len(faces.shape) == 1:
    faces = faces.reshape( (len(faces) / 3, 3) )

surface_file_labels = cfile.obj.get_by_name("%s")
if surface_file_labels is None:
    labels = None
else:
    labels = surface_file_labels.data.darrays[%s].data
    # Ensure correct dimension (1-D) for labels
    labels = labels.ravel()
    # Ensure that each vertices has a corresponding label
    assert vertices.shape[0] == len(labels)

# Create triangular mesh
x, y, z = vertices[:,0], vertices[:,1], vertices[:,2]
mlab.triangular_mesh(x, y, z, faces, scalars = labels)

"""

netscript = """
import numpy as np
from enthought.mayavi import mlab
from enthought.tvtk.api import tvtk

# Retrieve graph
g = cfile.obj.get_by_name("%s").data
position_key = "%s"
edge_key = "%s"
node_label_key = "%s"

# Node ids you want to create labels for
create_label = []

# Assume node id's are integers
nr_nodes = len(g.nodes())
position_array = np.zeros( (nr_nodes, 3) )
for i,nodeid in enumerate(g.nodes()):
    pos = g.node[nodeid][position_key]
    # apply a conversion procedure if the position
    # is a tuple store as string
    # we need a numpy array in the end
    pos = tuple(float(s) for s in pos[1:-1].split(','))
    pos = np.array(pos)
    position_array[i,:] = pos

x, y, z = position_array[:,0], position_array[:,1], position_array[:,2]

# Retrieve the edges and create a Numpy array
edges = np.array(g.edges())
nr_edges = len(edges)

# Retrieve edge values
ev = np.zeros( (nr_edges, 1) )
for i,d in enumerate(g.edges_iter(data=True)):
    ev[i] = d[2][edge_key]
    # ensure that we are setting the correct edge
    assert d[0] == edges[i,0] and d[1] == edges[i,1]

# Need to subtract one because the array index starts at zero
edges = edges - 1

# Create vectors which will become edges
start_positions = position_array[edges[:, 0], :].T
end_positions = position_array[edges[:, 1], :].T
vectors = end_positions - start_positions

# Create visualization
# --------------------
# create a new figure
mlab.figure()

nodesource = mlab.pipeline.scalar_scatter(x, y, z, name = 'Node Source')
nodes = mlab.pipeline.glyph(nodesource, scale_factor=3.0, scale_mode='none',\
                              name = 'Nodes', mode='cube')
nodes.glyph.color_mode = 'color_by_scalar'

vectorsrc = mlab.pipeline.vector_scatter(start_positions[0], 
                             start_positions[1],
                             start_positions[2],
                             vectors[0],
                             vectors[1],
                             vectors[2],
                             name = 'Connectivity Source')

# Add scalar array
da = tvtk.DoubleArray(name=edge_key)
da.from_array(ev)
            
vectorsrc.mlab_source.dataset.point_data.add_array(da)
vectorsrc.mlab_source.dataset.point_data.scalars = da.to_array()
vectorsrc.mlab_source.dataset.point_data.scalars.name = edge_key

# need to update the boundaries
vectorsrc.outputs[0].update()

# Add a thresholding filter
thres = mlab.pipeline.threshold(vectorsrc, name="Thresholding")

myvectors = mlab.pipeline.vectors(thres,colormap='OrRd',
                                            #mode='cylinder',
                                            name='Connections',
                                            #scale_factor=1,
                                            #resolution=20,
                                            # make the opacity of the actor depend on the scalar.
                                            #transparent=True,
                                            scale_mode = 'vector')

myvectors.glyph.glyph_source.glyph_source.glyph_type = 'dash'
# vectors.glyph.glyph_source.glyph_source.radius = 0.01
myvectors.glyph.color_mode = 'color_by_scalar'
myvectors.glyph.glyph.clamping = False

# create labels
for la in create_label:
    row_index = la - 1
    label = g.node[la][node_label_key]
    mlab.text3d(position_array[row_index,0],
                position_array[row_index,1],
                position_array[row_index,2],
                '     ' + label,
                name = 'Node ' + label)

"""

nbsscript = """
# Imports
import numpy as np
import cviewer.libs.pyconto.algorithms.statistics.nbs as nbs
from pylab import imshow, show, title
import networkx as nx

# Define your groups
# Retrieve the corresponding CNetwork objects
firstgroup = cfile.obj.get_by_name(%s)
first_edge_value = '%s'
secondgroup = cfile.obj.get_by_name(%s)
second_edge_value = '%s'
THRESH=%s
K=%s
TAIL='%s'
SHOW_MATRIX = False

# ===========

# Make sure that all networks are loaded in memory
for net in firstgroup:
    net.load()
for net in secondgroup:
    net.load()

# Convert your network data for each group into numpy arrays
nr1_networks = len(firstgroup)
nr1_nrnodes = len(firstgroup[0].data.nodes())

nr2_networks = len(secondgroup)
nr2_nrnodes = len(secondgroup[0].data.nodes())

X = np.zeros( (nr1_nrnodes, nr1_nrnodes, nr1_networks) )
Y = np.zeros( (nr2_nrnodes, nr2_nrnodes, nr2_networks) )

# Fill in the data from the networks
for i, sub in enumerate(firstgroup):
    graph=sub.data
    # Setting the edge requested edge value as weight value
    for u,v,d in graph.edges(data=True):
        graph[u][v]['weight']=d[first_edge_value]
    # Retrieve the matrix
    X[:,:,i] = nx.to_numpy_matrix(graph)

for i, sub in enumerate(secondgroup):
    graph=sub.data
    # Setting the edge requested edge value as weight value
    for u,v,d in graph.edges(data=True):
        graph[u][v]['weight']=d[second_edge_value]
    # Retrieve the matrix
    Y[:,:,i] = nx.to_numpy_matrix(graph)

# Compute NBS, this might take a long time
# you can run in the background...
PVAL, ADJ, NULL = nbs.compute_nbs(X,Y,THRESH,K,TAIL)

# We can now look at the connectivity matrix identified with matplotlib
if SHOW_MATRIX:
    imshow(ADJ, interpolation='nearest')
    title('Edges identified by the NBS')
    show()

# we create a networkx graph again from the adjacency matrix
nbsgraph = nx.from_numpy_matrix(ADJ)
# relabel nodes because the should not start at zero for our convention
nbsgraph=nx.relabel_nodes(nbsgraph, lambda x: x + 1)
# populate node dictionaries with attributes from first network of the first group
# it must include some location information to display it
for nid, ndata in firstgroup[0].data.nodes_iter(data=True):
    nbsgraph.node[nid] = ndata

# You can now add now the results to the connectome file
# Make sure that the name is not existing yet in the connectome file
cfile.obj.add_connectome_network_from_nxgraph('NBS result at %s', nbsgraph, dtype='NBSResult')
cfile.update_children()
"""

volrendering = """
from enthought.mayavi import mlab
import numpy as np
data=np.random.random( (10,10,10))
min = data.min()
max = data.max()
source=mlab.pipeline.scalar_field(data)
vol = mlab.pipeline.volume(source, vmin=min+0.65*(max-min), 
                                   vmax=min+0.9*(max-min))
"""

volslice = """
from enthought.mayavi import mlab
import numpy as np

volname="%s"

# Retrieve volume data (as Nibabel Image)
voldat = cfile.obj.get_by_name(volname).data
# Retrieve the image data
data = voldat.get_data()
# Retrieve the affine
affine = voldat.get_affine()
center = np.r_[0, 0, 0, 1]
# Create A ScalarField with spacing and origin from
# the affine
data_src = mlab.pipeline.scalar_field(data)
data_src.spacing = np.diag(affine)[:3]
data_src.origin = np.dot(affine, center)[:3]
# Create an outlint
mlab.pipeline.outline(data_src)
# Create a simple x-aligned image plane widget
image_plane_widget = mlab.pipeline.image_plane_widget(data_src, name=volname)
image_plane_widget.ipw.plane_orientation = 'x_axes'
image_plane_widget.ipw.reslice_interpolate = 'nearest_neighbour'                    
"""

reportlab = """
from reportlab.pdfgen import canvas
 
c = canvas.Canvas("hello.pdf")
c.drawString(100,750,"Welcome to Reportlab!")
c.save()
http://www.protocolostomy.com/2008/10/22/generating-reports-with-charts-using-python-reportlab/
"""