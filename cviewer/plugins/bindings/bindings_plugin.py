"""Bindings plugin for ConnectomeViewer

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
from enthought.envisage.api import Plugin
from enthought.traits.api import List
from enthought.envisage.api import Service

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class BindingsPlugin(Plugin):
    """ A plugin that binds some names to values in the Python Shell View. """
   
    bindings = List(contributes_to='enthought.plugins.python_shell.bindings')

    # The plugin's unique identifier.
    id = 'connectome.bindings'

    # The plugin's name (suitable for displaying to the user).
    name = 'Connectome File Bindings'
    
    mycfile = Service('cviewer.plugins.cff2.cfile.CFile')

    #analyze_node = Service('cviewer.plugins.analysis.analysis_node.AnalysisNode')

    def _bindings_default(self):
        """ Trait initializer. """
               
        bindings = {'cfile' : self.mycfile } #, 'analyze_node' : self.analyze_node}

        return [bindings]
        
    
    def stop(self):
        """ When the plugins stops, clear all remaining volume and trackfiles """

        # empty everything
        self.mycfile.close_cfile()
                    