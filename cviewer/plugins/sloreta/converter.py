""" Converter of functional EEG sLORETA data to Connectome File Format
    for Visualization and Analysis in the ConnectomeViewer
"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# TODO
# - metadata for generated files: key, values as editor (maybe tstat thresholds as meta)
# - not every time the number of ROIs is equal as the number of electrodes

import datetime

# Enthought library imports
from enthought.traits.api import HasTraits, Int, Str, Password, Directory, List,\
				 Bool, File, Button, Instance, Enum, DelegatesTo

from enthought.traits.ui.api import View, Item, ListStrEditor, HGroup, Handler, \
				    message, spring, Group, EnumEditor

class Metainfo(HasTraits):
    meta_init = Str
    meta_cdt = Str ( str(datetime.date.today()) )
    meta_moddt = Str( str(datetime.date.today()) )
    meta_name = Str
    meta_species = Str('Homo sapiens')
    meta_legal = Str('Creative Commons Attribution 3.0 Unported')
    meta_ref = Str
    meta_url = Str
    meta_desc = Str
    metaxml = Str

    view = View(
	Group(
	    Item('meta_init', label='Initial Creator'),
	    Item('meta_cdt', label='Creation date'),
	    Item('meta_moddt', label='Modification date'),
	    Item('meta_name', label='(Study) Name'),
	    Item('meta_species', label='Species'),
	    Item('meta_legal', label='Legal notice'),
	    Item('meta_ref', label='Reference'),
	    Item('meta_url', label='URL'),
	    Item('meta_desc', label='Description', style = 'custom'),
	    label = 'Metadata',
	    show_border = True
	),
	title     = 'Enter Metadata',
	width     = 0.5,
	height    = 0.5,
	resizable = True,
	buttons = [ 'OK', ]
    )

    def get_metaxml(self):
	return """
<generator>sLoreta Converter</generator>
<initial-creator>%s</initial-creator>
<creation-date>%s</creation-date>
<modification-date>%s</modification-date>
<name>%s</name>
<species>%s</species>
<legal-notice>%s</legal-notice>
<references>%s</references>
<url>%s</url>
<description>%s</description>""" % (self.meta_init, self.meta_cdt, self.meta_moddt, \
				      self.meta_name, self.meta_species, self.meta_legal,\
				      self.meta_ref, self.meta_url, self.meta_desc)


class ConverterParameters(HasTraits):
    """ All necessary parameters to converter sLORETA to Connectome File Format """

    # classical frequency bands
    freq_bands_type1 = ['delta', 'theta', 'alpha1', 'alpha2', 'beta1', 'beta2', 'beta3', 'gamma', 'fullband']
    
    # reading statistics files
    convert = Enum ( 'Subjects', ['Subjects', 'Statistics: CorrCoeff', 'Statistics: t-Statistics'] )
    # variables to set
    rootfolder = Directory()
    subjects = List( Str )
    conditions = List( Str )
    measures = List( Str )
    freq_bands = List( Str , freq_bands_type1)
    
    stats_name = List( Str )
    stats_extvars = List( Str )
    stats_measure = List( Str, ['CorrCoeff'] )
    
    metainfo = Instance( Metainfo )
    nr_rois = Int ()
    electrode_coordinates_file = File ()
    ROI_slor_file = File ()
    write_electrode_network = Bool( True )
    
    create_cff = Button
    validate_form = Button
    update_meta = Button
    save_pickle = Button
    pickle_file = File
    load_pickle = Button
    
    def __init__(self, **traits):
        super(ConverterParameters, self).__init__(**traits)
	
	self.metainfo = Metainfo()

    view = View(
	Item( 'update_meta', label = 'Update Metadata'),
	Item( 'rootfolder', label = 'Root Folder', help = 'You must copy all the files into this folder'),
	Item( 'nr_rois', label = '# of ROIs:', help = 'The number of ROIs = number of rows/columns of each matrix'),
	Item( 'electrode_coordinates_file', label = 'Electrode coordinates file:', help = 'The .sxyz file containing electrode names and coordinates'),
	Item( 'write_electrode_network', label= 'Create empty electrode network?', help = 'Indicates if an empty network with only the electrodes as nodes should be included in the CFF file'),
	Item( 'ROI_slor_file', label = 'ROI slor file:', help = 'slor file for Regions of Interest - leave empty for not-converting'),
	Item( 'convert', label = 'Convert?'),
	HGroup(
	    Item( 'subjects',
		  show_label = False,
		  editor = ListStrEditor( title = 'Subjects List', auto_add = True )
	    ),
	    Item( 'conditions',
		  show_label = False,
		  editor = ListStrEditor( title = 'Conditions List', auto_add = True )
	    ),
	    Item( 'measures',
		  show_label = False,
		  editor = ListStrEditor( title = 'Measure List', auto_add = True )
	    ),
	    Item( 'freq_bands',
		  show_label = False,
		  editor = ListStrEditor( title = 'Frequency Bands', auto_add = True )
	    ),
	    visible_when = 'convert == "Subjects"'
	),
	HGroup(
	    Item( 'stats_name',
		  show_label = False,
		  editor = ListStrEditor( title = 'Name', auto_add = True )
	    ),
	    Item( 'stats_extvars',
		  show_label = False,
		  editor = ListStrEditor( title = '# of external variables', auto_add = True ),
		   help = 'Number of corresponding external variables'
	    ),
	    Item( 'stats_measure',
		  show_label = False,
		  editor = ListStrEditor( title = 'Measure', auto_add = True )
	    ),
	    Item( 'freq_bands',
		  show_label = False,
		  editor = ListStrEditor( title = 'Frequency Bands', auto_add = True )
	    ),
	    visible_when = 'convert == "Statistics: CorrCoeff"'
	),
	HGroup(
	    Item( 'stats_name',
		  show_label = False,
		  editor = ListStrEditor( title = 'Name', auto_add = True )
	    ),
	    Item( 'stats_measure',
		  show_label = False,
		  editor = ListStrEditor( title = 'Measure', auto_add = True )
	    ),
	    Item( 'freq_bands',
		  show_label = False,
		  editor = ListStrEditor( title = 'Frequency Bands', auto_add = True )
	    ),
	    visible_when = 'convert == "Statistics: t-Statistics"'
	),
	HGroup( 
	    #Item( 'validate_form', label = 'Validate Form', show_label = False),
	    Item( 'load_pickle', label = 'Load State', show_label = False),
	    Item( 'save_pickle', label = 'Save State', show_label = False),
	    spring,
	    Item( 'create_cff', label = 'Create CFF ...', show_label = False),
	),
        title     = 'sLORETA Converter',
        width     = 0.5,
        height    = 0.5,
        resizable = True,
	buttons = [ 'OK', ]
	
    )

    def _update_meta_changed(self, value):
	self.metainfo.configure_traits()

    def _load_pickle_changed(self, value):
	import pickle
	import enthought.sweet_pickle as sp
	import os.path
	from enthought.pyface.api import FileDialog, OK
	
	wildcard = "sLORETA Converter State (*.pkl)|*.pkl|" \
                    "All files (*.*)|*.*"
        dlg = FileDialog(wildcard=wildcard,title="Choose a File to load state",\
                         resizeable=False, \
                         default_directory=self.rootfolder,)
        
        if dlg.open() == OK:            
            if not os.path.isfile(dlg.path):
                return
	    else:
		output = open(dlg.path, 'rb')
		data = sp.load(output)
		self.__setstate__(data.__getstate__())
		output.close()

    def _save_pickle_changed(self, value):
	import pickle
	import enthought.sweet_pickle as sp
	import os.path
	from enthought.pyface.api import FileDialog, OK
	
	wildcard = "sLORETA Converter State (*.pkl)|*.pkl|" \
                    "All files (*.*)|*.*"
        dlg = FileDialog(wildcard=wildcard,title="Choose a File to store state",\
                         resizeable=False, \
                         default_directory=self.rootfolder,)
        
        if dlg.open() == OK:
            
	    output = open(dlg.path, 'wb')
	    # Pickle the list using the highest protocol available.
	    sp.dump(self, output, -1)
	    output.close()


    def _create_cff_changed(self, value):
	import os
	from os.path import join, exists, split
	
	# Generate Connectome File
	# -----
	if self.convert == 'Subjects':
	    suffix = '_subjects'
	elif self.convert == 'Statistics: CorrCoeff':
	    suffix = '_statistics'
	elif self.convert == 'Statistics: t-Statistics':
	    suffix = '_tstatistics'
	
	def removeDir(path):
	    import shutil
	    import os.path
	    if os.path.isdir(path):
	        shutil.rmtree(path, ignore_errors = True)
	
	# create folder CFF
	if not exists(join(self.rootfolder, 'CFF'+ suffix)):
	    os.mkdir(join(self.rootfolder, 'CFF'+ suffix))
	else:
	    # remove folder if it exists
	    removeDir(join(self.rootfolder, 'CFF'+ suffix))
	    os.mkdir(join(self.rootfolder, 'CFF'+ suffix))
	# create subfolder Networks
	if not exists(join(self.rootfolder, 'CFF'+ suffix, 'Network')):
	    os.mkdir(join(self.rootfolder, 'CFF'+ suffix, 'Network'))
	# create subfolder Volumes
	if not exists(join(self.rootfolder, 'CFF'+ suffix, 'Nifti')):
	    os.mkdir(join(self.rootfolder, 'CFF'+ suffix, 'Nifti'))
	
	# create electrode position network
	if self.write_electrode_network:
	    outcontent, nodes = electrode_pos_to_graphml(filename = self.electrode_coordinates_file )
	    fi = open(join(self.rootfolder, 'CFF'+ suffix, 'Network', 'electrodes.graphml'), 'w')
	    fi.write(outcontent)
	    fi.close()
	
	# create ROI segmentation volume nifti file
	if not self.ROI_slor_file == '':
	    nim = slor2nifti(filename = self.ROI_slor_file, merge_roi = True)
	    filename = split(self.ROI_slor_file)[1].split('.')[0]
	    nim.to_filename(join(self.rootfolder, 'CFF'+ suffix, 'Nifti', filename + '.nii.gz'))

	
	# create meta.xml
	fim = open(join(self.rootfolder, 'CFF' + suffix, 'meta.xml'), 'w')
	content = """<?xml version="1.0" encoding="UTF-8"?>
<viewer xmlns="http://www.connectome.ch/2009/Connectome/xmlns"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.connectome.ch/2009/Connectome/xmlns
					    connectome.xsd">

<viewer-meta version="1.0">"""
	content = content + self.metainfo.get_metaxml() + """
    <nr_of_networks>%s</nr_of_networks>
</viewer-meta>\n""" % str(len(self.subjects) + 1)
    
	# add networks
	# electrode
	if self.write_electrode_network:
	    content = content + """<viewer-network name="Electrodes" src="Network/electrodes.graphml" hierarchical="0" hypergraph="0" directed="0" />\n"""

	# -----
	if self.convert == 'Statistics: CorrCoeff':
	    
	    for mes in self.stats_measure:
		#print 'Calculating measure: ', mes
		
		for i, name in enumerate(self.stats_name):
		    #print 'For name: ', name
		    
		    # number of external variables has to be an integer
		    val = int(self.stats_extvars[i])
		    
		    # loop through all the independent variables and create graphml		    
		    for extvar_i in range(1, val + 1):
			
			#print 'Independent Variable: ', extvar_i
			# correct zero-padding
			if val > 0:
			    extvar_name = '%02d' % extvar_i
			elif val > 99:
			    extvar_name = '%03d' % extvar_i
			
			fi = open(join(self.rootfolder, 'CFF'+ suffix, 'Network', 'statistic_'+name+'_indvar'+extvar_name+'.graphml'), 'wb')
			content2 = statistics2graphml(rootfolder = self.rootfolder, nr_rois = self.nr_rois, \
						     name = name, measure = mes, \
						     extvar = extvar_name, freq_bands = self.freq_bands, \
						     el_cord_file = self.electrode_coordinates_file)
		    
			fi.write(content2)
			fi.close()
	
		        # output network in metaxml
			content = content + """<viewer-network name="%s-%s-IndVar%s" src="Network/statistic_%s_indvar%s.graphml" hierarchical="0" hypergraph="0" directed="0">""" \
				  % (mes, name, extvar_name, name, extvar_name)
			if not self.ROI_slor_file == '':
			    content = content + """
		  <network-volume name="%s" src="Nifti/%s.nii.gz" fileformat="nifti" segmentation="true">
		    <description>The segmentation of the brain in Regions of Interest.</description>
		  </network-volume>\n""" % (filename, filename)
			content = content + """</viewer-network>\n"""
			
	elif self.convert == 'Statistics: t-Statistics':
	    
	    for mes in self.stats_measure:
		#print 'Calculating measure: ', mes
		
		for i, name in enumerate(self.stats_name):
		    #print 'For name: ', name
		    
		    fi = open(join(self.rootfolder, 'CFF'+ suffix, 'Network', 'tstatistic_'+name+'_'+mes+'.graphml'), 'wb')
		    content3 = tstatistics2graphml(rootfolder = self.rootfolder, nr_rois = self.nr_rois, \
						 name = name, measure = mes, \
						 freq_bands = self.freq_bands, \
						 el_cord_file = self.electrode_coordinates_file)
		
		    fi.write(content3)
		    fi.close()
    
		    # output network in metaxml
		    content = content + """<viewer-network name="%s-%s" src="Network/tstatistic_%s_%s.graphml" hierarchical="0" hypergraph="0" directed="0">""" \
			      % (name, mes, name, mes)
		    if not self.ROI_slor_file == '':
			content = content + """
	      <network-volume name="%s" src="Nifti/%s.nii.gz" fileformat="nifti" segmentation="true">
		<description>The segmentation of the brain in Regions of Interest.</description>
	      </network-volume>\n""" % (filename, filename)
		    content = content + """</viewer-network>\n"""
		
	elif self.convert == 'Subjects':
	    
	    # loop through all the subjects and create graphml
	    for subj in self.subjects:
		fi = open(join(self.rootfolder, 'CFF'+ suffix, 'Network', 'subject_'+subj+'.graphml'), 'wb')
		content2 = coherences2graphml(rootfolder = self.rootfolder, nr_rois = self.nr_rois, \
					     subject = subj, conditions = self.conditions, \
					     measures = self.measures, freq_bands = self.freq_bands, \
					     el_cord_file = self.electrode_coordinates_file)
		fi.write(content2)
		fi.close()
	
	    # subjects
	    for subj in self.subjects:
		content = content + """<viewer-network name="Subject %s" src="Network/subject_%s.graphml" hierarchical="0" hypergraph="0" directed="0">""" \
			  % (subj, subj)
		content = content + """
	  <network-volume name="%s" src="Nifti/%s.nii.gz" fileformat="nifti" segmentation="true">
	    <description>The segmentation of the brain in Regions of Interest.</description>
	  </network-volume>\n""" % (filename, filename)
		content = content + """</viewer-network>\n"""
	
	
	# ending
	content = content + "</viewer>"
	fim.write(content)
	fim.close()
	
	# create .cff with Zip
	import zipfile as zip
	filen = self.metainfo.meta_name.replace(' ', '_')
	filen = filen.lower()
	mycff = zip.ZipFile(join(self.rootfolder, filen + suffix + '.cff'), mode='w')
	
	os.chdir(join(self.rootfolder, 'CFF' + suffix))
	for root, dirs, files in os.walk('.'):
	    for name in files:       
		filename = os.path.join(root, name)
		# print 'Writing to Connectome File: ', filename
		mycff.write(filename)
	mycff.close()
    
	message(message = 'Connectome File generated in Root Folder!', title = 'Done.')


#####

# loreta voxel order matrix: 6239x3
def read_loretavoxelinf(filename = 'MNI-BAs-6239-voxels.csv'):
    """ Reads the voxel information of sLoreta """
    
    import csv
    fn = ['X', 'Y', 'Z', 'PrimAnat', 'SecAnat', 'BA']
    reader = csv.DictReader(open(filename, "r"), delimiter=',', fieldnames = fn)
    info = []
    for row in reader:
        info.append(row)
    return info
    
def electrode_pos_to_graphml(filename):
    """ Converts the electrode position file to a network without edges """
    
    f = open(filename, 'r')
    nodes = ''
    for i, line in enumerate(f):
        
        if i == 0:
            nr_of_electrodes = line
        else:
            lsp = line.split(' ')
            x, y, z, name = float(lsp[0]), float(lsp[1]), float(lsp[2]), lsp[3].rstrip('\r\n')
            nodes = nodes + """<node id="n%s">
    <data key="dn_label">%s</data>
    <data key="dn_position">%s,%s,%s</data>
</node>
""" % (i, name, x, y, z)
            
    outfile = startstring % nr_of_electrodes
    outfile = outfile + nodes
    outfile = outfile + endstring
    f.close()
    
    return outfile, nodes
    
startstring = """<?xml version="1.0" encoding="UTF-8"?>

<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:xlink="http://www.w3.org/1999/xlink"

	xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns 
	http://www.connectome.ch/graphml+xlink+gifti.xsd">

<key id="dn_label" for="node" attr.name="label" attr.type="string">
	<desc>The label of the electrode</desc>
</key>
<key id="dn_position" for="node" attr.name="position" attr.type="string">
	<desc>The position of the electrode</desc>
</key>
<graph id="%s electrodes" edgedefault="undirected">
"""
endstring = """</graph>
</graphml>
"""

def slor2nifti(filename, merge_roi = False):
    """ open .slor files and convert them to Nifti """

    from numpy import array, zeros, dot, float32, reshape
    
    import array as arr_reader
    from os.path import join
    
    # open binary file and extract the data of all images
    f = open(filename, 'rb')
    data_arr = array(arr_reader.array('f', f.read()))
    nr_images = len(data_arr)/6239
    da = reshape(data_arr, (nr_images, 6239))
    #print 'Number of images in file %s: %s' % (filename, nr_images)

    # MNI-space: 181 x 217 x 181
    # create the 3d array
    # x = [-70,70] dim = 141
    # y = [-100,65] dim = 166
    # z = [-45, 70] dim = 116
    # order of the array is z,y,x
    # arr = zeros((116, 166, 141))
    arr = zeros((1, 181, 217,181), dtype=float32)
    # transformation matrix: XYZ1 in MNI (mm) to voxel indices
    # from the Colin_1mm brain
    trafo = array([[1,0,0, 89],
                      [0,1,0, 125],
                      [0,0,1, 71],
                      [0,0,0, 1]])
    
    # read the loreta mapping for the voxel
    from cviewer.action.common import RESOURCE_PATH
    lor_list = read_loretavoxelinf(join(RESOURCE_PATH, 'MNI-BAs-6239-voxels.csv'))
    
    if merge_roi:
        # loop over 1..n-1 images (which are e.g. ROIs), giving each an ID
        for image_idx in range(nr_images-1):
	    #print 'ROI Nr: ', str(image_idx + 1)
            for i, vox in enumerate(lor_list):
                # i should go from 0 to 6238
		p = da[image_idx, i]
                if p != 0.0:
                    # write image_idx as a segementation label according to the ROI
                    # to the beautiful arr which will become the nifti volume
                    x,y,z = int(vox['X']), int(vox['Y']), int(vox['Z'])
		    
		    # transformation of mni RAS to voxel
		    val = array([x, y, z, 1])
		    # apply the transformation
		    x,y,z, t =  dot(trafo, val)
		    if 'ROI' in filename:
			
			vox_value = image_idx + 1
		    else:
			vox_value = p
		    arr[0, z-2:z+3, y-2:y+3, x-2:x+3] = vox_value

    # save arr as nifti
    #nim = nifti.NiftiImage(arr)
    from cviewer.io.nipy.imageformats.nifti1 import Nifti1Header, Nifti1Image
    #hdr = Nifti1Header()
    #hdr.set_sform(trafo)
    #hdr.set_data_dtype(float32)
    #hdr.set_sform(trafo)
    #hdr.set_data_shape(arr.shape)
    
    nim = Nifti1Image(arr, trafo)
    
    # hdr.set_sform(trafo, code='mni152')
    #nim.setPixDims([1.0, 1.0, 1.0])
    #nim.setVoxDims([1.0, 1.0, 1.0])
    #nim.setXYZUnit('mm')
    
    # close file and return nifti image
    f.close()
    return nim


def tstatistics2graphml(rootfolder, nr_rois, name = None, \
                       measure = None, freq_bands = None, el_cord_file = None):
    """ Converts sLORETA t-statistic files into GraphML
	Naming convention: name-measure.txt
	E.g. A-B-EEGLaggedConn.txt
        
    """
    
    from os.path import exists, isfile, join
    from numpy import zeros, array
    
    if not exists(rootfolder):
        raise Exception('Path %s does not exist.' % rootfolder)
        return

    outfile = startstring_coh
    
    # write edge keys
    for freq in freq_bands:
	key = '<key id="%s" for="edge" attr.name="%s" attr.type="double"><default>0</default></key>\n'
	outfile = outfile + key % (freq, freq)
    
    # start graph
    outfile = outfile + '<graph id="%s" edgedefault="undirected">\n' % name
    
    # write nodes (ROIs)
    # use the electrode positions preliminary
    out, nodes = electrode_pos_to_graphml(filename = join(rootfolder, el_cord_file))
    outfile = outfile + nodes
    
    # write edges

    file = join(rootfolder, name+'-'+measure+'.txt')
    if isfile(file):
	print 'Converting file %s ...' % str(file)
	nr_elect, nr_mat = extract_metadata_stats(filename = file)

	# number of electrodes (ROIs) from the file header must be equal as the one given in the form
	assert nr_elect == nr_rois
	# number of matrices must equal number of frequency bands given
	assert nr_mat == len(freq_bands)
	
	f = open(file, 'r')
	# skip header
	a = f.readline()
	a = f.readline()
	while ' = ' in a  or a == '':
	    a = f.readline()

	# create numpy array for all frequency bands
	data_arr = zeros((nr_elect, nr_elect, nr_mat))

	for mat_i in range(nr_mat):
	    for elect_i in range(nr_elect):
		line = f.readline()
		lsp = line.split(' ')
		
		while lsp.count('') != 0:
		    lsp.remove('')

#		print 'lenlsp',len(lsp)
		#print nr_elect
		assert len(lsp) == nr_elect

		data_arr[elect_i, :, mat_i] = array(lsp)
	    # skip empty line
	    f.readline()
	#print 'Done.'
    else:
	#print 'No file %s existing.' % file
	return
    
                
    # now loop over bigarray and write the the edges
    for i in range(0, nr_rois):
        for j in range(i, nr_rois):
            if i == j:
                # skipping self-loops
                continue
            
            outfile = outfile + '<edge id="e%s_%s" source="n%s" target="n%s">\n' \
                      % (str(i+1), str(j+1), str(i+1), str(j+1))
            
	    for freq_i, frequency_label in enumerate(freq_bands):                            
		# loop over all frequency bands
		
		label, value = frequency_label, data_arr[i,j,freq_i]
		if value == 0.0:
		    continue
		# convert value (float) to truncated string
		value = value
		outfile = outfile + '    <data key="%s">%s</data>\n'\
				    % (label, str(value))
            
	    outfile = outfile + '</edge>\n'

    outfile = outfile + endstring
    return outfile

def statistics2graphml(rootfolder, nr_rois, name = None, extvar = None, \
                       measure = None, freq_bands = None, el_cord_file = None):
    """ Converts sLORETA statistic files (correlations) into GraphML
        
    """
    
    from os.path import exists, isfile, join
    from numpy import zeros, array
    
    if not exists(rootfolder):
        raise Exception('Path %s does not exist.' % rootfolder)
        return

    outfile = startstring_coh
    
    # write edge keys
    for freq in freq_bands:
	key = '<key id="%s" for="edge" attr.name="%s" attr.type="double"><default>0</default></key>\n'
	outfile = outfile + key % (freq, freq)
    
    # start graph
    outfile = outfile + '<graph id="%s" edgedefault="undirected">\n' % name
    
    # write nodes (ROIs)
    # use the electrode positions preliminary
    out, nodes = electrode_pos_to_graphml(filename = join(rootfolder, el_cord_file))
    outfile = outfile + nodes
    
    # write edges

    file = join(rootfolder, name+'-'+measure+'-IndVar'+extvar+'.txt')
    if isfile(file):
	#print 'Converting file %s ...' % str(file)
	nr_elect, nr_mat = extract_metadata_stats(filename = file)

	# number of electrodes (ROIs) from the file header must be equal as the one given in the form
	assert nr_elect == nr_rois
	# number of matrices must equal number of frequency bands given
	assert nr_mat == len(freq_bands)
	
	f = open(file, 'r')
	# skip header
	a = f.readline()
	a = f.readline()
	while ' = ' in a  or a == '':
	    a = f.readline()

	# create numpy array for all frequency bands
	data_arr = zeros((nr_elect, nr_elect, nr_mat))

	for mat_i in range(nr_mat):
	    for elect_i in range(nr_elect):
		line = f.readline()
		lsp = line.split(' ')
		
		while lsp.count('') != 0:
		    lsp.remove('')

		assert len(lsp) == nr_elect

		data_arr[elect_i, :, mat_i] = array(lsp)
	    # skip empty line
	    f.readline()
	#print 'Done.'
    else:
	#print 'No file %s existing.' % file
	return
    
                
    # now loop over bigarray and write the the edges
    for i in range(0, nr_rois):
        for j in range(i, nr_rois):
            if i == j:
                # skipping self-loops
                continue
            
            outfile = outfile + '<edge id="e%s_%s" source="n%s" target="n%s">\n' \
                      % (str(i+1), str(j+1), str(i+1), str(j+1))
            
	    for freq_i, frequency_label in enumerate(freq_bands):                            
		# loop over all frequency bands
		
		label, value = frequency_label, data_arr[i,j,freq_i]
		if value == 0.0:
		    continue
		# convert value (float) to truncated string
		value = value
		outfile = outfile + '    <data key="%s">%s</data>\n'\
				    % (label, str(value))
            
	    outfile = outfile + '</edge>\n'

    outfile = outfile + endstring
    return outfile

def coherences2graphml(rootfolder, nr_rois, subject = None, conditions = None, \
                       measures = None, freq_bands = None, el_cord_file = None):
    """ Given a proper folder structure, all subjects, conditions and measures are
    searched and converted to GraphML files for all frequency bands.
    
    If allinone is True, all the files must be stored only in the root folder, but
    subjects, conditions, measures and frequency_bands have to be given according
    to the file name convention.
    
    Example:
    Files like VP07R1-ROIlaggedCoh.txt with the classical frequency bands amounts to calling
    (number of ROIs and number of frequency_bands is according to sLORETA selections)
    
    rootfolder = '/', nr_rois, subjects = 'VP07', conditions = ['R1'], \
        measures = ['ROIlaggedCoh'], freq_band['alpha1', 'alpha2'], el_cord_file = 'List19e.sxyz'
    
    """

    from os.path import exists, isfile, join
    from numpy import zeros, array
    
    if not exists(rootfolder):
        raise Exception('Path %s does not exist.' % rootfolder)
        return

    outfile = startstring_coh
    
    # write edge keys
    for cond in conditions:
        for meas in measures:
            for freq in freq_bands:
                key = '<key id="%s-%s-%s" for="edge" attr.name="%s.%s.%s" attr.type="double"><default>0</default></key>\n'
                outfile = outfile + key % (cond, meas, freq, cond, meas, freq)
    
    # start graph
    outfile = outfile + '<graph id="%s" edgedefault="undirected">\n' % subject
    
    # write nodes (ROIs)
    # use the electrode positions preliminary
    out, nodes = electrode_pos_to_graphml(filename = join(rootfolder, el_cord_file))
    outfile = outfile + nodes
    
    # write edges
    bigarray = {}
    for cond in conditions:
        for meas in measures:
            file = join(rootfolder, subject+cond+'-'+meas+'.txt')
            if isfile(file):
                #print 'Converting file %s ...' % str(file)
                nr_elect, nr_mat = extract_metadata(filename = file)
		# number of electrodes (ROIs) from the file header must be equal as the one given in the form
                assert nr_elect == nr_rois
		# number of matrices must equal number of frequency bands given
		assert nr_mat == len(freq_bands)
                
                f = open(file, 'r')
                # skip header
                a = f.readline()
                while ' = ' in a  or 'File name' in a or a == '':
                    a = f.readline()
                # create numpy array for all frequency bands
                data_arr = zeros((nr_elect, nr_elect, nr_mat))
                
                for mat_i in range(nr_mat):
		    #print 'Matrix: ', str(mat_i)
                    for elect_i in range(nr_elect):
			#print 'ROI:', str(elect_i)
                        line = f.readline()

                        lsp = line.split('  ')
			try:
			    lsp.remove('') # remove first empty character
			except ValueError:
			    break

                        assert len(lsp) == nr_elect
                        data_arr[elect_i, :, mat_i] = array(lsp)
					    
                #print 'Done.'
                
                bigarray[cond + '-' + meas] = data_arr
                # now store data_arr
                
    # now loop over bigarray and write the the edges
    for i in range(0, nr_rois):
        for j in range(i, nr_rois):
            if i == j:
                # skipping self-loops
                continue
            
            outfile = outfile + '<edge id="e%s_%s" source="n%s" target="n%s">\n' \
                      % (str(i+1), str(j+1), str(i+1), str(j+1))
            
            for k, v in bigarray.items():
                # number of matrices has to be the same as the number of frequency bands
                assert v.shape[2] == len(freq_bands)
                for freq_i, frequency_label in enumerate(freq_bands):
                    # loop over all frequency bands
                    label, value = k+'-'+frequency_label, v[i,j,freq_i]
		    if value == 0.0:
			continue

                    # convert value (float) to truncated string
                    value = value
                    outfile = outfile + '    <data key="%s">%s</data>\n'\
                                        % (label, str(value))
            outfile = outfile + '</edge>\n'

    outfile = outfile + endstring
    return outfile

startstring_coh = """<?xml version="1.0" encoding="UTF-8"?>

<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:xlink="http://www.w3.org/1999/xlink"

	xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns 
	http://www.connectome.ch/graphml+xlink+gifti.xsd">

<key id="dn_label" for="node" attr.name="label" attr.type="string">
	<desc>The label of the ROI</desc>
</key>
<key id="dn_position" for="node" attr.name="position" attr.type="string">
	<desc>The position of the ROI</desc>
</key>
"""

def extract_metadata(filename):
    """ Extract metadata, namely number of electrodes and nr of matrices
    and band type """
        
    f = open(filename, 'r')
    for line in f:
        lsp = line.split(' = ')
        if len(lsp) == 1:
            continue
        elif lsp == '':
            break

        if 'Number Electrodes' in lsp[1] or 'Number of regions of interest' in lsp[1]:
            nr_elect = int(lsp[0])
        elif 'Number of matrices' in lsp[1]:
            nr_mat = int(lsp[0])
        
    f.close()
    return nr_elect, nr_mat

def extract_metadata_stats(filename):
    """ Extract metadata for statistics file, namely number of electrodes and nr of matrices
    and band type """
        
    f = open(filename, 'r')
    for line in f:
	
        lsp = line.split(' = ')
        if len(lsp) == 1:
            continue

        if 'Number of regions of interest' in lsp[1]:
            nr_elect = int(lsp[0])
        elif 'Number of matrices' in lsp[1]:
            nr_mat = int(lsp[0])
        
    f.close()
    return nr_elect, nr_mat


