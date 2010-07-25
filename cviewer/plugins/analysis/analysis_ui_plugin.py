""" The ConntecomeAnalysis UI Plugin """
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
from enthought.envisage.api import Plugin, ServiceOffer
from enthought.traits.api import List, on_trait_change
from enthought.pyface.workbench.api import Perspective, PerspectiveItem
from enthought.etsconfig.api import ETSConfig

# View IDs
SHELL_VIEW = 'enthought.plugins.python_shell_view'
#PLOTVIEW1 = 'connectome.cviewer.analysis.plot_view'
ANALYSIS_VIEW = 'connectome.cviewer.analysis.analysis_view'
ID = 'connectome.cviewer.analysis.ui'

# This module's package.
PKG = '.'.join(__name__.split('.')[:-1])

###############################################################################
# AnalysisPerspective
###############################################################################

class AnalysisPerspective(Perspective):
    """ The network analysis perspective for the ConnectomeViewer """

    # the unique ID
    id = "cviewer.perspective.analysis_perspective"

    # The perspective's name.
    name = 'AnalysisPerspective'

    # Should this perspective be enabled or not?
    enabled = True

    # Should the editor area be shown in this perspective?
    show_editor_area = True

    # The contents of the perspective.
    contents = List()

    def _contents_default(self):

        # View IDs.extended to all the views one wants to have! but what id?!
        # is it ID from: Plugin-ID?, Application-ID? or TraitsUIView-ID? Test!
        # TraitsUI does not work, Plugin-ID does work!
        # Application-ID does also work 'ch.connectome.viewer'
        # but still problem adding the view!
        #CFILE_VIEW = 'connectome.cfile'
    
        # The contents of the perspective. Add the views here with position
        contents = [
            PerspectiveItem(id=SHELL_VIEW, position='bottom', height=0.2),
            PerspectiveItem(id=ANALYSIS_VIEW, position='left'),
            PerspectiveItem(id="enthought.plugins.ipython_shell.namespace_view",
                            position="right", width=0.2)
            #PerspectiveItem(id=PLOTVIEW1, position='left'),
            
            #PerspectiveItem(id=LOGGER_VIEW, position='with', 
            #                               relative_to=SHELL_VIEW),
            #PerspectiveItem(id=ENGINE_VIEW),
            #PerspectiveItem(id=CURRENT_SELECTION_VIEW, position='bottom',
             #               relative_to=ENGINE_VIEW),
        ]
     
        return contents


###############################################################################
# `AnalysisUIPlugin` class.
###############################################################################

class AnalysisUIPlugin(Plugin):
    """ A ConnectomeViewer analysis interface plugin.

    This plugin contributes the actions, menues, preference pages etc.

    """

    # extension points this plugin contributes to
    PERSPECTIVES        = 'enthought.envisage.ui.workbench.perspectives'
    ACTION_SETS         = 'enthought.envisage.ui.workbench.action_sets'
    COMMANDS            = 'enthought.plugins.python_shell.commands'
    VIEWS               = "enthought.envisage.ui.workbench.views"
    SERVICE_OFFERS = 'enthought.envisage.service_offers'
    
    # The plugin's unique identifier.
    id = ID

    # The plugin's name (suitable for displaying to the user).
    name = 'ConnectomeAnalysis UI'
    
    # Contributions to the views extension point made by this plug-in.
    views = List(contributes_to=VIEWS)
    
    # Perspectives.
    perspectives = List(contributes_to=PERSPECTIVES)
    
    # actions
    action_sets = List(contributes_to=ACTION_SETS)
    
    # more imports needed
    commands = List(contributes_to=COMMANDS)
    
    # Services we contribute: an analyisnode
    service_offers = List(contributes_to=SERVICE_OFFERS)
    
    #####################################################################
    # Private methods.

    def _service_offers_default(self):
        """ Trait initializer. """
        cfile_service_offer = ServiceOffer(
            protocol = 'cviewer.plugins.analysis.analysis_node.AnalysisNode',
            factory = 'cviewer.plugins.analysis.analysis_node.AnalysisNode',
        )
        
        return [cfile_service_offer]

    def _commands_default(self):
        """ Initializes some imports for the ipython shell """
        return['import pylab']

    def _views_default(self):
        """ Trait initialiser.
        """
        # TODO: Texteditor, Plots, Network Traits View
        # self._plot_view_factory, 
        return [self._analysis_view_factory]

    def _perspectives_default(self):
        """ Trait initializer. """
        return [AnalysisPerspective]

    def _action_sets_default(self):
        """ Trait initializer. """
        # from cviewer_ui_action_set import CViewerUIActionSet
        return []


    ######################################################################
    # Private methods

    def _analysis_view_factory(self, window, **traits):
        """ Factory method for engine views. """
        from enthought.pyface.workbench.traits_ui_view import \
                TraitsUIView
        
        # from analysis_view import AnalysisNode
        analysis_view_node = self.application.get_service('cviewer.plugins.analysis.analysis_node.AnalysisNode')
        cfileref = self.application.get_service('cviewer.plugins.cff.cfile.CFile')      
        analysis_view_node.set(cfile = cfileref)
        
        tui_engine_view = TraitsUIView(obj=analysis_view_node,
                                       id=ANALYSIS_VIEW,
                                       name='Analysis View',
                                       window=window,
                                       position='left',
                                       **traits
                                       )
        return tui_engine_view
    

