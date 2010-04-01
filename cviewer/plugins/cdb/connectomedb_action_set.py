""" Action set for the ConnectomeDatabase plugin

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
from enthought.envisage.ui.action.api import Action, Group, Menu, ToolBar
from enthought.envisage.ui.workbench.api import WorkbenchActionSet

database_interface = Action(
    id            = "DatabaseInterface",
    class_name    = "cviewer.plugins.cdb.connectomedb_actions.StartDownloadInterface",
    name          = "ConnectomeDatabase",
    path          = "MenuBar/PluginsMenu/Database"
)

class ConnectomeDBActionSet(WorkbenchActionSet):
    """ The actionset for the ConnectomeDB plugin """
    
    id = "cviewer.plugins.cdb.action_set"
    
    actions = [
        database_interface,
    ]
