""" A wrapper to get the needed preferences application wide """
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
from os.path import join
import pkg_resources

# Enthought library imports
from enthought.etsconfig.api import ETSConfig
from enthought.traits.api import Instance
from enthought.traits.ui.api import View, Group, Item
from enthought.preferences.api import (ScopedPreferences, IPreferences,
        PreferencesHelper)

# Mayavi2 imports
from enthought.mayavi.preferences.preference_manager import PreferenceManager

# ConnectomeViewer imports
from cviewer.plugins.ui.cviewer_ui_preferences_helper import CViewerUIPreferencesHelper

# Logging import
import logging
logger = logging.getLogger('root.'+__name__)

# This module's package.
PKG = '.'.join(__name__.split('.')[:-1])

# The application ID where the preferences are stored.
ID = 'ch.connectome.viewer'

################################################################################
# `PreferenceManager` class
################################################################################
class CViewerPreferenceManager(PreferenceManager):
    
    # add the cviewer ui preferences
    cviewerui = Instance(PreferencesHelper)
        
    # The preferences.
    preferences = Instance(IPreferences)

    ######################################################################
    # Traits UI view.

    traits_view = View(Group(
                           Group(Item(name='root', style='custom'),
                                 show_labels=False, label='Root',
                                 show_border=True
                                ),
                           Group(Item(name='mlab', style='custom'),
                                 show_labels=False, label='Mlab',
                                 show_border=True,
                                ),
                           Group(Item(name='cviewerui', style='custom'),
                                 show_labels=False, label='ConnectomeViewer',
                                 show_border=True
                                )
                           ),
                       buttons=['OK', 'Cancel'],
                       resizable=True
                      )

    ######################################################################
    # `HasTraits` interface.
    ######################################################################
    def __init__(self, **traits):
        super(PreferenceManager, self).__init__(**traits)

        if 'preferences' not in traits:
            self._load_preferences()
            
    def _preferences_default(self):
        return ScopedPreferences()

    def _cviewerui_default(self):
        """Trait initializer."""
        return CViewerUIPreferencesHelper(preferences=self.preferences)
        
    def _load_preferences(self):
        """Load the default preferences."""
        # Save current application_home.
        app_home = ETSConfig.get_application_home()
        logger.debug('Application home: ' + str(app_home))
        # Set it to where the cviewer preferences are temporarily.
        path = join(ETSConfig.get_application_data(), ID)
        ETSConfig.application_home = path
        try:
            for pkg in ('cviewer.plugins.ui',
                        'enthought.mayavi.preferences',
                        'enthought.tvtk.plugins.scene'):
                pref = 'preferences.ini'
                pref_file = pkg_resources.resource_stream(pkg, pref)

                preferences = self.preferences
                default = preferences.node('default/')
                default.load(pref_file)
                pref_file.close()
        finally:
            # Set back the application home.
            ETSConfig.application_home = app_home
            
    def _preferences_changed(self, preferences):
        """Setup the helpers if the preferences trait changes."""
        for helper in (self.root, ):
            helper.preferences = preferences

##########################################################
# A Global preference manager that all other modules can use.

preference_manager = CViewerPreferenceManager()
