""" The sLOREATA Converter Plugin

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.envisage.api import Plugin
from enthought.envisage.api import Service
from enthought.traits.api import List

# This module's package.
PKG = '.'.join(__name__.split('.')[:-1])

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class SLoretaConverterPlugin(Plugin):
    """ This plugin allows to convert networks form sLORETA to connectome files. """

    ACTION_SETS = 'enthought.envisage.ui.workbench.action_sets'
    
    # The plugin's unique identifier.
    id = 'connectome.sloreta'

    # The plugin's name (suitable for displaying to the user).
    name = 'sLORETA Converter Plugin'

    # adding the action sets
    action_sets = List(contributes_to=ACTION_SETS)

    def _action_sets_default(self):
        """ Trait initializer. """
        from sloreta_action_set import SLoretaConverterActionSet
        return [SLoretaConverterActionSet]
