""" Action set for the Oracl plugin

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.envisage.ui.action.api import Action, Group, Menu, ToolBar
from enthought.envisage.ui.workbench.api import WorkbenchActionSet

xnat_pushpull = Action(
    id            = "OracleXNATPushPull",
    class_name    = "cviewer.plugins.codeoracle.actions.XNATPushPull",
    name          = "XNAT Push and Pull",
    path          = "MenuBar/Code Oracle/Other"
)

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
    id            = "OracleCNetwork3D",
    class_name    = "cviewer.plugins.codeoracle.actions.ShowNetworks",
    name          = "3D Network",
    path          = "MenuBar/Code Oracle/CNetwork"
)

connection_matrix = Action(
    id            = "OracleCNetworkMat",
    class_name    = "cviewer.plugins.codeoracle.actions.ConnectionMatrix",
    name          = "Connection Matrix",
    path          = "MenuBar/Code Oracle/CNetwork"
)

simple_connection_matrix = Action(
    id            = "OracleCNetworkSimpleMat",
    class_name    = "cviewer.plugins.codeoracle.actions.SimpleConnectionMatrix",
    name          = "Simple Connection Matrix",
    path          = "MenuBar/Code Oracle/CNetwork"
)

compute_nbs = Action(
    id            = "OracleNBS",
    class_name    = "cviewer.plugins.codeoracle.actions.ComputeNBS",
    name          = "Network-based statistic (NBS)",
    path          = "MenuBar/Code Oracle/Statistics"
)

show_tracks = Action(
    id            = "OracleShowTracks",
    class_name    = "cviewer.plugins.codeoracle.actions.ShowTracks",
    name          = "Tracks between regions",
    path          = "MenuBar/Code Oracle/CTrack"
)


class OracleActionSet(WorkbenchActionSet):
    """ The actionset for the Oracle plugin """
    
    id = "cviewer.plugins.codeoracle.action_set"
    
    actions = [
        show_surface,
        show_network,
        compute_nbs,
        show_volumecre,
        connection_matrix,
        simple_connection_matrix,
        show_tracks,
        xnat_pushpull
    ]
