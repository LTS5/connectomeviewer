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

from cnetwork import CNetwork


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

    
    # standard name
    name = Str('Connectome File')


    # reference to the current window for statusbar updates
    # not nice MVC-style
    _workbenchwin = Instance('enthought.pyface.workbench.api.WorkbenchWindow')
        
    ######################################################################
    # `object` interface.
    ######################################################################
    
    def __init__(self, **traits):
        super(CFile, self).__init__(**traits)
        
        logger.info("Init CFF2 file")