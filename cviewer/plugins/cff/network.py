""" The implementation of the 'INetwork' interface. """
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
import os

# Enthought library imports
from enthought.traits.api import HasTraits, Str, Bool, CBool, Any, Dict, implements, \
      List, Instance, DelegatesTo
from enthought.traits.ui.api import View, Item, auto_close_message, message

# ConnectomeViewer imports
from interfaces.i_network import INetwork
from interfaces.i_volume import IVolume
from interfaces.i_trackfile import ITrackfile
from interfaces.i_surface_container import ISurfaceContainer
from cviewer.visualization.render_manager import RenderManager
from cviewer.sources.datasource_manager import DatasourceManager
from cviewer.plugins.cff.trackvis.trackvis import generate_scene_file, ThreadedTrackvis
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.plugins.cff.ui.edge_parameters_view import EdgeParameters

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

class Network(HasTraits):
    """ The implementation of the Connectome Networks """

    implements(INetwork)

    # Network ID, from parsed GraphML the graphid
    networkid = ''

    # Network name
    networkname = Str
    
    # network name as seen in the TreeView
    name = Str
    
    # Is it an hierarchical network?
    hierarchical = CBool(False)

    # TODO: later, also Hypergraph?!
    # see: http://www.ploscompbiol.org/article/info%3Adoi%2F10.1371%2Fjournal.pcbi.1000385
    hypergraph = CBool(False)

    # Directionality of the Network, {True: 'directed', False: 'undirected'}
    directed = CBool(False)

    # metadata for the network
    metadata = Dict

    # NodeKeys from the parsed GraphML
    # These are Dict of Dict, all having strings
    nodekeys = {}
    
    # Edgekeys, from parsed GraphML
    edgekeys = {}

    # A NetworkX AttrGraph containing all the information
    graph = Any
        
    # Surface containers
    surfaces = List(ISurfaceContainer)
    
    # Surface containers loaded
    surfaces_loaded = List(ISurfaceContainer)
    
    # Volume data
    volumes = List(IVolume)
    
    # Track data
    tracks = List(ITrackfile)

    # is this network active, and thus a render manager displayed?
    active = Bool

    # the render manager of this network
    rendermanager = Instance(RenderManager)
    
    # DatasourceManager Instance of this network
    datasourcemanager = Instance(DatasourceManager)
    
    # private traits
    ###########
    
    # parent cfile this networks belongs to
    _parentcfile = Any

    # filezip of cfile
    _filezip = DelegatesTo('_parentcfile')

    # edge parameters for visualization
    _edge_para = Instance(EdgeParameters)

    # View
    traits_view = View(
        Item('networkname', style = 'readonly'),
        Item('hierarchical', style = 'simple'),
        Item('hypergraph', style = 'simple'),
        Item('directed', style = 'simple'),
        Item('active', style = 'simple'),
        title   = 'A network', 
    )

    def __init__(self, name, src = None, directed = '0', pickled_graph = None, \
                 hierarchical ='0', hypergraph = '0', graph = None):
        """ Initializes the network and sets the traits.
        
        Parameters
        ----------
        name : string
            the name of the network
        src : file handle or StringIO object
            the source text of the network to parse
        pickled_graph : NetworkX graph
            reference to a graph object, src should be None
        directed : bool
            Is the network directed?
        hierarchical : bool
            Is the network hierarchical? (default: '0') Not implemented yet.
        hypergraph : bool
            Is the network a hypergraph? (default: '0') Not implemented yet.
        
        """
        
        # initialize the traits
        self.networkname = name
        self.directed = int(directed)
        self.hierarchical = int(hierarchical)
        self.hypergraph = int(hypergraph)
        
        if src is None and not pickled_graph is None:
            self.load_pickled_graphml(pickled_graph)
        else:
            if not src is None:
                # generates NetworkX Graph
                self.graph = self.parse_network_graphml(src)
            elif not graph is None:
                self.graph = graph
            else:
                logger.error('Cannot initialize network. No graph information available')
                
        # initializes the weight key of the graph
        # with the first edgekey
        if len(self.edgekeys) > 0:
            edgk = self.edgekeys.keys()
            if not 'weight' in edgk:
                self.set_weight_key(edgk[0])
        else:
            # try grabbing first edge from the graph
            if self.graph.number_of_edges() > 0:
                it = self.graph.edges_iter(data=True)
                edg = it.next()
                if len(edg[2]) > 0:
                    # if it has a weigth key, just leave it
                    edgk = edg[2].keys()
                    if not 'weight' in edgk:
                        self.set_weight_key(edgk[0])
            else:
                logger.error('Cannot set weight key for network : ' + self.networkname)
                
    def _name_default(self):
        return self.networkname

    def _active_default(self):
        return False

    def _active_changed(self , value):
        if value:
            n = self.name
            if ' [Active]' not in n:
                self.name = "%s [Active]" % n
                
            # XXX: do refactor with threaded loading of surfaces
            # and default spring force layout for graph rendering!
            # see also TraitsUI Demos: Multi thread demo
            
            # load the surface containers data
            # make a deep copy of the already loaded surface containers
            import copy
            self.surfaces = copy.deepcopy(self.surfaces_loaded)
            for surfcont in self.surfaces:
                surfcont.load_surface_container()
            
            if self.rendermanager is None:
                self._create_datasourcemanager()
                self._create_renderer()
                # if there are no surfaces, initialize
                # network rendering, but only if dn_positions are given
                if len(self.surfaces) == 0:
                    logger.debug('No surfaces found. Try to render graph view with dn_position information.')
                    self.rendermanager.datasourcemanager._compute_3DLayout(-1, -1)
                    self.rendermanager.visualize_graph()
                else:
                    logger.debug('SurfaceContainer found. Try to render 3D View using %s.' % self.surfaces[0].name)
                    if len(self.surfaces[0].surfaces) == 0:
                        logger.debug('Rendering not possible because SurfaceContainer contains no surfaces.')
                    else:
                        logger.debug('Using first surface for rendering.')
                        self.surfaces[0].surfaces[0]._layout_3DView()
            
            if not self._parentcfile._workbenchwin is None:
                #from enthought.pyface.timer.api import do_later
                from enthought.pyface.api import GUI
                GUI.invoke_later(self._parentcfile._workbenchwin.status_bar_manager.set, message = '')
            
        else:
            self.name = self.name.replace(' [Active]', '')
            logger.debug('Close RenderManager scenes')
            self.rendermanager.close_scenes()
            logger.debug('All scenes closed.')
            # FIXME: what is happening in the following?
            # e.g. for instances. e.g. reset traits?
            # XXX: this is somehow not correct. do i need to use del
            # or remove/reset traits?
            self.rendermanager = None
            self.datasourcemanager = None
            self.surfaces = []

    def _de_activate(self):
        """ Toggles the internal state of the activation """
        if self.active:
            self.active = False
        else:
            self._parentcfile._workbenchwin.status_bar_manager.message = 'Activating network ...'
            self.active = True

    def _edge_parameters(self):
        """ Dialog to change edge attribute and thresholding """
        if self._edge_para is None:
            self._edge_para = EdgeParameters(self, self.rendermanager.attract.point_scalars_name)
            
        self._edge_para.configure_traits()


    def _create_renderer(self):
        """ Creates the renderer instance if not yet available
        and opens the scenes in mayavi """

        if self.active:
            if self.rendermanager is None:
                logger.debug('Create a RenderManager instance')
                self.rendermanager = RenderManager(network=self)
            else:
                logger.debug('RenderManager instance already running. This is an error.')

    def _create_datasourcemanager(self):
        """ Creates the datasource manager instance if not yet available """
        if self.active:
            if self.datasourcemanager is None:
                logger.debug('Create a DatasourceManager instance')
                self.datasourcemanager = DatasourceManager(network=self)
            else:
                logger.debug('DatasourceManager instance already running. This is an error.')


    def _render_matrix(self):
        """ Invokes the connectivity matrix viewer """
        # assume the network is activated (i.e. data source generated)
        # we need the edge parameter instance initialized
        if self._edge_para is None:
            self._edge_para = EdgeParameters(self, self.rendermanager.attract.point_scalars_name)
            
        logger.debug('Invoke Matrix Viewer...')
        self.rendermanager.invoke_matrix_viewer()
        
    def _trackvis_launch(self):
        """ Generates scene file and launch Trackvis on the selected nodes """
        import tempfile
        
        logger.debug('Starting TrackVis ...')

        # extract selected subgraph
        selectionlist = self.get_selectiongraph_list()
        
        if len(selectionlist) == 0:
            # message            
            from enthought.traits.ui.message import message
            message(message = 'No nodes selected for ROI creation!', title = 'Infomessage', buttons = [ 'OK' ], parent = None)

        tmpgraph = self.graph.subgraph(selectionlist)

        # extract trackfile temporarily
        if len(self.tracks) == 0:
            logger.info('No trackfile found to invoke Trackvis.')
            return
        else:

            # load the first trackfile
            trackfname = self.tracks[0].load_trackfile_to_file()

            # find the first valid segmentation volume in the self.volumes list
            for vol in self.volumes:
                if vol.segmentation:
                    logger.debug('Found a segmentation volume file. Assume labels are corresponding.')
                    volumefname = vol.load_volume_to_file()
                    break

        # generate the scene file in the temporary folder
        tmpscenefile=tempfile.mkstemp(prefix='tmp', suffix='.scene')
            
        # generate trackfile        
        generate_scene_file(scenefname=tmpscenefile[1], \
                          trackfname = trackfname, \
                          volumefname = volumefname, \
                          selectiongraph = tmpgraph)
        
        # execute trackvis in a thread
        pref = preference_manager.preferences       
        action = ThreadedTrackvis(tvpath = pref.get('cviewer.plugins.ui.trackvispath'), \
                                    fname = tmpscenefile[1], \
                                    trkfname = trackfname,\
                                    volfname = volumefname)
        action.start()
    

    def add_surface_container(self, surfacecontainer):
        """ Add a surface container to the loaded list
        
        Parameters
        ----------
        surfacecontainer : `ISurfaceContainer` instance
            a surface container object
        
        """
        surfacecontainer._networkref = self
        self.surfaces_loaded.append(surfacecontainer)

    def add_volume(self, volume):
        """ Adds a volume to the volumes list
        
        Parameters
        ----------
        volume : `IVolume` instance
            a volume object
        
        """
        self.volumes.append(volume)
        
    def add_trackfile(self, trackfile):
        """ Adds a trackfile to the tracks list
        
        Parameters
        ----------
        trackfile : `ITrackfile` instance
            a trackfile of type ITrackfile
        
        """
        self.tracks.append(trackfile)

    def unselect_all(self):
        """ Unselects every node in the current network """
        if self.datasourcemanager is None:
            raise Exception('No DatasourceManager. You have to first activate the network and render it.')
        from numpy import array
        # get all the nodes
        graphnodes = self.datasourcemanager._srcobj.relabled_graph.nodes()
        # and unselect all nodes
        self.rendermanager._select_nodes(selection_node_array = array(graphnodes))

    def select_all(self):
        """ Selects all nodes in the current network """
        if self.datasourcemanager is None:
            raise Exception('No DatasourceManager. You have to first activate the network and render it.')
        from numpy import array
        # get all the nodes
        graphnodes = self.datasourcemanager._srcobj.relabled_graph.nodes()
        # and select all nodes
        self.rendermanager._select_nodes(selection_node_array = array(graphnodes), activate = True)
  
    def set_selectiongraph(self, sellist, activate = False):
        """ Sets the selected nodes in the network to active.
        
        Parameters
        ----------
        sellist : array_like
            a list of nodeids conforming to the NetworkX node id
        activate : boolean
            set the selectionlist nodes to activated?
        
        """
        from numpy import array, int16
        graphnodes = self.graph.nodes(data=False)

        if self.rendermanager is None:
            raise Exception('No RenderManager. You have to first activate the network and render it.')

        if len(sellist) == 0:
            self.unselect_all()
            return
        
        from numpy import array, append
        tmparr = array([])
        for node in sellist:
            # check if it is a valid graph node id
            if node in graphnodes:
                # get the node id as integer
                j = int(node.lstrip('n'))-1
                # extend empty array with node id
                tmparr = append(tmparr, j)
                
        self.rendermanager._select_nodes(selection_node_array = array(tmparr, dtype = int16), activate = activate)

    def get_selectiongraph_list(self):
        """ Returns a list of the node ids that were selected in
        the rendered scene.
        
        """
        if self.datasourcemanager is None:
            raise Exception('No DatasourceManager. You have to first activate the network and render it.')
        
        import numpy as np
        
        sel_list = []
        
        if not self.active:
            return sel_list
        
        selnodesarray = self.datasourcemanager._srcobj.selected_nodes
        
        # array with indices where the nodes are selected (==1)
        idx = np.where(selnodesarray == 1)[0]
        
        for i in idx:
            sel_list.append('n' + str(i + 1))
        
        return sel_list
        

    def set_weight_key(self, weight_key = None):
        """ Sets the weight key in the graph representation of the network.
        
        Parameters
        ----------
        weight_key : Str
            Must be a possible existing edge key
            
        """
        if not weight_key is None:
            for u,v,d in self.graph.edges(data=True):
                self.graph[u][v]['weight']=d[weight_key]
            return True
        else:
            return False

    def get_matrix(self, weight_key = None):
        """ Returns the connectivity matrix of the network with the nodes
        ordered according to their id in the GraphML file.
        
        Parameters
        ----------
        weight_key : Str
            Possible key value of the edges
        
        Returns
        -------
        matrix : `Numpy.array` instance
            The connectivity matrix
        
        """
        nr_nodes = len(self.graph.nodes())
        
        if not weight_key is None:
            #FIXME: sanity check if weight_key exists
            # thanks to Aric Hagberg
            for u,v,d in self.graph.edges(data=True):
                self.graph[u][v]['weight']=d[weight_key]
                
        nodes = [(lambda nmod:'n'+str(nmod))(node) for node in range(1,nr_nodes + 1)]
        from networkx import to_numpy_matrix
        return to_numpy_matrix(self.graph, nodelist = nodes)

    def toggle_surface(self):
        """ Toggle the surface for the selected network nodes """
        if self.rendermanager is None:
            raise Exception('No RenderManager. You have to first activate the network and render it.')
        self.rendermanager._toggle_surface()
        
    def show_surface(self):
        """ Shows the surface for the selected network nodes """
        if self.rendermanager is None:
            raise Exception('No RenderManager. You have to first activate the network and render it.')
        self.rendermanager._show_surface()

    def load_pickled_graphml(self, graph):
        """ Loads a pickled GraphML file
        
        Parameters
        ----------
        graph : NetworkX Graph instance
            A graph instance
            
        """
        
        # setting the graph
        self.graph = graph
        
        if self.graph.has_node('n0'):
            if self.graph.node['n0'].has_key('nodekeys'):
                # extracting the node keys from the first node
                self.nodekeys = self.graph.node['n0']['nodekeys']
                
            # extracting the edge keys from the first edge (without explanation)
            if self.graph.node['n0'].has_key('edgekeys'):
                self.edgekeys = self.graph.node['n0']['edgekeys']
                
            if self.graph.node['n0'].has_key('graphid'):
                self.networkid = self.graph.node['n0']['graphid']
                
            # remove node
            self.graph.remove_node('n0')
    
    def _return_default_edgevalue(self, edgekeys, key):
        """ Looks up if there is a default value defined, otherwise
        return zero """
        if edgekeys[key].has_key('default'):
            return float(edgekeys[key]['default'])
        else:
            return 0.0
    
    def parse_network_graphml(self, path):
        """ Read network in GraphML format from a path.
        
        Parameters
        ----------
        path : string
            path the the GraphML file
        
        Returns
        -------
        graph : NetworkX `Graph`
            
        """
        import networkx as nx
        from networkx.utils import _get_fh
        from lxml import etree
        
        # Return a file handle for given path.
        # Path can be a string or a file handle.
        # Attempt to uncompress/compress files ending in .gz and .bz2.
        
        fh=_get_fh(path,mode='r')
        
        tree = etree.parse(fh)
        # get the root node from parsed lxml
        root = tree.getroot()
        
        # Schema Validation
        # http://codespeak.net/lxml/validation.html#xmlschema
        
        # define the namespace prefixes
        nsprefix = "{%s}" % root.nsmap[None]
        nsxlink = "{%s}" % root.nsmap['xlink']
        
        nodekeys = {}
        edgekeys = {}
        defaultDirected = [True]
        
        # Parse the KEYs
        for child in root.iterchildren():
            if child.tag == (nsprefix+'key'):
                
                attribs = child.attrib
        
                ddkeys = {}
                for mchildren in child:
                    if mchildren.tag == (nsprefix+'default'):
                        ddkeys['default'] = mchildren.text
                    elif mchildren.tag == (nsprefix+'desc'):
                        ddkeys['desc'] = mchildren.text
        
                if child.attrib['for'] == 'node':
                    # Parse all the node keys
                    # Read in the description and the default (if existing)
                    # dict of dicts for nodes: key1: the id; key2: rest: attr.name, attr.type, desc, default
                    nodekeys[attribs['id']] = {'attr.name' : attribs['attr.name'], \
                                               'attr.type' : attribs['attr.type']}
                    # add default/desc keys if existing
                    nodekeys[attribs['id']] = ddkeys
                        
                elif child.attrib['for'] == 'edge':
                    # Parse all the edge keys
                    # Read in the description and the default (if existing)
                    # dict of dicts for edges: key1: the id; key2: rest: attr.name, attr.type, desc, default
                    edgekeys[attribs['id']] = {'attr.name' : attribs['attr.name'], \
                                               'attr.type' : attribs['attr.type']}
                    # add default/desc keys if existing
                    edgekeys[attribs['id']] = ddkeys
                    
                else:
                    logger.error("The 'for' attribute of key-tag not known, must be either node or edge")
                    
            elif child.tag == (nsprefix+'graph'):
                # start parsing the graph into networkx data structure
                # create graph depending on (either AttrGraph or AttrDiGraph)
                #   directionality: undirected/directed
                #   version of networkx:
                #   contains self-loops
                #   edges have dicts
                #   data per graph/node/edge
                for attr, value in child.items():
                    if attr == 'edgedefault' and value == 'undirected':
                        defaultDirected[0] = False
                    elif attr == 'id':
                        graphid = value
                
                if defaultDirected[0]:
                    G = nx.DiGraph()
                else:
                    G = nx.Graph()
    
                # add id, nodekeys and edkeys as traits               
                self.networkid = graphid
                self.nodekeys = nodekeys
                self.edgekeys = edgekeys
    
                # iterate over all nodes and edges
                for children in child.iterchildren():
                    if children.tag == (nsprefix+'node'):
                        
                        # parse the node
                        for attr, value in children.items():
                            if attr == 'id':
                                # add the node with corresponding id
                                G.add_node(value)
                                # keep node id to store attributes
                                nodeid = value
                            elif attr == (nsxlink+'href'):
                                # add xlink to node dictionary
                                G.node[nodeid]['xlink'] = value
                            else:
                                # node attribute not known
                                logger.warning('The following node attribute is not known and thus discarded:'+ attr + ':' + value)
        
                        # parse node data, add to node dict
                        for data in children.iterchildren():
                            # read the keylabel, i.e. the data attribute name
                            keylabel = data.attrib['key']
                            # is the keylabel in the list of allowed keys
                            if nodekeys.has_key(keylabel):
                                if not data.text == '':
                                    # add data to the node's dict
                                    G.node[nodeid][keylabel] = data.text

                                else:
                                    # no data available, check if default value exists
                                    if nodekeys[keylabel].has_key('default'):
                                        # add default data to the node's dict
                                        G.node[nodeid][keylabel] = nodekeys[keylabel]['default']
                                        logger.debug('Added default value '+ keylabel + ':' + nodekeys[keylabel]['default'])
                                    else:
                                        logger.warning('Nor data nor default value defined for ' + keylabel)
                                        # TODO: Work with exceptions!
                            else:
                                logger.warning("Data entry with key " + keylabel + " not defined.")
        
                        
                    elif children.tag == (nsprefix+'edge'):
                        
                        # parse the edge
                        # parse its attributes
                        for attr, value in children.items():
                            if attr == 'id':
                                # no usage of edge id
                                # add the edge with corresponding id
                                src = children.attrib['source']
                                tar = children.attrib['target']
                                G.add_edge(src, tar)
                                # keep dest and tar id to store attributes
                                srcid = src
                                tarid = tar
                            elif attr == (nsxlink+'href'):
                                # add xlink to edge dictionary
                                G.edge[srcid][tarid]['xlink'] = value
        
                        # parse data, and add to the edge dict
                        for data in children.iterchildren():
                            # read the keylabel, i.e. the data attribute name
                            keylabel = data.attrib['key']
                            # is the keylabel in the list of allowed keys
                            if self.edgekeys.has_key(keylabel):
                                if not data.text == '':
                                    # add data to the edge's dict, assume float!!
                                    G.edge[srcid][tarid][keylabel] = float(data.text)
                                else:
                                    # no data available, check if default value exists
                                    G.edge[srcid][tarid][keylabel] = self._return_default_edgevalue(self.edgekeys, keylabel)
                        data_keys = G.edge[srcid][tarid].keys()
                        # check if we missed some edge keys that are available in the header
                        for k, v in self.edgekeys.items():
                            if not k in data_keys:
                                G.edge[srcid][tarid][k] = self._return_default_edgevalue(self.edgekeys, k)
        
        # return the generated network graph
        return G


#### EOF ######################################################################
