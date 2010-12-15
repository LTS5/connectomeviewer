""" The ConntecomeViewer UI Plugin """
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

# Partly modified version; mayavi_ui_plugin.py
# Author: Prabhu Ramachandran <prabhu [at] aero . iitb . ac . in>
# Copyright (c) 2008, Enthought, Inc.
# License: BSD Style.

# Enthought library imports
from enthought.envisage.api import Plugin
from enthought.traits.api import List, on_trait_change
from enthought.pyface.workbench.api import Perspective, PerspectiveItem
from enthought.etsconfig.api import ETSConfig

# View IDs
ENGINE_VIEW = 'enthought.mayavi.core.ui.engine_view.EngineView'
CURRENT_SELECTION_VIEW = 'enthought.mayavi.core.engine.Engine.current_selection'
SHELL_VIEW = 'enthought.plugins.python_shell_view'
LOGGER_VIEW = 'enthought.logger.plugin.view.logger_view.LoggerView' 
ID = 'connectome.cviewer.ui'
CFFVIEW = 'cviewer.plugins.cff2.ui.cff_view.CFFView'

# This module's package.
PKG = '.'.join(__name__.split('.')[:-1])

from welcome.perspective import WelcomePerspective

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

###############################################################################
# ViewerPerspective
###############################################################################

class ViewerPerspective(Perspective):
    """ The default perspective for the ConnectomeViewer """

    # the unique ID
    id = "cviewer.perspective.viewer_perspective"

    # The perspective's name.
    name = 'ViewerPerspective'

    # Should this perspective be enabled or not?
    enabled = True

    # Should the editor area be shown in this perspective?
    show_editor_area = True

    # The contents of the perspective.
    contents = List()

    def _contents_default(self):
    
        # The contents of the perspective. Add the views here with position
        contents = [
            PerspectiveItem(id=CFFVIEW, position='left', width=0.2),
            PerspectiveItem(id=SHELL_VIEW, position='bottom', height=0.2),
            # XXX: deactivate mayavi views per default as not to confuse end-users too much
            PerspectiveItem(id=ENGINE_VIEW),
            #PerspectiveItem(id=CURRENT_SELECTION_VIEW, position='bottom',
            #                relative_to=ENGINE_VIEW),
        ]
     
        return contents


###############################################################################
# `CViewerUIPlugin` class.
###############################################################################

class CViewerUIPlugin(Plugin):
    """ A Connectome Viewer user interface plugins.

    This plugin contributes the actions, menues, preference pages etc.

    """

    # extension points this plugin contributes to
    PERSPECTIVES        = 'enthought.envisage.ui.workbench.perspectives'
    PREFERENCES         = 'enthought.envisage.preferences'
    PREFERENCES_PAGES   = 'enthought.envisage.ui.workbench.preferences_pages'
    #BANNER              = 'enthought.plugins.ipython_shell.banner'
    ACTION_SETS         = 'enthought.envisage.ui.workbench.action_sets'
    #COMMANDS            = 'enthought.plugins.python_shell.commands'
    VIEWS               = "enthought.envisage.ui.workbench.views"
    
    # The plugin's unique identifier.
    id = ID

    # The plugin's name (suitable for displaying to the user).
    name = 'ConnectomeViewer UI'
    
    # Contributions to the views extension point made by this plug-in.
    views = List(contributes_to=VIEWS)
    
    # Perspectives.
    perspectives = List(contributes_to=PERSPECTIVES)
    
    # actions
    action_sets = List(contributes_to=ACTION_SETS)

    # Preferences
    preferences = List(contributes_to=PREFERENCES)

    # Preference pages
    preferences_pages = List(contributes_to=PREFERENCES_PAGES)
    
    # Ipython banner
    #banner = List(contributes_to=BANNER)
    
    # more imports needed
    #commands = List(contributes_to=COMMANDS)
    
    #####################################################################
    # Private methods.

    # TODO: currently deactivated imports for better performance
    #def _commands_default(self):
    #    """ Initializes some imports for the ipython shell """
    #    return []
        #return['import cviewer.plugins.cff.gifti as gifti']

    def _views_default(self):
        """ Trait initialiser.
        """
        return [self._engine_view_factory,
                self._current_selection_view_factory,]

    def _perspectives_default(self):
        """ Trait initializer. """
        return [ViewerPerspective]

    #def _banner_default(self):
    #    """Trait initializer """
    #    return ["Welcome! After loading Connectome File (.cff), double-click the network to activate and render it. See Help->Key Bindings for more info.", ]

    def _action_sets_default(self):
        """ Trait initializer. """
        from cviewer_ui_action_set import CViewerUIActionSet
        
        return [CViewerUIActionSet]

    def _preferences_default(self):
        """ Trait initializer. """
        return ['pkgfile://%s/preferences.ini' % PKG]
        
    def _preferences_pages_default(self):
        """ Trait initializer. """

        from cviewer_ui_preferences_page import CViewerUIPreferencesPage
        return [CViewerUIPreferencesPage]

    ######################################################################
    # Private methods. (imported from MayaviUI plugin)
    
    def _engine_view_factory(self, window, **traits):
        """ Factory method for engine views. """
        from enthought.pyface.workbench.traits_ui_view import \
                TraitsUIView
        from enthought.mayavi.core.ui.engine_view import \
                            EngineView

        engine_view = EngineView(engine=self._get_engine(window))
        tui_engine_view = TraitsUIView(obj=engine_view,
                                       id=ENGINE_VIEW,
                                       name='MayaVi Visualization Tree',
                                       window=window,
                                       position='left',
                                       **traits
                                       )
        return tui_engine_view
    
    def _current_selection_view_factory(self, window, **traits):
        """ Factory method for the current selection of the engine. """

        from enthought.pyface.workbench.traits_ui_view import \
                TraitsUIView

        engine = self._get_engine(window)
        tui_engine_view = TraitsUIView(obj=engine,
                                       view='current_selection_view',
                                       id=CURRENT_SELECTION_VIEW,
                                       name='Visualization Object Editor',
                                       window=window,
                                       position='bottom',
                                       relative_to=ENGINE_VIEW,
                                       **traits
                                       )
        return tui_engine_view

    def _get_engine(self, window):
        """Return the Mayavi engine of the particular window."""
        from enthought.mayavi.core.engine import Engine
        return window.get_service(Engine)

    def _get_script(self, window):
        """Return the `enthought.mayavi.plugins.script.Script` instance
        of the window."""
        from enthought.mayavi.plugins.script import Script
        return window.get_service(Script)

    ######################################################################
    # Trait handlers.
    @on_trait_change('application.gui:started')
    def _on_application_gui_started(self, obj, trait_name, old, new):
        """This is called when the application's GUI is started.  The
        method binds the `Script` and `Engine` instance on the
        interpreter.
        """
        # This is called when the application trait is set but we don't
        # want to do anything at that point.
        if trait_name != 'started' or not new:
            return

        # Get the script service.
        app = self.application
        window = app.workbench.active_window
        script = self._get_script(window)

        # Get a hold of the Python shell view.
        id = SHELL_VIEW
        py = window.get_view_by_id(id)
        if py is None:
            logger.warn('*'*10)
            logger.warn("Can't find the Python shell view to bind variables")
            return

        # Bind the script and engine instances to names on the
        # interpreter.
        try:
            py.bind('mayavi', script)
            py.bind('engine', script.engine)
            from enthought.naming.ui.api import explore
            py.bind('explore', explore)
        except AttributeError, msg:
            # This can happen when the shell is not visible.
            # FIXME: fix this when the shell plugin is improved.
            logger.warn(msg)
            logger.warn("Can't find the Python shell to bind variables")

