""" Connectome Mapper Plugin for Connectome Viewer

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

CMPVIEW = 'cviewer.plugins.cmp.cmpview'

class CMPPlugin(Plugin):
    """ The plugin integrates the Connectome Mapper as a View """

    VIEWS = "enthought.envisage.ui.workbench.views"

    # The plugin's unique identifier.
    id = 'connectome.mapper'

    # Contributions to the views extension point made by this plug-in.
    views = List(contributes_to=VIEWS)

    # The plugin's name (suitable for displaying to the user).
    name = 'Connectome Mapper Plugin'

    def _views_default(self):
        """ Trait initialiser.
        """
        return [self._cmp_factory]

    def _cmp_factory(self, window, **traits):
        """ Factory method for the mapper """

        from enthought.pyface.workbench.traits_ui_view import TraitsUIView

        try:
            from cmp.gui import CMPGUI
            mycmp = CMPGUI()
        except:
            return
        
        tui_engine_view = TraitsUIView(obj=mycmp,
                                       id=CMPVIEW,
                                       name='Connectome Mapper',
                                       window=window,
                                       position='right',
                                       #relative_to=ENGINE_VIEW,
                                       **traits
                                       )
        return tui_engine_view