""" Network based statistic plugin for Connectome Viewer

Original code author:  Andrew Zalesky, azalesky@unimelb.edu.au

"""
# Copyright (C) 2011, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from envisage.api import Plugin
from traits.api import List
from envisage.api import Service

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class NBSPlugin(Plugin):
    """ The plugin exposes a GUI for the NBS in Python Connectome Toolbox """
    
    COMMANDS            = 'envisage.plugins.python_shell.commands'

    commands = List(contributes_to=COMMANDS)
    
    # The plugin's unique identifier.
    id = 'connectome.pyconto.algorithms.nbs'

    # The plugin's name (suitable for displaying to the user).
    name = 'Network Based Statistics Plugin'

    def _commands_default(self):
        """ Exposes the nbs namespace in the python shell """

        return [
            'import cviewer.libs.pyconto.groupstatistics.nbs as nbs' ]
        
