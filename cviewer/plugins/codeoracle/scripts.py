surfscript = """
# Importing Mayavi mlab interface
from enthought.mayavi import mlab


# Retrieving the data
# -------------------

# surface data from connectome file
surface_file_vertices = cfile.obj.get_by_name("%s")
vertices = surface_file_vertices.data.darrays[%s].data
surface_file_faces = cfile.obj.get_by_name("%s")
faces = surface_file_faces.data.darrays[%s].data
surface_file_labels = cfile.obj.get_by_name("%s")

# Sanity check
# ------------

# ensure that (triangluar) faces have dimension (N,3)
if len(faces.shape) == 1:
    faces = faces.reshape( (len(faces) / 3, 3) )

# check for labels
if surface_file_labels is None:
    labels = None
else:
    labels = surface_file_labels.data.darrays[%s].data
    # Ensure correct dimension (1-D) for labels
    labels = labels.ravel()
    # Ensure that each vertices has a corresponding label
    assert vertices.shape[0] == len(labels)

# Perform task
# ------------

# Create triangular surface mesh
x, y, z = vertices[:,0], vertices[:,1], vertices[:,2]
mlab.triangular_mesh(x, y, z, faces, scalars = labels)

"""

conmatrix = """
# Importing NetworkX
import networkx as nx
# Import the Connectome Matrix Viewer
from cviewer.visualization.matrix.con_matrix_viewer import ConnectionMatrixViewer

# Retrieving the data
# -------------------

# retrieve the graph
g = cfile.obj.get_by_name("%s").data

# set the node key to use the labels
nodelabelkey = "%s"

# Defining some helper functions
# ------------------------------

def relabel_to_int(graph):
    " Relabel string node ids to integer "
    def intmap(x): return int(x)
    return nx.relabel_nodes(graph,intmap)

def get_nodelabels(graph, nodekey = 'dn_label'):
    " Retrieve a list of node labels "
    g = relabel_to_int(graph)
    a = []
    return [v[nodekey] for n,v in g.nodes_iter(data=True)]

def get_edge_values(graph):
    " Retrieve valid edge keys "
    if len(graph.edges()) == 0:
        return
    edi = graph.edges_iter(data=True)
    u,v,ed = edi.next()
    ret = []
    for k,v in ed.items():
        if isinstance(v, float) or isinstance(v, int):
            ret.append(k)
    return ret

def get_matdict(graph):
    matdict = {}
    g = relabel_to_int(graph)
    # grab keys from the first edge, discarding id
    dl = get_edge_values(g)
    # create numpy matrix for each key using recarray
    matrec = nx.to_numpy_recarray(g, dtype=zip(dl, [float]*len(dl)) )
    for k in dl:
        matdict[k] = matrec[k]
    return matdict

def invoke_matrix_viewer(graph, nodelabelkey = 'dn_label'):
    " Invoke the Connectome Matrix Viewer "
    cmatrix_viewer = ConnectionMatrixViewer(get_nodelabels(graph, nodekey = nodelabelkey),
                                  get_matdict(graph))
    cmatrix_viewer.edit_traits()

# Perform task
# ------------

invoke_matrix_viewer(g, nodelabelkey)
"""

conmatrixpyplot = """
# Importing NetworkX
import networkx as nx
# Import Pylab
from pylab import imshow, show, cm

# Retrieving the data
# -------------------

# retrieve the graph
g = cfile.obj.get_by_name("%s").data

# define the edge key to plot
edgekey = "%s"

# binarize matrix
binarize = False

# Defining some helper functions
# ------------------------------

def show_matrix(a, edge, binarize = False):
    for u,v,d in a.edges_iter(data=True):
        a.edge[u][v]['weight'] = a.edge[u][v][edge]
    bb=nx.to_numpy_matrix(a)
    if binarize:
        c=np.zeros(bb.shape)
        c[bb>0] = 1
        b = c
    else:
        b = bb
    imshow(b, interpolation='nearest', cmap=cm.jet, vmin = b.min(), vmax=b.max())
    show()

# Perform task
# ------------
show_matrix(g, edgekey, binarize)

"""

ctrackedge = """
# Importing Numpy
import numpy as np
# Import Dipy Visualization
import dipy.viz.fvtk as fvtk

# Retrieving the data and set parameters
# --------------------------------------

a = cfile.obj.get_by_name("Filtered Tractography").get_fibers_as_numpy()
fiberlabels = cfile.obj.get_by_name("Fiber labels (freesurferaparc)").data

fromtofloatid = 8.10


# Defining some helper functions
# ------------------------------

def sidx(arr, value):
    " Returns the indices that are equal to a given value "
    return np.where( arr == value)[0]

def randcolarr(arr):
    " Returns a random color for each row in arr "
    return np.random.rand(1,3).repeat(len(arr),axis=0)
    
def showfibfvtk(fibarr, colarr, percentage = 100):
    fibarr2 = fibarr[::percentage]
    colarr2 = colarr[::percentage]
    fibarr2list = fibarr2.tolist()
    r=fvtk.ren()
    #fvtk.add(r,fvtk.axes())
    r.SetBackground(1, 1, 1)
    [fvtk.add(r,fvtk.line(ele, colarr2[i,:])) for i, ele in enumerate(fibarr2list)];
    fvtk.show(r, title = "Fibers", size = (500,500))
    

# Perform task
# ------------

idx = sidx(fiberlabels, fromtofloatid)
showfibfvtk(a[idx], randcolarr(a[idx]), 100)

"""

netscript = """
# Importing NumPy
import numpy as np
# Importing Mayavi mlab and tvtk packages
from enthought.mayavi import mlab
from enthought.tvtk.api import tvtk

# Retrieving the data and set parameters
# --------------------------------------

# load graph data
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

# Perform task
# ------------

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

# add scalar array
da = tvtk.DoubleArray(name=edge_key)
da.from_array(ev)
            
vectorsrc.mlab_source.dataset.point_data.add_array(da)
vectorsrc.mlab_source.dataset.point_data.scalars = da.to_array()
vectorsrc.mlab_source.dataset.point_data.scalars.name = edge_key

# need to update the boundaries
vectorsrc.outputs[0].update()

# Add a thresholding filter to threshold the edges
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
# Import Numpy
import numpy as np
# Import pylab for plotting
from pylab import imshow, show, title
# Import NetworkX
import networkx as nx
# Import Network based statistic
import cviewer.libs.pyconto.algorithms.statistics.nbs as nbs


# Retrieving the data and set parameters
# --------------------------------------

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

# Perform task
# ------------

# Compute NBS, this might take a long time
# and might better be done in a thread

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
# Import Mayavi mlab interface
from enthought.mayavi import mlab
# Import NumPy
import numpy as np

# Retrieving the data and set parameters
# --------------------------------------

# the CVolume name
volname="%s"

# Retrieve volume data (as Nibabel Image)
voldat = cfile.obj.get_by_name(volname).data

# Retrieve the image data
data = voldat.get_data()

# Retrieve the affine
affine = voldat.get_affine()
center = np.r_[0, 0, 0, 1]

# Perform task
# ------------

# create A ScalarField with spacing and origin from the affine
data_src = mlab.pipeline.scalar_field(data)
data_src.spacing = np.diag(affine)[:3]
data_src.origin = np.dot(affine, center)[:3]

# Create an outline
mlab.pipeline.outline(data_src)

# Create a simple x-aligned image plane widget
image_plane_widget = mlab.pipeline.image_plane_widget(data_src, name=volname)
image_plane_widget.ipw.plane_orientation = 'x_axes'
image_plane_widget.ipw.reslice_interpolate = 'nearest_neighbour'                    
"""

reportlab = """
# Credits
# http://www.protocolostomy.com/2008/10/22/generating-reports-with-charts-using-python-reportlab/

# Import ReportLab
from reportlab.platypus import *
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.rl_config import defaultPageSize
from reportlab.lib.units import inch
# Import NetworkX
import networkx

# Retrieving the data and set parameters
# --------------------------------------

PAGE_HEIGHT=defaultPageSize[1]
styles = getSampleStyleSheet()
Title = "Connectome Report: "
URL = "http://www.connectomics.org/"
email = "info@connectomics.org"

Elements=[]
HeaderStyle = styles["Heading1"]
ParaStyle = styles["Normal"]
PreStyle = styles["Code"]

# load data
net=a.get_by_name("%s")
net.load()
g=net.data
netw = g
 
def header(txt, style=HeaderStyle, klass=Paragraph, sep=0.3):
    s = Spacer(0.2*inch, sep*inch)
    para = klass(txt, style)
    sect = [s, para]
    result = KeepTogether(sect)
    return result
 
def p(txt):
    return header(txt, style=ParaStyle, sep=0.1)
 
def pre(txt):
    s = Spacer(0.1*inch, 0.1*inch)
    p = Preformatted(txt, PreStyle)
    precomps = [s,p]
    result = KeepTogether(precomps)
    return result
 
def go():
    doc = SimpleDocTemplate('gfe.pdf')
    doc.build(Elements)
 
mytitle = header(Title + a.get_connectome_meta().get_title())

mysite = header(URL, sep=0.1, style=ParaStyle)
mymail = header(email, sep=0.1, style=ParaStyle)

myabstract = p(Abstract)
head_info = [mytitle, mysite, mymail]

 
code_title = header("Basic code to produce output")
code_explain = p("This is a snippet of code. It's an example using the Preformatted flowable object, which
                 makes it easy to put code into your documents. Enjoy!")


mytitlenet = header(net.get_name())

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import networkx as nx

for u,v,d in netw.edges_iter(data=True):
    netw.edge[u][v]['weight'] = netw.edge[u][v]['de_adcmean']

b=nx.to_numpy_matrix(netw)
fig = plt.figure()
fig.suptitle("Connection matrix")
aa= plt.imshow(b, interpolation='nearest', cmap=plt.cm.jet, vmin = b.min(), vmax=b.max())
fig.savefig('matrix.png')

fig.clear()
fig.suptitle("Degree distribution")
plt.hist(netw.degree().values(),30)
fig.savefig('distri.png')

# measures

me1 = p("Number of Nodes: " + str(netw.number_of_nodes()))
me2 = p("Number of Edges: " +  str(netw.number_of_edges()))
me3 = p("Is network connected: " + str(nx.is_connected(netw)))
me4 = p("Number of connected components: " + str(nx.number_connected_components(netw)))
me5 = p("Average unweighted shortest path length: " + str(nx.average_shortest_path_length(netw, weighted = False)))
me6 = p("Average Clustering Coefficient: " + str(nx.average_clustering(netw)))

logo = "matrix.png"
im1 = Image(logo, 300,225)
logo = "distri.png"
im2 = Image(logo, 250,188)

codesection = [mytitle, mysite, mymail, mytitlenet, im1, im2, me1, me2, me3, me4, me5, me6]
src = KeepTogether(codesection)
Elements.append(src)
go()
"""