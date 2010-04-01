""" Action set for the sLORETA Converter plugin

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

sloreta_converter = Action(
    id            = "SLoretaConverter",
    class_name    = "cviewer.plugins.sloreta.sloreta_actions.StartConverterDialog",
    name          = "sLORETA Converter",
    path          = "MenuBar/PluginsMenu/sLORETA"
)

class SLoretaConverterActionSet(WorkbenchActionSet):
    """ The actionset for the sLORETA Converter plugin """
    
    id = "cviewer.plugins.sloreta.action_set"
    
    actions = [
        sloreta_converter,
    ]
