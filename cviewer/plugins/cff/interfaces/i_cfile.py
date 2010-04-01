""" The interface for Connectome Files. """
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

# Enthought library imports.
from enthought.traits.api import Interface, Str, Button, File, Instance, \
                                    List, Any, Bool

from i_meta import IMeta
from i_network import INetwork

class ICFile(Interface):
    """ The interface for Connectome Files.
    
    This class basically allows you to create an instance for a
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
    # name = Str('Connectome File')
    
    ####
    
    # zipfile
    _filezip = Any
    
    # data loaded
    _data_loaded = Bool
    
    # reference to the current window for statusbar updates
    # not nice MVC-style
    _workbenchwin = Instance('enthought.pyface.workbench.api.WorkbenchWindow')
    
    def close_cfile(self, close_scenes=False):
        """ Closes the loaded data of the cfile including the temporary files
        
        Parameters
        ----------
        close_scenes : bool
            Trying to close the scenes as well?
      
        """

    def load_cfile(self, filepath):
        """ Load a given cfile as path and initializes the attributes """
        
    def open_parse_meta_xml(self, metaxmlcontent = None):
        """ Open the meta.xml from the zip archive given its full pathname
            and create an instance of metadata
        
        Parameters
        ----------
        metaxmlcontent : string
            Metadata meta.xml from a cfile
            
        """
        
    def remove_network(self, networkid):
        """ Removes the i'th network from the cfile """
        
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

        
    def add_network_from_matrix(self, name, matrix, like_network, metadata = None, \
                                directed = False, hierarchical = False, hypergraph = False):
        """ Add a network to the cfile based on the given connectivity matrix.
        
        Parameters
        ----------
        name: String
            Name of the newly added network
        matrix : NxN array
            A NxN Numpy array with the values for the edges. N must be the same
            as len(like_network.graph.nodes()). The first row of the matrix
            corresponds to the `NetworkX` node with id 'n1'
        like_network: `INetwork` object
            Used to copy graph node information and surface containers
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
        
#### EOF ######################################################################
