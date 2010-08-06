""" Network based statistic plugin for ConnectomeViewer

Original code author:  Andrew Zalesky, azalesky@unimelb.edu.au

"""
# Copyright (C) 2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
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
from enthought.envisage.api import Plugin
from enthought.traits.api import List
from enthought.envisage.api import Service

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class NBSPlugin(Plugin):
    """ The plugin exposes a GUI for the NBS in Python Connectome Toolbox """
    
    COMMANDS            = 'enthought.plugins.python_shell.commands'

    commands = List(contributes_to=COMMANDS)
    
    # The plugin's unique identifier.
    id = 'connectome.pyconto.algorithms.nbs'

    # The plugin's name (suitable for displaying to the user).
    name = 'Network Based Statistics Plugin'

    def _commands_default(self):
        """ Exposes the nbs namespace in the python shell """

        return [
            'import cviewer.libs.pyconto.algorithms.statistics.nbs as nbs' ]
        
