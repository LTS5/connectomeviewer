writegexf = '''
""" Convert a connectome network to the GEXF format
readable by Gephi for further analysis and visualization """

import networkx as nx
import numpy as np

cnet = cfile.obj.get_by_name('connectome_freesurferaparc')
cnet.load()
outpath = 'out.gexf'

def fix_float_for_gexf(network):
    for u,v,d in network.edges_iter(data=True):
        for k,v in d.items():
            if isinstance(d[k], np.float64):
                d[k] = float( d[k] )
    for u,d in network.nodes_iter(data=True):
        for k,v in d.items():
            if isinstance(d[k], np.float64):
                d[k] = float( d[k] )
    return network

G = fix_float_for_gexf(cnet.data)
nx.write_gexf(cnet.data, outpath)
'''

corticocortico ='''
""" Extract cortio-cortico fibers from tractography outputs of CMTK,
apply clustering and visualize them. The basic idea is to remove sets of
fibers such as the corpus callosum first, and then inspect various histograms
of start-endpoint-distances, mean curvature etc. and downsample to
incrementally filter out fibers until only cortico-cortico fibers remain.
The remaining fibers are then clustered to yield bundles of cortico-cortical fibers.

This script shows the outcome of the power of interactively working with
scientific datasets for exploration and parameter estimation.

Stephan Gerhard, May 2011 """

import numpy as np
import nibabel as ni
from os.path import join as j
import os.path as op
import time

from nibabel import trackvis
import cfflib as cf

from dipy.tracking import metrics as tm
from dipy.tracking import distances as td
from dipy.io import pickles as pkl
from dipy.tracking.metrics import length, mean_curvature
import dipy.viz.fvtk as fvtk

from fos import World, Window
from fos.actor.curve import InteractiveCurves

# Load the relevant data files
# load connectome file object

con = cfile.obj
# alternatively when loading from Ipython directly with cfflib
# con = cf.load('meta.cml')

def load_data(con):

    fibobj = con.get_by_name('Final Tractography (freesurferaparc)')
    fibobj.load()
    fibers = fibobj.get_fibers_as_numpy()
    fibershdr = fibers.data[1]

    epmmobj = con.get_by_name('Fiber mean curvature')

    eleobj = con.get_by_name('Final fiber lengths (freesurferaparc)')
    eleobj.load()
    lenghts = eleobj.data

    emobj = con.get_by_name('Fiber mean curvature')
    emobj.load()
    meancurv = emobj.data

    print "Compute endpoints [mm] array"
    endpoints = np.zeros( (len(fibers), 2, 3), dtype = np.float32 )
    for i in xrange(len(fibers)):
        endpoints[i,0,:] = fibers[i][0,:]
        endpoints[i,1,:] = fibers[i][-1,:]

    return fibers, fibershdr, lenghts, meancurv, endpoints

# load stuff, can uncomment for rerunning the script in ipython with
# run -i script.py
fibers, fibershdr, lenghts, meancurv, endpoints = load_data(con)

# Helper functions

def compute_start_end_distances(endpoints):
    n = endpoints.shape[0]
    di = np.zeros( (n, 1) )
    for i in xrange(n):
        d = np.abs(endpoints[i,0,:]-endpoints[i,1,:])
        di[i,0] = np.sqrt(np.dot(d,d))
    return di

def sidx(arr, value):
    """ Returns the indices that are smaller or equal to the given array """
    return np.where( arr <= value)[0]

def randcolarr(arr):
    """ Returns a random color for each row in arr """
    return np.random.rand(1,3).repeat(len(arr),axis=0)

def filterfibers(fibarr, filtarr, lower, upper):
    return fibarr[ np.where((filtarr>lower) & (filtarr<upper))[0] ]

def filterfibersidx(filtarr, lower, upper):
    return np.where((filtarr>lower) & (filtarr<upper))[0]

def filterfibersidxcomplement(filtarr, lower, upper):
    return np.where((filtarr<=lower) | (filtarr>=upper))[0]

def filterhemisphere(endpointsmean, xcut, hemi = 0):
    if hemi == 1:
        return np.where( (endpointsmean < xcut) )[0]
    elif hemi == 2:
        return np.where( (endpointsmean > xcut) )[0]

def filterfibersidx2(filtarr, lower, upper, curvarr, value):
    return np.where((filtarr>lower) & (filtarr<upper) & (curvarr >value) )[0]

def filterccidx(endpoints, xcenterline):
    """ Returns the index of all fibers where the start or endpoint
    of each fiber is opposite the x centerline """
    return np.where((endpoints[:,0,0]>xcenterline) & (endpoints[:,1,0]<xcenterline))[0]

def compfilterccidx(endpoints, xcenterline, onlyleft = True):
    """ Returns the index of all fibers where the start or endpoint
    of each fiber is opposite the x centerline """
    if onlyleft:
        return np.where( (((endpoints[:,0,0]>xcenterline) & (endpoints[:,1,0]>xcenterline)) |
                         ((endpoints[:,0,0]<xcenterline) & (endpoints[:,1,0]<xcenterline))) &
                         ((endpoints[:,0,0]<xcenterline) & (endpoints[:,1,0]<xcenterline)) )[0]
    else:
        return np.where( ((endpoints[:,0,0]>xcenterline) & (endpoints[:,1,0]>xcenterline)) |
                         ((endpoints[:,0,0]<xcenterline) & (endpoints[:,1,0]<xcenterline)) )[0]

def showfibfvtk(fibarr, colarr, percentage = 100):
    fibarr2 = fibarr[::percentage]
    colarr2 = colarr[::percentage]
    fibarr2list = fibarr2.tolist()
    r=fvtk.ren();
    #fvtk.add(r,fvtk.axes())
    r.SetBackground(1, 1, 1)
    [fvtk.add(r,fvtk.line(ele, colarr2[i,:])) for i, ele in enumerate(fibarr2list)];
    fvtk.show(r, title = "Fibers", size = (500,500))

def showfibfos(fibarr, colarr, percentage = 100):
    fibarr2 = fibarr[::percentage]
    colarr2 = colarr[::percentage].astype('f4')
    cu = InteractiveCurves(curves = fibarr2.tolist(), colors = colarr2)
    w=World()
    w.add(cu)
    wi = Window(caption="Multi-Modal 1", width = 800, height = 800)
    wi.attach(w)

def preparecolorarray(clustering, number_of_fibers):
    colors=np.zeros((number_of_fibers,4))
    for c in clustering:
        color=np.random.rand(1,4)
        for i in clustering[c]['indices']:
            colors[i]=color
    colors[:,3] = 1.0 # need alpha channel
    return colors

def angle(x,y): return np.rad2deg(np.arccos(np.dot(x,y)/np.sqrt(np.dot(x,x))/np.sqrt(np.dot(y,y))))

def compute_angle_array(tracksobj, downsampling = 3):
    n = tracksobj.shape[0]
    fiberangles = np.zeros( (n, 1) )
    if downsampling == 3:
        for i in xrange(n):
            x=tracksobj[i][0]-tracksobj[i][1]
            y=tracksobj[i][2]-tracksobj[i][1]
            fiberangles[i] = angle(x,y)
    return fiberangles

# start script

# Compute the distance array
dist = compute_start_end_distances(endpoints)

# Inspect histogram to find xcenterline
# hist(endpoints[:,0,0],200)
# -> e.g. found x coordinate 103

# compute distance mean
distmean = np.mean(endpoints[:,:,0],axis=1)
# noccidx = filterfibersidx(distmean, 70, 104) # good to have cc and few corticospinal
noccidx = filterfibersidxcomplement(distmean, 70, 104) # good to have cc and few corticospinal

# change hemi parameter to switch hemisphere
onlyleftidx = filterhemisphere(distmean, 84, hemi = 1)

noccidx = np.intersect1d(noccidx , onlyleftidx)

# 4. filter out corpous callosum fibers
# noccidx = compfilterccidx(endpoints, 84, False)

noccfibers = fibers[noccidx]

# 5. show fibers without cc
#showfibfvtk(noccfibers,randcolarr(noccfibers), 1000)

# 6. look at distance histogram
#hist(dist,100)

# 7. compute new distance histogram for fibers without cc
distnocc = compute_start_end_distances(endpoints[noccidx,:,:])
meancuvnocc = meancurv[noccidx]
#hist(distnocc,100)
#hist(meancuvnocc,100)

# 8. find short fibers and show them
#shortfibers = noccfibers[filterfibersidx(distnocc, 0, 30)]
#showfibfvtk(shortfibers,randcolarr(shortfibers), 100)

# using meancurvature
shortfibers = noccfibers[filterfibersidx2(distnocc, 10, 30, meancuvnocc, 0.05)]
#showfibfvtk(shortfibers,randcolarr(shortfibers), 10)

#shortfibers = noccfibers[filterfibersidx(dist, 8, 40)]
# 120-180: long range
# around 85: some projection, cortico spinal, temporal association
# 8-40: enough U fibers

# 9. fiber clustering
fiblist = shortfibers.tolist()
print("Downsampling...")
tracks=[tm.downsample(t,3) for t in fiblist]

# calculate the angle of downsampled fibers
tracksobj=np.array(tracks, dtype=np.object)

fiberangles = compute_angle_array(tracksobj)

# filter fibers
fiberangleidx = filterfibersidx(fiberangles, 20, 80)

# create new short fiber set
shortfibersnew = shortfibers[fiberangleidx]
tracksobjnew = tracksobj[fiberangleidx]

print("Clustering....")
now=time.clock()
C=td.local_skeleton_clustering(tracksobjnew.tolist(),d_thr=10)
print('Done in %.2f s'  % (time.clock()-now,))

# 10. prepare color array and show
mycols = preparecolorarray(C, len(shortfibersnew))
#showfibfvtk(shortfibersnew,mycols, 10)

#mycols = preparecolorarray(C, len(shortfibers))
#mycols = randcolarr(shortfibers)
#showfibfvtk(shortfibers,mycols, 50)

# only show left hemisphere

# show number of fibers in each bundle histogram
a=np.array([(id,v['N']) for id, v in C.items()])
idx=np.where(a[:,1] < 100)[0]
allidx = np.arange(tracksobjnew.shape[0])
for i in idx:
    idxremove = C[i]['indices']
    allidx = np.lib.arraysetops.setdiff1d(allidx, C[i]['indices'])
    #del C[i]

shortfibersnew2 = shortfibersnew[allidx]
tracksobjnew2 = tracksobjnew[allidx]
C2=td.local_skeleton_clustering(tracksobjnew2.tolist(),d_thr=9)
mycols2 = preparecolorarray(C2, len(shortfibersnew2))

# show with fos
showfibfvtk(shortfibersnew2,mycols2, 10)
#showfibfos(shortfibersnew2,mycols2, 50)
#hist(a,100)

# use fos to show with bigger sized tubes
"""
percentage=1
fibarr2 = shortfibersnew2[::percentage]
colarr2 = mycols2[::percentage].astype('f4')
cu = InteractiveCurves(curves = fibarr2.tolist(), colors = colarr2, line_width = 6.0)
w=World()
w.add(cu)
wi = Window(caption="Multi-Modal 1", width = 1200, height = 900, bgcolor = (1,1,1,0))
wi.attach(w)
# w.delete(cu)
"""
'''

threedviz2 = """
# Modified from NetworkX drawing
# https://networkx.lanl.gov/trac/browser/networkx/examples/drawing/mayavi2_spring.py

import networkx as nx
import numpy as np
from mayavi import mlab

# Retrieve NetworkX graph
G = cfile.obj.get_by_name("connectome_freesurferaparc").data

# Key value on the nodes to transform to scalar value for node coloring
node_scalar_key = "dn_correspondence_id"

# Network Layouting: 2d circular layout
pos=nx.circular_layout(G,dim=2,scale=1)
# numpy array of x,y,z positions in sorted node order
xyz=np.array([pos[v] for v in sorted(G)])
# adding zero z coordinate
xyz = np.hstack( (xyz, np.zeros( (len(xyz), 1) ) ) )

# Network Layouting: 3d spring layout
#pos=nx.spring_layout(G,dim=3)
# numpy array of x,y,z positions in sorted node order
#xyz=np.array([pos[v] for v in sorted(G)])

# If you do not want to apply a layouting algorithm
# You can create the xyz array from your node positions
# as displayed in Code Oracle "3D Network"

# scalar colors
scalars = np.zeros( (len(G.nodes()),) )
for i,data in enumerate(G.nodes(data=True)):
    scalars[i] = float(data[1][node_scalar_key])

mlab.figure(1, bgcolor=(0, 0, 0))
mlab.clf()

pts = mlab.points3d(xyz[:,0], xyz[:,1], xyz[:,2],
                    scalars,
                    scale_factor=0.05,
                    scale_mode='none',
                    colormap='Blues',
                    resolution=20)

# Defines only the connectivity
# You can combine this script with the "3D Network" Code Oracle
pts.mlab_source.dataset.lines = np.array(G.edges())
tube = mlab.pipeline.tube(pts, tube_radius=0.008)
mlab.pipeline.surface(tube, color=(0.8, 0.8, 0.8))

# You can store the resulting figure programmatically
# mlab.savefig('mynetwork.png')
"""

nipypebet = """
# Prerequisite:
# 1. You need to have Nipype installed on your system. You can check this by entering
#    import nipype
#    In the IPython console. If it gives an error, you might want to install it from NeuroDebian.
#    See http://neuro.debian.net/ - Documentation is on http://nipy.sourceforge.net/nipype/
# 2. For this simple brain extraction script, you need to have FSL installed.

# Goal:
# This script shows how to extract the brain using BET through the Nipype interface.
# It is derived from http://nipy.sourceforge.net/nipype/users/dartmouth_workshop_2010.html

# As an input, you need a T1-weighted image that as an input to the Nipype node.
rawimage = cfile.obj.get_by_name('MYRAWT1IMAGE')

# We do not necessarily need to load the connectome object - if the connectome file is extracted
# locally. We just need to retrieve the absolute file path
rawimage_pwd = rawimage.get_abs_path()

# We need the Nipype FSL interface
import nipype.interfaces.fsl as fsl

# We set the FSL default output type to compressed Nifti-1
fsl.FSLCommand.set_default_output_type('NIFTI_GZ')

# We want to store the processed file in the temporary folder for now.
fname_out = '/tmp/only_brain.nii.gz'

# Now, we run the Nipype BET node, providing the correct input
result = fsl.BET(in_file=rawimage_pwd, out_file = fname_out ).run()

# We can print the result
print result.outputs

# To add the processed data file to the currently loaded connectome file, ...
#cvol = cf.CVolume(name="BETed brain", src=fname_out, fileformat='Nifti1GZ', dtype='T1-weighted')
#cfile.obj.add_connectome_volume(cvol)
# Make sure that you save the connectome file if you want to keep the processed file.

"""

pushpull = """
# Prerequisite:
# 1. For this script to run, you need to have PyXNAT installed
#    on your PYTHONPATH system. You can get it from http://packages.python.org/pyxnat/
# 2. You need to have access to an XNAT server and a project
#    You can create a login and project here:
#    http://central.xnat.org/
#    http://sandbox.xnat.org/

# Goal:
# 1. We want to push a connectome file to an XNAT server
# 2. We want to pull a connectome file from an XNAT server

# Hint:
# If you plan to retrieve or upload big datasets, it is recommended to run this
# script in an external Python shell, as long script executions block the IPython
# shell within the Connectome Viewer.

# We assume that a connectome file is currently loaded. For testing purposes,
# it is beneficial if the files are not too big.

# We need to load cfflib
import cfflib as cf

# Retrieve the currently loaded connectome object to push to the XNAT Server
a = cfile.obj

# You need to setup the XNAT connection
cf.set_xnat_connection({'server': 'http://sandbox.xnat.org', 'user':'YOURUSERNAME', 'password':'YOURPASSWORD'})

# You need to have write access on the XNAT Server given. You will need the projectid to push
# data to the server. In addition, you need to provide a subjectid and an experimentid. If overwrite
# is set to True, remote files are overwritten by the local files.

# Then, you can push the connectome file to XNAT
cf.xnat_push( connectome_obj = a, projectid = "YOURPROJECTID", subjectid = "SUBJECTID", experimentid = "EXPID", overwrite = False)

# NB: On the remote server, unique identifier for the subject and experiment id are generated, using the project id.
# The push operation may take some time.

# Similarly as you pushed a connectome file to XNAT, you can pull it again from the server.
# You need the same identifiers to retrieve the data again. In addition, you need to provide a storage
# path for the retrieved files.

#cf.xnat_pull( projectid = "YOURPROJECTID", subjectid = "SUBJECTID", experimentid = "EXPID", '/YOUR/FILE/STORAGE/PATH')

# In case you want to load the pulled connectome object, you can load it using cfflib

#import cfflib as cf; mynewcon = cf.load( '/YOUR/FILE/STORAGE/PATH/meta.cml' )

"""

surfscript = """
# Importing Mayavi mlab interface
from mayavi import mlab


# Retrieving the data
# -------------------

# surface data from connectome file
surface_file_vertices = cfile.obj.get_by_name("%s")
vertices = surface_file_vertices.data.darrays[%s].data
surface_file_faces = cfile.obj.get_by_name("%s")
faces = surface_file_faces.data.darrays[%s].data
labelname = "%s"
surface_file_labels = cfile.obj.get_by_name(labelname)

# Sanity check
# ------------

# ensure that (triangluar) faces have dimension (N,3)
if len(faces.shape) == 1:
    faces = faces.reshape( (len(faces) / 3, 3) )

# check for labels
if labelname == "None":
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
x = x + 128
y = y + 128
z = z - 128
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
# Import Numpy
import numpy as np

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

a = cfile.obj.get_by_name("Final Tractography (freesurferaparc)").get_fibers_as_numpy()
fiberlabels = cfile.obj.get_by_name("Final fiber labels (freesurferaparc)").data

fromid = 8
toid = 10

# Defining some helper functions
# ------------------------------

def sidx(arr, fromval, toval):
    " Returns the indices that are equal to a given value "
    return np.where( (arr[:,0] == fromval) & (arr[:,1] == toval) )[0]

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

idx = sidx(fiberlabels, fromid, toid)
showfibfvtk(a[idx], randcolarr(a[idx]), 100)

"""

netscript = """
# Importing NumPy
import numpy as np
# Importing Mayavi mlab and tvtk packages
from mayavi import mlab
from tvtk.api import tvtk

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
nr_nodes = np.max(g.nodes())
position_array = np.zeros( (nr_nodes, 3) )
for i,nodeid in enumerate(g.nodes()):
    pos = g.node[nodeid][position_key]
    # apply a conversion procedure if the position
    # is a tuple store as string
    # we need a numpy array in the end
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
import cviewer.libs.pyconto.groupstatistics.nbs as nbs

# For documentation of Network-based statistic parameters
# do in IPython: nbs.compute_nbs?
# https://github.com/LTS5/connectomeviewer/blob/master/cviewer/libs/pyconto/groupstatistics/nbs/_nbs.py

# Retrieving the data and set parameters
# --------------------------------------

# Define the two groups of networks you want to compare,
# setting the connectome network name. These objects need
# to exist in the loaded connectome file.
first = ['FirstNetwork_control', 'SecondNetwork_control']
# The same for the second group:
second = ['FirstNetwork_patient', 'SecondNetwork_patient']

# Select the edge value to use for the first group
first_edge_value = 'number_of_fibers'

# Select the edge value to use for the second group
second_edge_value = 'number_of_fibers'

# More parameters for threshold (THRESH)
# andd the number of # permutations (K)
THRESH=3
K=10
# Can be one of 'left', 'equal', 'right'
TAIL='left'

SHOW_MATRIX = True

# Perform task
# ------------

# Get the connectome objects for the given connectome network names
firstgroup = [cfile.obj.get_by_name(n) for n in first]
secondgroup = [cfile.obj.get_by_name(n) for n in second]

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
# and might better be done in a seperate script

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

# Find a date
import datetime as dt
a=dt.datetime.now()
ostr = '%s:%s:%s' % (a.hour, a.minute, a.second)

# You can now add now the results to the connectome file
# Make sure that the name is not existing yet in the connectome file
cfile.obj.add_connectome_network_from_nxgraph('NBS result at %s' % ostr, nbsgraph, dtype='NBSResult')
cfile.update_children()
"""

volrendering = """
from mayavi import mlab
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
from mayavi import mlab
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
image_plane_widget.ipw.slice_index = int(data.shape[0]/2)
"""

reportlab = """
# Credits
# http://www.protocolostomy.com/2008/10/22/generating-reports-with-charts-using-python-reportlab/

from reportlab.platypus import *
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.rl_config import defaultPageSize
from reportlab.lib.units import inch
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import datetime
import tempfile
import os.path

PAGE_HEIGHT=defaultPageSize[1]
styles = getSampleStyleSheet()
Title = "Connectome Report for "
Elements=[]
HeaderStyle = styles["Heading1"]
ParaStyle = styles["Normal"]
PreStyle = styles["Code"]
today = datetime.date.today()
tmpdir = tempfile.gettempdir()

# Retrieving the data
# -------------------

# the network for the reporting
net = cfile.obj.get_by_name('connectome_freesurferaparc')
net.load()
# the edge key
de_key = 'number_of_fibers'
# output file name
fname = os.path.join(tmpdir, 'out.pdf')

date = today.strftime('Reported on %dth, %h %Y')
netw = net.data

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

def go(fname):
    doc = SimpleDocTemplate(fname)
    doc.build(Elements)

mytitle = header(Title + '"' + cfile.obj.get_connectome_meta().get_title() + '"')
mydate = header(date, sep=0.1, style=ParaStyle)

mytitlenet = header(net.get_name() + " (CNetwork)")

for u,v,d in netw.edges_iter(data=True):
    edgval = netw.edge[u][v][de_key]
    if edgval > 0:
        netw.edge[u][v]['weight'] = 1
    else:
        netw.edge[u][v]['weight'] = 1

b=nx.to_numpy_matrix(netw)
fig = plt.figure()
fig.suptitle("Binary Connection matrix")
aa= plt.imshow(b, interpolation='nearest', cmap=plt.cm.Greys, vmin = b.min(), vmax=b.max())
fig.savefig(os.path.join(tmpdir, 'matrix.png'))

fig.clear()
fig.suptitle("Degree distribution")
plt.hist(netw.degree().values(),30)
fig.savefig(os.path.join(tmpdir,'distri.png'))

# measures
if nx.is_connected(netw):
    isit = "Yes"
else:
    isit = "No"
me1 = p("Number of Nodes: " + str(netw.number_of_nodes()))
me2 = p("Number of Edges: " +  str(netw.number_of_edges()))
me3 = p("Is network connected: " + isit)
me4 = p("Number of connected components: " + str(nx.number_connected_components(netw)))
me44 = p("Average node degree: %.2f" % np.mean(netw.degree().values()))
me5 = p("Average unweighted shortest path length: %.2f" % nx.average_shortest_path_length(netw, weighted = False))
me6 = p("Average clustering coefficient: %.2f" % nx.average_clustering(netw))

logo = os.path.join(tmpdir, "matrix.png")
im1 = Image(logo, 300,225)
logo = os.path.join(tmpdir, "distri.png")
im2 = Image(logo, 250,188)

codesection = [mytitle, mydate, mytitlenet, me1, me2, me3, me4,me44, me5, me6, im1, im2]
src = KeepTogether(codesection)
Elements.append(src)
go(fname)
"""
