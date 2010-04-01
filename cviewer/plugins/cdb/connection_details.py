""" Wrapper for a SQL connection to a ConnectomeDatabase

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
from enthought.traits.api import HasTraits, Int, Str, Password, Directory
from enthought.traits.ui.api import View, Item

class ConnectionDetails(HasTraits):
    """ The details for the ConnectomeDB SQL connection """
        
    host = Str
    user = Str
    passwd = Password
    db = Str
    savepath = Directory
    
    def _allfull(self):
        """ Tests if all fields are not-empty """

        if len(self.host.strip()) > 0 and \
            len(self.user.strip()) > 0 and \
            len(self.passwd.strip()) > 0 and \
            len(self.db.strip()) > 0 and \
            len(self.savepath.strip()) > 0:
            return True
        return False
    
    traits_view = View(
        Item('host', label= 'Host:'),
        Item('user', label= 'Username:'),
        Item('passwd', label= 'Password:'),
        Item('db', label= 'Database:'),
        Item('savepath', label= 'Save Path:'),
        width = 0.9, 
    )
