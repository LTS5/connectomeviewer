from nipype.interfaces.base import (BaseInterface, BaseInterfaceInputSpec, traits,
                                    File, TraitedSpec, InputMultiPath,
                                    OutputMultiPath, isdefined)
from nipype.utils.filemanip import split_filename
import os, os.path as op
import networkx as nx
import numpy as np
import pickle
from enthought.mayavi import mlab
from enthought.tvtk.api import tvtk
import logging
from nipype.utils.misc import package_check
import warnings

logging.basicConfig()
iflogger = logging.getLogger('interface')

try:
    package_check('enthought')

except Exception, e:
    warnings.warn('Enthought/Mayavi/TVTK not installed')
try:
    package_check('cviewer')
except Exception, e:
    warnings.warn('ConnectomeViewer not installed')

else:
    from dipy.tracking.utils import density_map


def get_positions_and_vectors(ntwk, position_key='dn_position', edge_key='weight'):
    ntwk_position_array = get_positions(ntwk, position_key)
    vectors, start_positions, end_positions, ev = get_vectors(ntwk, ntwk_position_array, edge_key)
    return ntwk_position_array, vectors, start_positions, end_positions, ev
    
    
def get_positions(ntwk, position_key="dn_position"):
    ntwk = nx.read_gpickle(ntwk)
    num_ntwk_nodes = ntwk.number_of_nodes()
    ntwk_position_array = np.zeros( (num_ntwk_nodes, 3) )
    for i, nodeid in enumerate(ntwk.nodes()):
        ntwk_pos = ntwk.node[nodeid][position_key]
        ntwk_pos = np.array(ntwk_pos)
        ntwk_position_array[i,:] = ntwk_pos
    return ntwk_position_array


def get_vectors(ntwk, ntwk_position_array, edge_key='weight'):
    ntwk = nx.read_gpickle(ntwk)
    edges = np.array(ntwk.edges())
    ev = np.zeros( (ntwk.number_of_edges(), 1) )
    for i,d in enumerate(ntwk.edges_iter(data=True)):
        ev[i] = d[2][edge_key]
        # ensure that we are setting the correct edge
        assert d[0] == edges[i,0] and d[1] == edges[i,1]
    edges = edges - 1 # Need to subtract one because the array index starts at zero
    start_positions = ntwk_position_array[edges[:, 0], :].T
    end_positions = ntwk_position_array[edges[:, 1], :].T
    vectors = end_positions - start_positions
    return vectors, start_positions, end_positions, ev


def plot_text_and_scalarbar(phrase):    
    """
    Adds text and scale bar
    """
    x = 0.02
    y = 0.02
    width = 0.3
    text = ntwk_name
    mlab.text(x,y,phrase,width=width)
    #mlab.scalarbar(myvectors,'Mean Fiber Length (mm)', 'vertical')
    #mlab.scalarbar(ntwk_nodes, ntwk_name, 'vertical')


def plot_nodes(ntwk, position_key='dn_position', scalar_key='value'):
    ntwk_position_array = get_positions(ntwk, position_key)
    ntwk = nx.read_gpickle(ntwk)
    node_scalar_key = 'value'
    scalars = np.zeros( (len(ntwk.nodes()),) )
    for i,data in enumerate(ntwk.nodes(data=True)):
        if data[1].has_key(scalar_key):
            scalars[i] = float(data[1][scalar_key])
    x, y, z = ntwk_position_array[:,0], ntwk_position_array[:,1], ntwk_position_array[:,2]
    nodesource = mlab.pipeline.scalar_scatter(x, y, z, scalars, name = scalar_key + ' Node Source')
    ntwk_nodes = mlab.pipeline.glyph(nodesource, scale_factor=1.5, scale_mode='none', name = scalar_key + ' Nodes', mode='sphere')
    ntwk_nodes.glyph.color_mode = 'color_by_scalar'


def plot_labels_by_phrase(ntwk, phrase, position_key='dn_position', node_label_key='dn_name'):
    ntwk_position_array = get_positions(ntwk, position_key)
    ntwk = nx.read_gpickle(ntwk)
    nodes = ntwk.nodes_iter()
    for node in nodes:
        node_name = str(ntwk.node[node][node_label_key])
        row_index = node - 1
        if node_name.rfind(phrase) >= 0:
            label = ntwk.node[node][node_label_key]
            mlab.text3d(ntwk_position_array[row_index,0],
                        ntwk_position_array[row_index,1],
                        ntwk_position_array[row_index,2],
                        '     ' + label,
                        name = 'Node ' + label)


def plot_labels_by_degree(ntwk, degree, position_key='dn_position', node_label_key='dn_name'):
    ntwk_position_array = get_positions(ntwk, position_key)
    ntwk = nx.read_gpickle(ntwk)
    nodes = ntwk.nodes_iter()
    for node in nodes:
        node_degree = ntwk.degree(node)
        row_index = node - 1
        if node_degree >= degree:
            label = ntwk.node[node][node_label_key]
            mlab.text3d(ntwk_position_array[row_index,0],
                        ntwk_position_array[row_index,1],
                        ntwk_position_array[row_index,2],
                        '     ' + label,
                        name = 'Node ' + label)
           
                        
def plot_edges(ntwk, position_key='dn_position', edge_key='weight'):
    ntwk_position_array, vectors, start_positions, end_positions, ev = get_positions_and_vectors(ntwk, position_key, edge_key)
    ntwk = nx.read_gpickle(ntwk)
    vectorsrc = mlab.pipeline.vector_scatter(start_positions[0], 
                                 start_positions[1],
                                 start_positions[2],
                                 vectors[0],
                                 vectors[1],
                                 vectors[2],
                                 name = edge_key)
    da = tvtk.DoubleArray(name=edge_key)
    da.from_array(ev)
    vectorsrc.mlab_source.dataset.point_data.add_array(da)
    vectorsrc.mlab_source.dataset.point_data.scalars = da.to_array()
    vectorsrc.mlab_source.dataset.point_data.scalars.name = edge_key

    vectorsrc.outputs[0].update() # need to update the boundaries
    # Add a thresholding filter to threshold the edges
    thres = mlab.pipeline.threshold(vectorsrc, name="Thresholding")

    myvectors = mlab.pipeline.vectors(thres,colormap='hot',
                                                #mode='cylinder',
                                                name=edge_key,
                                                #scale_factor=1,
                                                #resolution=20,
                                                # make the opacity of the actor depend on the scalar.
                                                transparent=True,
                                                scale_mode = 'vector')
    myvectors.glyph.glyph_source.glyph_source.glyph_type = 'dash'
    # vectors.glyph.glyph_source.glyph_source.radius = 0.01
    myvectors.glyph.color_mode = 'color_by_scalar'
    myvectors.glyph.glyph.clamping = False


def plot_surfaces(surface_file, label_file="None"):
    import nibabel.gifti as gifti
    surface_file = gifti.read(surface_file)
    vertices = surface_file.darrays[0].data
    faces = surface_file.darrays[1].data
    if label_file == "None":
        labels = None
    else:
        label_file = gifti.read(label_file)
        labels = label_file.darrays[0].data
        labels = labels.ravel()
        assert vertices.shape[0] == len(labels)

    if len(faces.shape) == 1:
        faces = faces.reshape( (len(faces) / 3, 3) )

    x, y, z = vertices[:,0], vertices[:,1], vertices[:,2]
    x = x + 128
    y = y + 128
    z = z - 128
    mlab.triangular_mesh(x, y, z, faces, scalars = labels)


def plot_volumes(image_file):
    import nibabel as nb
    image = nb.load(image_file)
    image_data = image.get_data()
    affine = image.get_affine()
    center = np.r_[0, 0, 0, 1]
    data_src = mlab.pipeline.scalar_field(image_data)
    data_src.spacing = np.diag(affine)[:3]
    data_src.origin = np.dot(affine, center)[:3]
    mlab.pipeline.outline(data_src)
    image_plane_widget = mlab.pipeline.image_plane_widget(data_src, name=image_file)
    image_plane_widget.ipw.plane_orientation = 'x_axes'
    image_plane_widget.ipw.reslice_interpolate = 'nearest_neighbour'
    image_plane_widget.ipw.slice_index = int(image_data.shape[0]/2)

def create_rotation_frames(out_folder, out_name, frames=4, ext='.png'):
	for frame in range(1,frames+1):
		f = mlab.gcf()
		f.scene.camera.azimuth(1)
		name = '%04d' %frame
		out_frame = op.join(out_folder, out_name) + name + ext
		f.scene.render()
		mlab.savefig(out_frame)

class RotationMovieInputSpec(BaseInterfaceInputSpec):
    in_file = File(exists=True, mandatory=True, desc='Networks for node removal subjects')
    number_of_frames = traits.Int(360, usedefault=True, desc='Number of frames to generate')
    x_degrees = traits.Int(360, usedefault=True, desc='Degrees to rotate around the x axis')
    y_degrees = traits.Int(0, usedefault=True, desc='Degrees to rotate around the y axis')
    z_degrees = traits.Int(0, usedefault=True, desc='Degrees to rotate around the z axis')
    output_as_mpeg = traits.Bool(False, usedefault=True, desc='Option to save the output networks in an mpeg movie file')
    output_mpeg_file = File('rotation.mpg', usedefault=True, desc='The output images saved as an mpeg movie file')

class RotationMovieOutputSpec(TraitedSpec):
    mpeg_file = File(desc='The output networks saved in a single mpeg file')
    out_files = OutputMultiPath(File(desc='Output sequence of images'))

class RotationMovie(BaseInterface):
    """
    Creates a movie by plotting single frames and combining them using ffmpeg

    Example
    -------

    >>> import nipype.interfaces.connectomeviewer as cv
    >>> rm = cv.RotationMovie()
    >>> rm.inputs.in_files = ['subj1.pck', 'subj2.pck'] # doctest: +SKIP
    >>> rm.run()                 # doctest: +SKIP
    """
    input_spec = RotationMovieInputSpec
    output_spec = RotationMovieOutputSpec

    def _run_interface(self, runtime):
        global out_paths
        out_paths = []
        create_rotation_frames(out_directory, 'const1', 360)
        for in_file in self.inputs.in_files:
            if idx == 0:
                out_path = get_out_paths(in_file)
                remove_nodes_named(in_file, phrase, out_path)
            else:
                remove_nodes_named(out_path, phrase, out_path)
            out_paths.append(out_path)
                        
        if self.inputs.output_as_cff:
            out_paths = get_out_paths(self.inputs.in_files)
            convert.inputs.gpickled_networks = out_paths
            iflogger.info(out_paths)
            convert.inputs.out_file = op.abspath(self.inputs.output_cff_file)
            convert.run()
            iflogger.info('Saving output CFF file as {out}'.format(out=op.abspath(self.inputs.output_cff_file)))
        
        isolate_list = []
        for idx, out_file in enumerate(out_paths):
            graph = nx.read_gpickle(out_file)
            n_isolates = len(nx.isolates(graph))
            iflogger.info('File: {f} has {n} unconnected nodes'.format(f=out_file, n=n_isolates))
            isolate_list.append(n_isolates)
        iflogger.info(isolate_list)
        return runtime

    def _list_outputs(self):
        outputs = self.output_spec().get()
        out_paths = get_out_paths(self.inputs.in_files)
        outputs['out_files'] = out_paths
        if self.inputs.output_as_cff:
            outputs['mpeg_file'] = op.abspath(self.inputs.output_cff_file)
        return outputs        
