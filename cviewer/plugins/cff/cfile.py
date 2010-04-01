""" The class for the main data object holding all information about a loaded
    connectome file
"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# Standard library imports
import os, os.path
import tempfile
from threading import Thread

# Enthought library imports
from enthought.traits.api import HasTraits, Instance, Any, Str, File, List, Bool

# ConnectomeViewer imports
from interfaces.i_meta import IMeta
from interfaces.i_network import INetwork

from network import Network
from volume import Volume
from trackfile import Trackfile

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

######################################################################
# CFile` class.
######################################################################
class CFile(HasTraits):
    """This class basically allows you to create an instance for a
    loaded .cff file, and serves as the primary data source for scenes etc.
    
    The view shows all the relevant attributes.
    """

    # The name of the selected cff file
    file_name = File
    
    # Full pathname to file including filename
    fullpathtofile = Str

    # Meta data
    metadata = Instance(IMeta)
       
    # Network data
    networks = List(INetwork)
    
    # standard name
    name = Str('Connectome File')

    #######################################
    # Private traits.
    #######################################
    
    # zipfile
    _filezip = Any
    
    # data loaded
    _data_loaded = Bool(False)
    
    # reference to the current window for statusbar updates
    # not nice MVC-style
    _workbenchwin = Instance('enthought.pyface.workbench.api.WorkbenchWindow')
    
        
    ######################################################################
    # `object` interface.
    ######################################################################
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
    def load_cfile(self, filepath):
        """ Load a given cfile as path and initializes the attributes """
        
        if not os.path.isfile(filepath):
            logger.error('Not existing file: %s' %filepath)
            return
        
        # set the fullpath to internal trait
        self.fullpathtofile = filepath
        # set the file name and make it visible in the TraitsUIView
        self.file_name = os.path.split(filepath)[1]
    
        # open the .cff file with Zip
        from zipfile import ZipFile, ZIP_DEFLATED
        self._filezip = ZipFile(self.fullpathtofile, 'r', ZIP_DEFLATED)
        metadatastring = self._filezip.read('meta.xml')
        
        # start parsing the metadata
        self.open_parse_meta_xml(metadatastring)

        # setting the name
        self.name = self.metadata.name

        action = ThreadedParsing(self, metadata = metadatastring)
        action.start()
        
        
    def close_cfile(self, close_scenes=False):
        """ Closes the loaded data of the cfile including the temporary files
        
        Parameters
        ----------
        close_scenes : bool
            Trying to close the scenes as well?
      
        """
        
        if self._data_loaded:
            for netw in self.networks:        
                # remove all the scenes
                if not netw.rendermanager is None:
                    if close_scenes:
                        netw.rendermanager.close_scenes()
                        netw.rendermanager = None
            
            self.fullpathtofile = ''
            self.metadata = None
            self.networks = []
            self._filezip = None
          
        self._data_loaded = False


    def open_parse_meta_xml(self, metaxmlcontent = None):
        """ Open the meta.xml from the zip archive given its full pathname
            and create an instance of metadata
        
        Parameters
        ----------
        metaxmlcontent : string
            Metadata meta.xml from a cfile
            
        """
        from meta import Meta
        
        if not metaxmlcontent is None:
            self.metadata = Meta(metaxmlcontent)

    def remove_network(self, networkid):
        """ Removes the networkid'th network from the cfile """
        if len(self.networks) <= networkid:
                return
        self.networks[networkid].active = False
        del self.networks[networkid]

    def add_network_from_matrix_with_pos(self, name, matrix, pos, nodeinfo_like_graph = None, \
                                         metadata = None, directed = False, hierarchical = False, hypergraph = False):
        """ Add a network to the cfil based on the given connectivity matrix and
        position information.
        
        Parameters
        ----------
        name : String
            Name of the newly added network
        matrix : NxN array
            A NxN Numpy array with the values for the edges. N must be the same
            as len(like_network.graph.nodes()). The first row of the matrix
            corresponds to the `NetworkX` node with id 'n1'
        pos : Nx3 array
            A Nx3 array with the X,Y,Z coordinates for every row (=node) of the matrix
        nodeinfo_like_graph : `NetworkX` graph
            Use the node information for the new network
        metadata : Dict
            A dictionary of metadata
        directed : bool
            Is the network directed?
        hierarchical : bool
            Is the network hierarchical? (default: '0') Not implemented yet.
        hypergraph : bool
            Is the network a hypergraph? (default: '0') Not implemented yet.
            
        Returns
        -------
        networkid : int
            The index in the list of networks in the cfile
        """
        from cviewer.plugins.cff.network import Network
        
        # create a new network object
        my_network = Network(name = name, directed = directed, hierarchical = hierarchical, \
                             hypergraph = hypergraph)
        
        if not metadata is None:
            my_network.metadata = metadata

        # add graph 
        import networkx as nx
        test_G = nx.from_numpy_matrix(matrix)
        
        # relabeling function
        addn = lambda nmod:'n'+str(nmod+1)
        
        # relabel graph
        test_G_corrected = nx.relabel_nodes(test_G, addn)

        # update the node position
        for i in range(matrix.shape[0]):
            nodeid = addn(i)
            if not nodeinfo_like_graph is None and nodeinfo_like_graph.has_node(nodeid):
                test_G_corrected.node[nodeid] = nodeinfo_like_graph.node[nodeid]
            test_G_corrected.node[nodeid]['dn_position'] = ','.join(list(map(str,pos[i, :])))

        my_network.graph = test_G_corrected
        
        # setting the correct parent cfile
        my_network._parentcfile = self


        # add the created network object to this cfile
        self.networks.append(my_network)
        
        # returns the id of the added network
        return len(self.networks)-1
        
        
    def add_network_from_matrix(self, name, matrix, like_network = None, metadata = None, \
                                directed = False, hierarchical = False, hypergraph = False):
        """ Add a network to the cfile based on the given connectivity matrix and
        an already existing other network with node and surface container information.
        
        Parameters
        ----------
        name : String
            Name of the newly added network
        matrix : NxN array
            A NxN Numpy array with the values for the edges. N must be the same
            as len(like_network.graph.nodes()). The first row of the matrix
            corresponds to the `NetworkX` node with id 'n1'
        like_network : `INetwork` object, optional
            Used to copy graph node information and surface containers
        metadata : Dict, optional
            A dictionary of metadata
        directed : bool, optional
            Is the network directed?
        hierarchical : bool, optional
            Is the network hierarchical? (default: '0') Not implemented yet.
        hypergraph : bool, optional
            Is the network a hypergraph? (default: '0') Not implemented yet.
            
        Returns
        -------
        networkid : int
            The index in the list of networks in the cfile
            
        Note
        ----
        If you use this function only with name and matrix, a spring-force layout
        algorithm is applied to define the node positions. This may take some time.
        You can also use the `add_network_from_matrix_pos` and define the
        node positions by yourself.
        
        """
        from cviewer.plugins.cff.network import Network
        
        create_graph_info = False
        
        if like_network is None:
            create_graph_info = True
        else:
            # matrix has N rows and columns which is equal to the number of nodes
            assert matrix.shape[0] == matrix.shape[1] == len(like_network.graph.nodes())

        # add graph 
        import networkx as nx
        test_G = nx.from_numpy_matrix(matrix)
        
        # relabeling function
        addn = lambda nmod:'n'+str(nmod+1)
        
        # relabel graph
        test_G_corrected = nx.relabel_nodes(test_G, addn)
        
        nodes_ids = test_G_corrected.nodes()
        
        # create node positions with springlayout if create_graph_info
        if create_graph_info:
            npos = nx.spring_layout(test_G_corrected, dim = 3, weighted = True, scale = 100)
        
        # add node properties
        if not create_graph_info:
            for k, v in like_network.graph.nodes(data=True):
                if not k in nodes_ids:
                    logger.error('Node id %s is not like_network.graph' % str(k))
                # add node properties
                test_G_corrected.node[k] = v
        else:
            for k in test_G_corrected.nodes():
                li = str(npos[k].tolist())
                li = li.replace('[','')
                li = li.replace(']','')
                test_G_corrected.node[k]['dn_position'] = li
                
                
        # create a new network object
        my_network = Network(name = name, directed = directed, hierarchical = hierarchical, \
                             hypergraph = hypergraph, graph = test_G_corrected)
        
        if not metadata is None:
            my_network.metadata = metadata
            
        # setting the correct parent cfile
        my_network._parentcfile = self
                
        if not create_graph_info:
            # add surface container from like_network
            action = ThreadedParsing(self)
            for surf in like_network.surfaces:
                if surf.is_atlas:
                    tmp_surfcont = action.process_atlas(surface_node=surf._lxml_node, \
                                                        nsprefix=surf._lxml_node_prefix)
                else:
                    tmp_surfcont = action.process_surface(surface_node=surf._lxml_node, \
                                                        nsprefix=surf._lxml_node_prefix)
                my_network.add_surface_container(tmp_surfcont)


        # add the created network object to this cfile
        self.networks.append(my_network)
        
        # returns the id of the added network
        return len(self.networks)-1
        

class ThreadedParsing(Thread):
    """ Parse the content of the XML data in a seperate thread """
  
    def __init__(self, data, metadata = None, **kwargs):
        Thread.__init__(self, **kwargs)
        self.data = data
        if not metadata is None:
            self.metaxmlcontent = metadata

    def process_pickle(self, childnode):
        """ Checks if there is a pickled version for the network for faster
        loading. If not, generate one and store it in the cfile.
        
        Returns
        -------
        tmpstring: StringIO
            The GraphML we have to parse into a NetworkX Graph
        pickled_graph:
            The NetworkX read from the pickled file
        local_fname:
            The filename the pickle is saved in the cfile
        """

        child = childnode

        # check if a pickled form exists
        nx_local_src_path = child.attrib['src']
        
        if not '/' in nx_local_src_path:
            logger.error('You have to use "/" in the meta.xml for the local source path "src=" of the networks!')
            raise Exception('Can not open this connectome file.')
            
        tmpsplit = nx_local_src_path.split('/')
        tmpsplit[0] = 'Pickle'
        nx_local_src_path_pickle = '/'.join(tmpsplit)
        if nx_local_src_path_pickle in self.data._filezip.namelist():
            
            logger.info('Found a pickled graph.')
            logger.info('Load the pickle: '+str(nx_local_src_path_pickle))
            
            # extract pickle from archive to tmp area
            tmppicklefile=tempfile.mkstemp(prefix='picklefile', suffix='.pickle')
            os.write(tmppicklefile[0],self.data._filezip.read(nx_local_src_path_pickle))
            
            # give the pickled version to the path
            from networkx import read_gpickle
            pickled_graph = read_gpickle(tmppicklefile[1])
            os.close(tmppicklefile[0])
            
            # remove pickle file from system
            logger.debug('Unlink: %s' % tmppicklefile[1])
            os.unlink(tmppicklefile[1])
            
            tmpstring = None
            
        else:
            # otherwise load the source, parse it
            # and directly store a pickled version in the connectome file
        
            if child.attrib['src'] in self.data._filezip.namelist():
                from StringIO import StringIO
                network_source = self.data._filezip.read(child.attrib['src'])
                tmpstring =  StringIO(network_source)
                pickled_graph = None
                
            else:
                logger.error('The file %s was not found in the Connectome File!' % child.attrib['src'])
                raise ('The file %s was not found in the Connectome File!' % child.attrib['src'])
                
        return (tmpstring, pickled_graph, tmpsplit[1])

    def save_pickle_in_cfile(self, local_fname, networkref):
        """ Creates a pickled version of the graph and stores it in the
        cfile
        
        Parameters
        ----------
        local_fname: string
            The filename used in the Pickle folder to store
        networkref: NetworkX Graph instance
            The NetworkX graph to pickle
        
        """

        logger.info('Write a generated graph pickle to the connectome file.')
        picklefilepath = os.path.join(tempfile.gettempdir(),local_fname)
        from networkx import write_gpickle
        # add nodekeys, edgekeys, graphid to helpernode 'n0' before storage
        helperdict = {'nodekeys': networkref.nodekeys.copy(), \
                      'edgekeys': networkref.edgekeys.copy(), \
                      'graphid' : networkref.networkid }
        networkref.graph.add_node('n0')
        networkref.graph.node['n0'] = helperdict
        write_gpickle(networkref.graph, picklefilepath)
        networkref.graph.remove_node('n0')
        
        from zipfile import ZipFile, ZIP_DEFLATED
        tmpzipfile = ZipFile(self.data.fullpathtofile, 'a', ZIP_DEFLATED)
        # store it in the zip file
        tmpzipfile.write(picklefilepath, 'Pickle/' + local_fname)
        tmpzipfile.close()
        
        # remove pickle file from system
        logger.debug('Unlink: %s' % picklefilepath)
        os.unlink(picklefilepath)


    def process_surface(self, surface_node, nsprefix):
        """ Processing of the surface node
        
        Parameters
        ----------
        surface_node: the lxml node
            The XML node network-surface from the meta.xml file
        nsprefix: String
            The Name Space Prefix for clean iterating
        
        Returns
        -------
        tmpsurfacecontainer: `ISurfaceContainer` instance to add to the network
        
        """
        from surface_container import SurfaceContainer
        
        # create surface
        tmpsurfacecontainer = SurfaceContainer(name = surface_node.attrib['name'], \
                         src = surface_node.attrib['src'])

        tmpsurfacecontainer.set(_lxml_node = surface_node, _lxml_node_prefix = nsprefix, \
                                is_atlas = False)

        # adding label information to the surface container
        labelidentification = ''
        l_cnt = 0
        
        for tmplabels in surface_node.iterchildren():
            
            if tmplabels.tag == (nsprefix+'surface-label'):
                l_cnt = l_cnt + 1
                
                if l_cnt > 2:
                    logger.error('There are multiple label for one surface container! Please check your meta.xml')
                    continue
                
                lattr = tmplabels.attrib
                if lattr.has_key('labelid'):
                    labelidentification = lattr['labelid']
                    if lattr.has_key('src'):
                        tmpsurfacecontainer._add_label(labelid = labelidentification,\
                                                    src = lattr['src'])
                        
        return tmpsurfacecontainer

    def process_atlas(self, surface_node, nsprefix):
        """ Processing of the surface node for atlases
        
        Parameters
        ----------
        surface_node: the lxml node
            The XML node network-surface from the meta.xml file
        nsprefix: String
            The Name Space Prefix for clean iterating
        
        Returns
        -------
        tmpsurfacecontainer: `ISurfaceContainer` instance to add to the network
        
        """
        from surface_container import SurfaceContainer

        if not cmp(surface_node.attrib['addatlas'], 'template_atlas_homo_sapiens_01'):
            logger.debug('Load the Homo sapiens Template Atlas 01 ...')
            
            from cviewer.resources.atlases.library import get_template_atlas, get_template_atlas_label
            atlaspathtofile = get_template_atlas(atlastype = 'homo_sapiens_01')
            if os.path.isfile(atlaspathtofile):
                logger.debug('Valid Freesurfer Atlas file. Add it as SurfaceContainer...')
                
                tmpsurfacecontaineratlas = SurfaceContainer(name = 'Freesurfer Atlas', \
                                                            src = atlaspathtofile)
                
                tmpsurfacecontaineratlas.set(_lxml_node = surface_node, _lxml_node_prefix = nsprefix, \
                                             is_atlas = True)
                
                # add the freesurfer label file
                labelidentification = ''
                nolabelid = False
                l_cnt = 0
                
                for tmplabels in surface_node.iterchildren():
                    
                    if tmplabels.tag == (nsprefix+'surface-label'):
                        l_cnt = l_cnt + 1
                        
                        if l_cnt > 2:
                            logger.error('There are multiple label for one surface container! Please check your meta.xml')
                            nolabelid = True
                            continue
                            
                        lattr = tmplabels.attrib
                        if lattr.has_key('labelid'):
                            labelidentification = lattr['labelid']
                            
                              
                if not nolabelid:
                    logger.debug('Using LabelID: %s' % labelidentification)
                    labelfile = get_template_atlas_label(atlastype = 'homo_sapiens_01')
                    
                    tmpsurfacecontaineratlas._add_label(labelid = labelidentification, \
                                                        src = labelfile)
                    
                    return tmpsurfacecontaineratlas
                
                else:
                    logger.error('No valid labelid is given in the meta.xml file')
            else:
                logger.debug('No valid file. Freesurfer Atlas not loaded.')
        

    def process_metadata(self, metadata_node, nsprefix):
        """ Processes the metadata in meta.xml for a network
        
        Returns
        -------
        metadata_dict : dictionary
            The processed dictionary
            
        """
        tmpdict =  {}
        for tmpdata in metadata_node.iterchildren():

            if tmpdata.tag == (nsprefix+'data'):
                # get the key/value
                tmpdict[tmpdata.attrib['key']] = tmpdata.text
        
        return tmpdict
    
    def run(self):
        logger.debug("Start Threaded Parsing of Connectome File ...")
        import time
        tic = time.time()
        
        # given the full path to meta.xml, create lxml tree and
        # under the precondition of having a certain file version and generator
        # one can assume some content of the .cff file
        # loop over the networks, for each network, the surfaces etc.
        # in the form of all the attributes (name, srce filename, description etc.)
        # are given to the created Network instance methods to complete
        from lxml import etree
        tree = etree.fromstring(self.metaxmlcontent)   
        nsprefix = "{%s}" % tree.nsmap[None]
        
        t = 0
        for child in tree.iterchildren():
            if child.tag == (nsprefix+'viewer-network'):
                t = t + 1

        if t != self.data.metadata.nr_of_networks:
            logger.error('The number of networks of networks in meta.xml is not correct!')
            logger.error('Loading ' + str(t) + ' network(s).')


        # find all the networks
        for child in tree.iterchildren():
            if child.tag == (nsprefix+'viewer-network'):

                # load network as string from zip
                # create temporary StringIO
                logger.info('-----------------------------------------')
                logger.info('Now loading ' + child.attrib['src'])
                logger.info('-----------------------------------------')
                
                if child.attrib.has_key('src'):
                    (tmpstring, pickled_graph, local_fname) = self.process_pickle(childnode = child)
                
                tmp_network = Network(name = child.attrib['name'],
                                    src = tmpstring,
                                    pickled_graph = pickled_graph,
                                    directed = child.attrib['directed'],
                                    hierarchical = child.attrib['hierarchical'],
                                    hypergraph = child.attrib['hypergraph'])

                # store pickled version in connectome file
                if not tmpstring is None and pickled_graph is None:
                    self.save_pickle_in_cfile(local_fname, networkref = tmp_network)

                
                # iterate over children
                for mchildren in child.iterchildren():
                    
                    # handling of the metadata
                    # -------------------------
                    if mchildren.tag == (nsprefix+'network-metadata'):
                        tmp_network.metadata = self.process_metadata(metadata_node = mchildren, nsprefix = nsprefix)
                        
                        
                    # handling of the surfaces
                    # ------------------------
                    if mchildren.tag == (nsprefix+'network-surface'):
                    
                        # load into surface
                        attr = mchildren.attrib
    
                        if attr.has_key('src'):
                            # create surface container
                            tmpsurfacecontainer = self.process_surface(surface_node = mchildren, nsprefix = nsprefix)
                            # and add it to the network
                            tmp_network.add_surface_container(tmpsurfacecontainer)

                        # adding a template atlas if stated in the meta.xml
                        if attr.has_key('addatlas'):
                            # create surface container for atlas
                            tmpsurfacecontainer = self.process_atlas(surface_node = mchildren, nsprefix = nsprefix)
                            # and add it to the network
                            tmp_network.add_surface_container(tmpsurfacecontainer)
                            
                        
                    # handling of the volumes
                    # -----------------------
                    elif mchildren.tag == (nsprefix+'network-volume'):
                        
                        # get the description, is there an easier way with lxml?
                        for desc in mchildren.iterchildren():
                            if desc.tag == (nsprefix+'description'):
                                mydescr = desc.text
                      
                        attr = mchildren.attrib
                        
                        if attr.has_key('src') and attr.has_key('name'):
                          
                            if not attr.has_key('segmentation'):
                                segm = False
                            else:
                                segm = bool(attr['segmentation'])
                          
                            # create an instance of the Volume class with it
                            tmpvolume = Volume(volumename = attr['name'], \
                                                 networkref = tmp_network, \
                                                 src = attr['src'], \
                                                 description = mydescr, \
                                                 segmentation = segm)
                            
                            # add this instance to the list of its network
                            tmp_network.add_volume(tmpvolume)
                            logger.info('Added volume ' + tmpvolume.volumename + \
                                        ' to ' + tmp_network.networkname)

                            
                    # handling of the trackfiles
                    # --------------------------
                    elif mchildren.tag == (nsprefix+'network-track'):
                        
                        # get the description, is there an easier way with lxml?
                        for desc in mchildren.iterchildren():
                            if desc.tag == (nsprefix+'description'):
                              mydescr = desc.text
                      
                        attr = mchildren.attrib
                    
                        if attr.has_key('src') and attr.has_key('name'):
                            tmptrack = Trackfile(trkname = attr['name'], tmpfname = attr['src'], \
                                                 description = mydescr, networkref = tmp_network)
                            
                            # add this instance to the list of its trackfiles
                            tmp_network.add_trackfile(tmptrack)
                            logger.info('Added trackfile ' + tmptrack.trkname + \
                                        ' to ' + tmp_network.networkname)

                # setting the correct parent cfile
                tmp_network._parentcfile = self.data
                
                # add the created network object to this cfile
                self.data.networks.append(tmp_network)

        toc = time.time()
        logger.info('Loading Connectome File DONE. (Time: %s)' % str(toc-tic))
        logger.info('-----------------------------------------')
        
        # setting data loaded flag
        self.data._data_loaded = True
        
        # with do_later, we the change in the statusbar is done in the gui main thread
        if not self.data._workbenchwin is None:
            #from enthought.pyface.timer.api import do_later
            #do_later(self.data._workbenchwin.status_bar_manager.set, message = '')
            # TEST
            from enthought.pyface.api import GUI
            GUI.invoke_later(self.data._workbenchwin.status_bar_manager.set, message = '')
        