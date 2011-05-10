""" Diffusion in Python (DiPy) Plugin for ConnectomeViewer

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.envisage.api import Plugin
from enthought.traits.api import List
from enthought.envisage.api import Service

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class DipyPlugin(Plugin):
    """ The plugin exposes the DiPy package for use in ConnectomeViewer """
    
    COMMANDS            = 'enthought.plugins.python_shell.commands'

    commands = List(contributes_to=COMMANDS)
    
    # The plugin's unique identifier.
    id = 'connectome.dipy'

    # The plugin's name (suitable for displaying to the user).
    name = 'Diffusion in Python Plugin'

    def _commands_default(self):
        """ Includes dipy in the python shell """

        return [ 'import dipy' ]
        
