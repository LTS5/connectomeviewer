""" Handling the data sources for a network in a Connectome File

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
import networkx as nx
import numpy as np
import time

# Enthought library imports
from enthought.traits.api import HasTraits, Instance, Any, Int, Array, NO_COMPARE, Either, Bool, Str

# ConnectomeViewer imports
from cviewer.plugins.cff.interfaces.i_network import INetwork

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

ArrayOrNone = Either(None, Array, comparison_mode=NO_COMPARE)

class SourceObject(HasTraits):
    """ A little wrapper class for the source object variables """

    def __init__(self, **traits):
        super(SourceObject, self).__init__(**traits)

    def reset(self, **traits):
        """Creates the dataset afresh or resets existing data source."""
        self.set(trait_change_notify=False, **traits)

    # node enumerated, defining the order of the nodes
    # starting with 0
    # one-subtracted integer as compared to the GraphML node id
    nodeids = Any

    # colors per nodeid (as numpy matrix with RGB values)
    colors = Any
    
    # currently picked nodes as array with length of number of nodes
    # 0 -> not picked
    # 1 -> picked
    selected_nodes = Any
    
    # list of node labels in order with nodeids
    labels = Any

    # is the graph directed?
    directed = Any

    # dictionary with current attributes as key and numpy array as values
    scalarsdict = Any

    # dictionary for all edge attributes and the corresponding numpy matrix
    # ordered by stripn'ed nodes
    edgeattributes_matrix_dict = Any
    
    # array of connecting edges
    edges = ArrayOrNone
    
    # currently selected edge attribute
    # this can be changed by matrixviewer / edge param vis / scene2d
    # XXX: currently, scene2d assumes that there exists a set weight key in the graph
    #current_edge_attribute = Str
    
    # array of selected edges
    selected_edges = ArrayOrNone
    
    # positions of the nodes
    positions = ArrayOrNone
    
    # needed for the actual vectors displayed (vectors are zero when not selected!)
    start_positions = ArrayOrNone
    vectors = ArrayOrNone
    
    # needed as an original source for the vectors length
    vectors_orig = ArrayOrNone
    
    # reference to relabled graph according to stripn
    relabled_graph = Any
    
    # current surface vertices
    daV = ArrayOrNone
    
    # current surface faces
    daF = ArrayOrNone
    
    # current surface label array containing intensity values from the nodes
    labelarr = ArrayOrNone
    
    # the orginial label array (used to filter for selecting patches)
    labelarr_orig = ArrayOrNone
    
    # surfaces display activated
    show_surface = False
    
    # used surface container for rendering
    _usedsurfcont = Int(-1)
    
    def __init__(self, **traits):
        super(SourceObject, self).__init__(**traits)

    
class DatasourceManager(HasTraits):
    """ Manages data from the cfile interface to visualize it appropriately
    with Mayavi """
    
    # reference to the current network object
    _networkdata = Instance(INetwork)
    
    # all the data computed for the network views
    _srcobj = Instance(SourceObject)
        
    # reference to the graph managed
    _graph = Instance(nx.Graph)
    
    def __init__(self, network, **traits):
        """ Initializes the DataSource Manager
        
        Parameters
        ----------
        network : `INetwork` instance
            Storing the reference to the network object
        
        """
        super(DatasourceManager, self).__init__(**traits)
        
        # a reference to the actual network instance this DatasourceManager manages
        self._networkdata = network
        
        # extract from the network the graph
        self._graph = network.graph
        
        # creating the sourceobject
        self._srcobj = SourceObject()
        
        # setting the surface container to not set
        self._srcobj._usedsurfcont = -1
        
        # init computation of the source object
        self._init_compute_sourceobject()
        
    def get_sourceobject(self):
        """ Returns the internal sourceobject """
        return self._srcobj

    def update_positions(self, positions):
        """ Updates the node positions in the visualization
        
        Parameters
        ----------
        positions : ndarray Nx3
        
        Returns
        -------
        success : Bool
        """
        
        # positions array has to have the correct dimension
        assert self._srcobj.positions.shape == positions.shape
        
        # update positions as given
        self._srcobj.positions = positions
        
        # update data for vectors
        start_positions = positions[self._srcobj.edges[:, 0], :].T
        end_positions = positions[self._srcobj.edges[:, 1], :].T
        vectors = end_positions - start_positions
            
        # update the data
        self._srcobj.reset(positions = positions,
                          start_positions = start_positions, \
                          vectors_orig = vectors) 

        # update _srcobj.vectors (depending on which nodes are selected)
        self._update_vector()
        
        # who should do the actual update of the visualization?
        # in render manager or datasource manager
        
        return True    
    
    def _update_labelarr(self, intensityvalue_array = None):
        """ Updates the label array for the surface  """
        from numpy import zeros, where
        
        if not self._srcobj.show_surface:
            # hide the surfaces            
            # absolute value for every label array element
            from numpy import zeros
            self._srcobj.labelarr = zeros(self._srcobj.labelarr.shape)
            
        else:
            # make surfaces visible
            larr_orig = self._srcobj.labelarr_orig
            self._srcobj.labelarr = zeros(larr_orig.shape)
            
            if not intensityvalue_array is None:
                for iv in intensityvalue_array:
                    # make the selected nodes/surfaces positive
                    self._srcobj.labelarr[where(larr_orig==iv)] = iv
            
        # postcondition
        assert self._srcobj.labelarr.shape == self._srcobj.labelarr_orig.shape

    def _init_compute_sourceobject(self):
        """ Initial computation for the sourceobject """
        
        logger.debug('Computing initial SourceObject...')
        
        # compute nodeids, labels, colors, directed       
        tic = time.time()
        self._compute_graphnode_characteristics()
        toc = time.time()
        logger.debug('Computing node data. Time: %s' % str(toc-tic))
        
        # compute scalarsdict, edges
        tic = time.time()
        self._compute_graphedge_characteristics()
        toc = time.time()
        logger.debug('Computing edge data. Time: %s' % str(toc-tic))
        
        # positions, start_positions, vectors computed when layouting invoked
        
        logger.debug('SourceObject computed. Time: %s' %str(toc-tic))

    def _compute_graphnode_characteristics(self):
        """ Computes the fixed graph node characteristics """
        import networkx as nx
        import numpy as np
        
        # use the internal graph
        G = self._graph
        
        # relabel graph
        H = nx.relabel_nodes(G, self._stripn)
        # store the relabeled graph for later manipulation
        self._srcobj.relabled_graph = H
        
        # node as ints
        nodeids = H.nodes()
        
        # the ONE array
        selected_nodes = np.zeros(len(nodeids))
        
        labels = []
        rgbs = np.zeros((len(nodeids), 4))
        
        for i in range(len(G.nodes())):
            # to initialize the string list
            # possibly there is a nicer solution, feel free to change :)
            labels.append('')
            
        for tmpnode in G.nodes_iter(data=True):
            # each node is a tuple
            # tmpnode[0] -> the node id
            # tmpnode[1] -> the node dict
            # store the abbreviation appropriately
            
            indices = self._stripn(tmpnode[0])

            if tmpnode[1] is None:
                logger.debug('The node %s has no information. Please check the GraphML file!' % str(tmpnode[0]))
                continue

            if tmpnode[1].has_key('dn_label'):
                labels[ indices ] = tmpnode[1]['dn_label']
            
            def extract_colorarray(str_code, alpha = '255'):
                """ Parses the coma-separated colorcode into an array """
                # FIXME: to check if in range [0,255] or float [0.,1.]
                from numpy import array
                str_code = str(str_code)
                if str_code.count(',') == 2 or str_code.count(',') == 3:
                    cc = str_code
                    cc = cc.replace(' ', '')
                    rgblist = cc.split(',')
                    rgblist = rgblist + [alpha]
                    # convert strings to integers
                    # FIXME: potentially this could be floats?
                    rgblist = [int(v) for v in rgblist]
                    return np.array(rgblist)
                else:
                    logger.error('A none valid colorcode found: %s . Use a coma-separated form: R,G,B(,A)' % str_code)
                    return array()
            
            if tmpnode[1].has_key('dn_colorcode'):
                # parse the como separated color codes into an array
                rgbs[ indices, : ] = extract_colorarray(str_code = tmpnode[1]['dn_colorcode'])
            else:
                # no colorcode existing, try to find default colorcode
                # if a default colorcode value was given, take this one
                if self._networkdata.nodekeys.has_key('dn_colorcode'):
                    if self._networkdata.nodekeys['dn_colorcode'].has_key('default'):
                        default_colorcode = self._networkdata.nodekeys['dn_colorcode']['default']
                        logger.info('Default colorcode was used for node %s. ' % tmpnode[0])
                        colorarr = extract_colorarray(str_code = default_colorcode)
                        tmpnode[1]['dn_colorcode'] = tuple(colorarr)
                        rgbs[ indices, : ] = colorarr
                else:
                    # otherwise just use a predefined one
                    logger.info('No colorcode for node %s found! Taking default RGB: (225,217,7)' % tmpnode[0])
                    colorarr = np.array([225, 217, 7, 255])
                    tmpnode[1]['dn_colorcode'] = tuple(colorarr)
                    rgbs[ indices, : ] = colorarr
                      
        self._srcobj.set (nodeids = nodeids, labels = labels, \
                                  colors = rgbs, directed = self._graph.is_directed())
        self._srcobj.reset (selected_nodes = selected_nodes)
        
        
    def _compute_graphedge_characteristics(self):
        """ Compute the graph edge characteristics including matrices """
        import networkx as nx
        
        # graph to use
        G = self._graph
        
        # if no edges are existing
        if len(G.edges()) == 0:
            self._srcobj.set(scalarsdict = {}, edges = np.array([]))
            self._srcobj.set(edgeattributes_matrix_dict = {})
            return
        
        # relabel graph
        H = self._srcobj.relabled_graph
        
        # edges as array
        edges = np.array(H.edges())
        nr_edges = len(edges)
        nr_nodes = H.number_of_nodes()
        
        # a dict collecting the numpy arrays of scalars ordered by increasing edge
        edgeattr_dict = {}
        
        # get all edges key
        # the following line does not work because it could also have colorcodes etc
        #tmpkeys = self._networkdata.edgekeys.keys()
        
        # for now, assume that the first edge contains all possible attributes
        edge_iter = G.edges_iter(data=True)
        tmpedge = edge_iter.next()
        
        # the 3rd value of the tuple is the dict
        tmpkeys = tmpedge[2].keys()
        
        # ordered dict for edge attributes
        edgeattr_dict = {}
        edgeattr_matrix_dict = {}
        
        # edgeattr_dict from keys with nr_edges-length numpy zeros array
        edgeattr_dict = edgeattr_dict.fromkeys(tmpkeys)
        edgeattr_matrix_dict = edgeattr_dict.fromkeys(tmpkeys)
        
        # important: generate on individual zeros numpy array for each key
        # and also matrix
        for key, value in edgeattr_dict.items():
            edgeattr_dict[key] = np.zeros(nr_edges)
            edgeattr_matrix_dict[key] = np.zeros( (nr_nodes, nr_nodes) )
        
        # set all the edge attributes correctly
        for i, tmpedge in enumerate(H.edges_iter()):
            # each tmpedge is a tuple, containing
            # (fromnode, tonode)

            # iterate over all attributes and set the edgeattr_dict accordingly
            for key, value in G.edge[self._istripn(tmpedge[0])][self._istripn(tmpedge[1])].items():
                edgeattr_dict[str(key)][i] = value
                if G.is_directed():
                    edgeattr_matrix_dict[str(key)][tmpedge[0]][tmpedge[1]] = value
                else:
                    edgeattr_matrix_dict[str(key)][tmpedge[0]][tmpedge[1]] = value
                    edgeattr_matrix_dict[str(key)][tmpedge[1]][tmpedge[0]] = value
                    

        self._srcobj.set(scalarsdict = edgeattr_dict, edges = edges, \
                         edgeattributes_matrix_dict = edgeattr_matrix_dict)
        
        
    def _compute_3DLayout(self, surfacecontainerid, surfaceid):
        """ Updates the source object with recomputed values for layouting
        
        Parameters
        ----------
        surfacecontainerid : int
            The list id of the surface container
        surfaceid : int
            The surface id of the surface for the given surface container
        
        """
        logger.debug('Compute 3D Layout:')
        # do the recomputation
        
        logger.debug('value of sourceobject used surfcont %s' % str(self._srcobj._usedsurfcont))
        if self._srcobj._usedsurfcont != surfacecontainerid:
            logger.debug('New SurfaceContainer (ID:%s) Recompute SourceObject...' % str(surfacecontainerid) )
            self._srcobj.set(_usedsurfcont = surfacecontainerid)
            updatelabels = True
        else:
            updatelabels = False

        logger.debug('Use SurfaceContainer (ID:%s)' % str( self._srcobj._usedsurfcont)) 
        tic = time.time()
        self._recompute_3DLayout(surfacecontainerid, surfaceid, updatelabels = updatelabels)
        toc = time.time()
        logger.debug('SourceObject recomputed. Time: %s' % str(toc-tic))

        logger.debug('Compute 3D Layout done.')
        
        return self.get_sourceobject()

    def _recompute_3DLayout(self, surfacecontainerid, surfaceid, updatelabels = False):
        """ Recomputes 3D View layout data, resetting sourceobject
        position,start_positions, vectors, daV, daF, labelarr
        
        Parameters
        ----------
        surfacecontainerid : int
            The list id of the surface container
        surfaceid : int
            The surface id of the surface for the given surface container
        updatelabels : bool
            If the labelarrays should be updated
            
        """
        
        logger.debug('Recompute 3D Layout (SurfaceContainerID: %s, SurfaceID: %s)' \
                     %(str(surfacecontainerid), str(surfaceid)) )
        
        # graph to use
        G = self._graph
        
        positions = np.zeros( (len(G.nodes()), 3) )

        tic = time.time()
        # or surfacecontainerid == -1 and surfaceid == -1
        if len(self._networkdata.surfaces) == 0:
            nosurfaces = True
            nolabels = True
        else:
            nosurfaces = False
            daV, daF, labelarr = \
                self._networkdata.surfaces[surfacecontainerid].surfaces[surfaceid].get_geometry()
            if labelarr is None:
                nolabels = True
            else:
                nolabels = False
                
            # helper array to compute centers of gravity with maximally len(daV) entries
            # tmpcoordarray = np.zeros((len(daV), 3))
        toc = time.time()
        logger.debug('Getting surface data. Time: %s' % str(toc-tic))

        tic = time.time()
        for tmpnode in G.nodes_iter(data=True):
            # each node is a tuple
            # tmpnode[0] -> the node id
            # tmpnode[1] -> the node dict
            indices = self._stripn(tmpnode[0])

            # for each node (intenstiyvalue)
            # extract all vertices from surfaceid in surfacecontainer
            # and compute the CoG, store in positions
            idx = ''
            # indices in verticesset equal to intensity value
            
            if not nolabels:

                if tmpnode[1].has_key('dn_intensityvalue'):
                    # find the indices in the labelarray for the given surface
                    myintval = int(tmpnode[1]['dn_intensityvalue'])
                    idxtuple = np.where(labelarr == myintval)
                    if len(idxtuple) >= 1:
                        idx = idxtuple[0]
                    else:
                        logger.debug('Need to check the return value of np.where again! ' + str(idxtuple))

                    if not len(idx) == 0:
                        # select only vertices with the corresponding coordinates
                        tmpcoordarray = daV[idx]
                        # compute the center of gravity of this set of coordinates
                        positions[ indices, : ] = self._compute_cog(tmpcoordarray)

                else:
                    logger.error('No Intensityvalue found for node '+str(tmpnode[0]))
                    logger.error("Can't use labelarray for layouting")

            if len(idx) == 0:
                # check if there is a dn_position data in the network
                if tmpnode[1].has_key('dn_position'):
                    tmpos = tmpnode[1]['dn_position']
                    positions[ indices, : ] = np.array(map(float,tmpos.split(',')))

            
        toc = time.time()
        logger.debug('Iterating over nodes end. Time: %s' % str(toc-tic))
        
        tic = time.time()

        # we have the original labelarr in labelarr
        # here we need to make sure that the labelarr and labelarr_orig of
        # the sourceobject are consistent with the layout of the new surface
        # i.e. the correct dimensions

        # for the first render or if no surface
        if self._srcobj.labelarr_orig is None:
            if not nosurfaces:
                logger.debug('Setting the labelarray to zero')
                self._srcobj.reset(labelarr = np.zeros(labelarr.shape), \
                                   labelarr_orig = labelarr)
        else:
            # it could be
            # - selection of another surface container surface -> update orig and labelarr
            # - selection of the same surface container surface -> everything stays the same
            if updatelabels:
                logger.debug('Updating the labelarray required.')
                self._srcobj.reset(labelarr = np.zeros(labelarr.shape), \
                               labelarr_orig = labelarr)
            
        # if no edges are in the dataset, don't set them
        if not len(self._srcobj.edges) == 0:
            logger.debug('There are edges for this network.')
            
            # compute the startpositions and vectors for the 3d view, thanks to Gael Varoquaux
            start_positions = positions[self._srcobj.edges[:, 0], :].T
            end_positions = positions[self._srcobj.edges[:, 1], :].T
            vectors = end_positions - start_positions

            # get all the selected vectors and set it to current vectors object
            # with the appropriate new components
            if self._srcobj.selected_edges is None:
                currentedges = np.zeros(self._srcobj.edges.shape[0])
                logger.debug('Current Edges setting %s' % str(currentedges.shape))
                currentvectors = np.zeros(vectors.shape)
                self._srcobj.reset(vectors = currentvectors, \
                                   selected_edges = currentedges)
                logger.debug('is it set? %s' % str(self._srcobj.selected_edges.shape))
                            
            # if just another surface, update the vectors
            self._srcobj.reset(positions = positions, start_positions = start_positions, \
                                    vectors_orig = vectors) 
            
                
        else:
            logger.debug('No edges found for the this network.')
            # otherwise, just set the values for the node/surface computation
            self._srcobj.set(positions = positions, start_positions = np.array([[0.],[0.],[0.]]), \
                                     vectors = np.array([[0.],[0.],[0.]]), vectors_orig = np.array([[0.],[0.],[0.]]))

        # if surfaces are existing
        if not nosurfaces:
            self._srcobj.reset(daV = daV, daF = daF)
            if not len(self._srcobj.edges) == 0:
                self._update_vector()
            
        toc = time.time()
        logger.debug('Resetting current source object. Time: %s' % str(toc-tic))
    
    def _update_vector(self):
        """ Update the vector source based on all selected edges.
        The client has to update the renderings.
        
        """
        # elementwise multiply vectors_orig with selected_edges
        e = self._srcobj.selected_edges
        orig = self._srcobj.vectors_orig
        
        # somehow, this assertion fails when activating a second network
        # even thought it evaluates to true
        assert orig.shape[1] == e.shape[0]
        
        self._srcobj.vectors[0,:] =  orig[0,:] * e
        self._srcobj.vectors[1,:] =  orig[1,:] * e
        self._srcobj.vectors[2,:] =  orig[2,:] * e
    
    def _stripn(self, x):
        " Strips the n of the networknode id and returns a one-subtracted integer "
        return int(x.lstrip('n'))-1
        
    def _istripn(self, i):
        " Giving an integer, return a one-added string with n as prefi "
        return 'n' + str(i + 1)
    
    def _compute_cog(self, pointset):
        """ Returns the center of gravity as array 3x1 for x,y,z
        
        Parameters
        ----------
        pointset : array_like
            The coordinates as a set of N points in 3-space (N-1)x3
        
        """
        if pointset.ndim != 2:
            return
        if pointset.shape[1] == 3:
            # a valid (N-1)x3 array
            # all x values: x[:,0]
            
            # average along the correct dimension
            return np.average(pointset, axis = 0)
    
    def get_current_edges_and_weights(self):
        """ Returns edges and currently selected weight in an array """
        
        # XXX: currently assume that there is set weight key in the graph to be used
        weights = np.zeros(self._graph.number_of_edges())
        edges = self._srcobj.edges
        
        for i,ele in enumerate(edges):
            # extract the weight value from the graph edges
            try:
                weights[i] = self._graph.edge[self._istripn(ele[0])][self._istripn(ele[1])]['weight']
            except KeyError:
                raise('No weight key is set in the graph.')
            
        return edges, weights