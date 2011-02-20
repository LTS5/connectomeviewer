""" Action set for the Oracl plugin

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.envisage.ui.action.api import Action, Group, Menu, ToolBar
from enthought.envisage.ui.workbench.api import WorkbenchActionSet

show_surface = Action(
    id            = "OracleCSurface",
    class_name    = "cviewer.plugins.codeoracle.actions.ShowSurfaces",
    name          = "Show Surface",
    path          = "MenuBar/Code Oracle/CSurface"
)

show_volumecre = Action(
    id            = "OracleCVolumeCre",
    class_name    = "cviewer.plugins.codeoracle.actions.ShowVolumes",
    name          = "Volume Creation",
    path          = "MenuBar/Code Oracle/CVolume"
)

show_network = Action(
    id            = "OracleCNetwork",
    class_name    = "cviewer.plugins.codeoracle.actions.ShowNetworks",
    name          = "Show Network",
    path          = "MenuBar/Code Oracle/CNetwork"
)

compute_nbs = Action(
    id            = "OracleNBS",
    class_name    = "cviewer.plugins.codeoracle.actions.ComputeNBS",
    name          = "Network-based statistic (NBS)",
    path          = "MenuBar/Code Oracle/Statistics"
)


class OracleActionSet(WorkbenchActionSet):
    """ The actionset for the Oracle plugin """
    
    id = "cviewer.plugins.codeoracle.action_set"
    
    actions = [
        show_surface,
        show_network,
        compute_nbs,
        show_volumecre
    ]
