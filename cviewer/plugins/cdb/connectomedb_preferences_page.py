""" The preferences for the ConnectomeDatabase plugin

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

# Enthought library imports.
from enthought.preferences.ui.api import PreferencesPage
from enthought.preferences.api import PreferencesHelper
from enthought.traits.api import Str, Password, Directory
from enthought.traits.ui.api import View, Group, Item

class ConnectomeDBPreferencesPage(PreferencesPage):
    """ The preferences page for the ConnectomeDatabase Plugin. """

    #### 'PreferencesPage' interface ##########################################

    # The page's category (e.g. 'General/Appearance'). The empty string means
    # that this is a top-level page.
    category = ''

    # The page name (this is what is shown in the preferences dialog.
    name = 'ConnectomeDatabase'

    # The path to the preference node that contains the preferences.
    preferences_path = 'cviewer.plugins.cdb'

    #### Preferences ##########################################################
    
    host = Str(desc='host address or IP to use for the database')
    user = Str(desc='username to login')
    passwd = Password(desc='password to login')
    db = Str(desc='the name of the database to use')
    savepath = Directory(desc='where to store the downloaded files')
    
    traits_view = View(
        Item('host', label= 'Host:'),
        Item('user', label= 'Username:'),
        Item('passwd', label= 'Password:'),
        Item('db', label= 'Database:'),
        Item('savepath', label= 'Save Path:'),
        resizable = True
    )
    
class ConnectomeDBPreferencesHelper(PreferencesHelper):
    """ Helper for the ConnectomeDatabase preferences """
    
    # The preferences path for which we use.
    preferences_path = 'cviewer.plugins.cdb'
  
    host = Str(desc='host address or IP to use for the database')
    user = Str(desc='username to login')
    passwd = Password(desc='password to login')
    db = Str(desc='the name of the database to use')
    savepath = Directory(desc='where to store the downloaded files')
    
    traits_view = View(
        Item('host', label= 'Host:'),
        Item('user', label= 'Username:'),
        Item('passwd', label= 'Password:'),
        Item('db', label= 'Database:'),
        Item('savepath', label= 'Save Path:'),
        resizable = True,
        
    )
    

