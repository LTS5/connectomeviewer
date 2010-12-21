""" Action set for the Oracl plugin

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

oracle_action = Action(
    id            = "Oracle",
    class_name    = "cviewer.plugins.codeoracle.actions.ShowSurfaces",
    name          = "Show Surfaces",
    path          = "MenuBar/PluginsMenu/Code Oracle"
)

class OracleActionSet(WorkbenchActionSet):
    """ The actionset for the Oracle plugin """
    
    id = "cviewer.plugins.codeoracle.action_set"
    
    actions = [
        oracle_action,
    ]
