"""This node contains the currently calculated global network metrics
for the selected network

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

# Enthought library imports
from enthought.traits.api import HasTraits, Any, Instance, Enum, Float, Int, \
                                Button, List, Str, on_trait_change, Bool, Property
from enthought.traits.ui.api import Item, Group, View, EnumEditor

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class AnalysisNode(HasTraits):
    """A container for analysis related options """
    
    # ConnectomeViewer imports
    from cviewer.plugins.cff.cfile import CFile
    
    # reference to the cfile
    cfile = Instance(CFile)
    
    # networks
    networks = List([])
    network = Str
    
    # selection subgraph of network
    selection = Enum('Whole network', 'Selected subgraph')
    
    # modality for edges
    modality = List([])
    sel_mod = Str

    # calculate global network metrics button
    calc_global_nx = Button('Calculate global metrics')    
    # update the the nodes and edges
    doupdate_values = Button('Update the values')
    
    # currently selected networkx graph
    currentGraph = Any
    # current graph weighted with selected modality
    currentGraphWeighted = Any
    
    # Plots
    # ---
    # degree distribution histogramm
    # weighted degree distribution histogramm
    
    # Global network metrics
    # ---
    # number of nodes
    nr_nodes = Int
    # number of edges
    nr_edges = Int
    # number of components
    nr_components = Int
    # directed
    directed = Bool
    
    # network diameter: nx.diameter(G)
    nx_diameter = Float
    # network radius: nx.radius(G)
    nx_radius = Float
    # network density: nx.density(G)
    nx_density = Float
    # average shortest path length
    nx_avgpath = Float
    # average shortest path length weighted
    nx_avgpath_w = Float
    # average clustering
    nx_avgclust = Float
    
    # is it scale-free? give parameters, probability
    # is it small-world? give parameters, probability
    
    ###########################################################################
    # `object` interface.
    ###########################################################################    
    def __init__(self, **traits):
        super(AnalysisNode, self).__init__(**traits)

    ###########################################################################
    # `HasTraits` interface.
    ###########################################################################
    
    def _update_node(self, value):
        """ Updates fields """
        
        netwlist = []
        for netw in self.cfile.networks:
            netwlist.append(netw.networkname)
            
        # set the current list of networks
        self.networks = netwlist
        
        # update modality
        self._network_changed()

    def _update_data(self):
        """ Updates data for the particular network

        """
        
        if not len(self.cfile.networks) == 0:
            if self.network in self.networks:
                # the index of the choosen network
                idx = self.networks.index(self.network)

        import networkx as nx
        
        # extract the graph
        # - correct network: idx
        cur_network = self.cfile.networks[idx]
        
        # - whole / selection graph?
        if self.selection == 'Whole network':
            self.currentGraph = cur_network.graph
        else:
            # create subgraph from the selected nodes list
            sel_nodes = cur_network.get_selectiongraph_list()
            self.currentGraph = cur_network.graph.subgraph(nbunch = sel_nodes)
            
        # which edge key for data?
        # depending on if it is directed
        self.directed = cur_network.directed
        self.nr_nodes = nx.number_of_nodes(self.currentGraph)
        self.nr_edges = nx.number_of_edges(self.currentGraph)
        
        if not self.directed:
            self.nr_components = nx.number_connected_components(self.currentGraph)
            
    def _sel_mod_changed(self, value):
        """ If the modality was changed """
        # something like defining which weights to take!
        # iterate over nodes
        
        import networkx as nx

        if self.directed:
            self.currentGraphWeighted = nx.DiGraph()
        else:
            self.currentGraphWeighted = nx.Graph()
            
        self.currentGraphWeighted.add_nodes_from(self.currentGraph.nodes())        
        # iterate over edges and store weight attribute
        # corresponding to selected modality
        # (small overhead when iterating over undirected graph)
        for n,nbrs in self.currentGraph.adjacency_iter():
            for nbr,eattr in nbrs.iteritems():
                data=eattr[self.sel_mod]
                # TODO: KeyError when changing? are edgekeys corresponding?
                self.currentGraphWeighted.add_edges_from([(n,nbr)], weight=data)


    def _selection_changed(self):
        """ If the selection changed """
        self._update_data()

    def _network_changed(self):
        """ If a network is selected, update the edgekeys """
        
        # get position of selected network
        if not len(self.cfile.networks) == 0:
            if self.network in self.networks:
                idx = self.networks.index(self.network)
                self.modality = self.cfile.networks[idx].edgekeys.keys()
                self._update_data()
        else:
            self.modality = []
    
    def _cfile_changed(self, value):
        """ If a connectome file is assigned """
        # SETUP the handler if _data_load changes!
        self.cfile.on_trait_change(self._update_node, '_data_loaded')
    
    def _doupdate_values_changed(self, value):
        """ Update the values """
        self._update_data()
    
    def _calc_global_nx_changed(self, value):
        """ Global measure calculation button pressed """
        
        import networkx as nx
        # do we need to calculate them in a thread?
        # http://networkx.lanl.gov/reference/algorithms.traversal.html
        self.nx_diameter = nx.diameter(self.currentGraph)
        self.nx_radius = nx.radius(self.currentGraph)
        self.nx_density = nx.density(self.currentGraph)
        self.nx_avgpath = nx.average_shortest_path_length(self.currentGraph)
        # use the weighted graph
        self.nx_avgpath_w = nx.average_shortest_path_length(self.currentGraphWeighted, weighted=True)
        self.nx_avgclust = nx.average_clustering(self.currentGraph)
    
    def default_traits_view(self):
        """ The Connectome File Tree View
        """
       
        view = View(Group(
                        Item( 'network', editor=EnumEditor(name='networks')),
                        Item( '_' ),
                        Item( 'selection', style = 'simple',  label = 'Subnetwork:' ),
                        Item( '_' ),
                        Item( 'sel_mod',  editor=EnumEditor(name='modality'), label = 'Modality:' ), 
                        
                        Item( 'doupdate_values', style = 'simple', show_label=False),
                        Item( 'calc_global_nx', style = 'simple', show_label=False, enabled_when = 'sel_mod != "" and nr_components == 1'),
                        Item( 'nr_nodes', label = '# Nodes:', style = 'readonly'),
                        Item( 'nr_edges', label = '# Edges:', style = 'readonly'),
                        Item( 'nr_components', label = '# Components:', style = 'readonly', visible_when = 'directed == False'),
                        Item( 'directed', label = 'Directed?', style = 'custom'),
                        Item( '_' ),
                        Item( 'nx_diameter', label = 'Diameter:', style = 'readonly'),
                        Item( 'nx_radius', label = 'Radius:', style = 'readonly'),
                        Item( 'nx_density', label = 'Density:', style = 'readonly'),
                        Item( 'nx_avgpath', label = 'Avg.Shortest Path:', style = 'readonly'),
                        Item( 'nx_avgpath_w', label = 'Weighted Avg. Shortest Path:', style = 'readonly'),
                        Item( 'nx_avgclust', label = 'Average Clustering:', style = 'readonly'),
                       
                    ),
                    id= 'connectome.cviewer.analysis.analysis_view',
                    help=False,
                    resizable=True,
                    scrollable=True,
                    ok=False,
                    cancel=False,
                    title = 'Analysis View')
        
        return view