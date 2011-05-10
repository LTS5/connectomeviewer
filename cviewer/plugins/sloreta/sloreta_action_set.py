""" Action set for the sLORETA Converter plugin

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

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
