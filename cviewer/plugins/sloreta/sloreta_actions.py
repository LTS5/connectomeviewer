""" Actions for the sLORETA Converter

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthought library imports
from enthought.pyface.image_resource import ImageResource
from enthought.traits.api import Bool
from enthought.pyface.action.api import Action

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class StartConverterDialog(Action):
    """ Opens the sLORETA Converter dialog """

    name        = "sLORETA Converter"
    tooltip     = "sLORETA Converter Dialog"
    description = "Open the dialog of the sLORETA Converter to generate connectome files"

    # Is the action enabled?
    enabled = Bool(True)

    # Is the action visible?
    visible = Bool(True)

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event):
        """ Performs the action. """
        
        from converter import ConverterParameters
        cp = ConverterParameters()
        cp.configure_traits()
