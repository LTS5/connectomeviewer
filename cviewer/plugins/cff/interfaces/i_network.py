""" The interface for Connectome Networks. """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports.
from enthought.traits.api import Interface, Str, Bool, CBool, Any, Dict, List, \
                                    DelegatesTo

# local interfaces
from i_surface import ISurface
from i_volume import IVolume
from i_trackfile import ITrackfile
from i_surface_container import ISurfaceContainer

class INetwork(Interface):
    """ The interface for Connectome Networks. """

    # Network ID, from parsed GraphML
    networkid = Str

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
    nodekeys = Dict
    
    # Edgekeys, from parsed GraphML
    edgekeys = Dict

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
    rendermanager = Any
    
    # DatasourceManager Instance of this network
    datasourcemanager = Any
    
    # private traits
    ###########
    
    # parent cfile this networks belongs to
    _parentcfile = Any

    # filezip of cfile
    _filezip = DelegatesTo('_parentcfile')

    def __init__(self, name, src = None, directed = '0', pickled_graph = None, \
                 hierarchical ='0', hypergraph = '0'):
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

    def _de_activate(self):
        """ Toggles the internal state of the activation """

    def _create_renderer(self):
        """ Creates the renderer instance if not yet available
        and opens the scenes in mayavi """

    def _render_matrix(self):
        """ Invokes the rendering of the Connectivity Matrix """

    def _trackvis_launch(self):
        """ Generates scene file and launch Trackvis on the selected nodes """
    

    def add_surface_container(self, surfacecontainer):
        """ Add a surface container to the loaded list
        
        Parameters
        ----------
        surfacecontainer : `ISurfaceContainer` instance
            a surface container object
        
        """

    def add_volume(self, volume):
        """ Adds a volume to the volumes list
        
        Parameters
        ----------
        volume : `IVolume` instance
            a volume object
        
        """
        
    def add_trackfile(self, trackfile):
        """ Adds a trackfile to the tracks list
        
        Parameters
        ----------
        trackfile : `ITrackfile` instance
            a trackfile of type ITrackfile
        
        """

    def unselect_all(self):
        """ Unselects every node in the current network """

    def set_selectiongraph(self, sellist, value):
        """ Sets the selected nodes the network to value.
        
        Parameters
        ----------
        sellist : array_like
            a list of nodeids conforming to the NetworkX name
        value : integer
            the value to set the nodes
        
        """

    def get_selectiongraph_list(self):
        """ Returns a list of the node ids that were selected in
        the rendered scene.
        
        """        

    def set_weight_key(self, weight_key = None):
        """ Sets the weight key in the graph representation of the network.
        
        Parameters
        ----------
        weight_key : Str
            Must be a possible existing edge key
            
        """

    def get_matrix(self, weight = None):
        """ Returns the connectivity matrix of the network with the nodes
        ordered according to their id in the GraphML file.
        
        Parameters
        ----------
        weight: Str
            Possible key value of the edges
        
        Returns
        -------
        matrix : `Numpy.array` instance
            The connectivity matrix
        
        """

    def toggle_surface(self):
        """ Toggle the surface for the selected network nodes """
        
    def show_surface(self):
        """ Shows the surface for the selected network nodes """
        
    def load_pickled_graphml(self, graph):
        """ Loads a pickled GraphML file
        
        Parameters
        ----------
        graph : NetworkX Graph instance
            A graph instance
            
        """
    
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

#### EOF ######################################################################
