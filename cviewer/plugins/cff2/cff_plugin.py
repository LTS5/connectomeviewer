""" Plugin to expose a connectome file and adding a view """
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Enthough library imports
from traits.api import List
from envisage.api import Plugin, ServiceOffer, ExtensionPoint

# ConnectomeViewer imports


# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

CFFVIEW = 'cviewer.plugins.cff2.ui.cff_view.CFFView'

##############################################################################
 #`ConnectomeFilePlugin` class.
##############################################################################
class ConnectomeFile2Plugin(Plugin):

    # Extension points we contribute to.
    VIEWS  = 'envisage.ui.workbench.views'    
    #- for application scope and
    SERVICE_OFFERS = 'envisage.service_offers'

    #### Extension points offered by this plugin ##############################

    CFILE = 'cviewer.plugins.cff2.cfile'

    # The connectome file extension point.
    cfiles = ExtensionPoint(
        List( ), id=CFILE, desc="""

        This extension point allows you to contribute files in the connectome
        file format, e.g. adding template atlases

        """
    )
    
    # The plugin's unique identifier.
    id = 'connectome.file'

    # The plugin's name (suitable for displaying to the user).
    name = 'Connectome File'
    
    # Services we contribute.
    service_offers = List(contributes_to=SERVICE_OFFERS)

    # Views.
    views = List(contributes_to=VIEWS)
       
    # it contributes at the same time one cfile to the extension point!
    cfile = List(contributes_to=CFILE)

    #####################################################################
    # Private methods
    #####################################################################

    def _cfile_default(self):
        """ Creates the dummy cfile contributed """
        return  self._get_cff_service()

    # a service offer looks up a corresponding
    # file from the extension point, and then can be used
    # below in the cfile_view_factory to display it.
    # ...e.g. open a cff just changes its contents.
    # and this makes also window available in the cfile for service registry lookup

    def _service_offers_default(self):
        """ Trait initializer. """
        cfile_service_offer = ServiceOffer(
            protocol = 'cviewer.plugins.cff2.cfile.CFile',
            factory = 'cviewer.plugins.cff2.cfile.CFile'
        )
        
        return [cfile_service_offer]
        
    def _get_cff_service(self):
        """ Looks up a cfile from the application service offers
        to be used by the view factory
        
        """
        return self.application.get_service('cviewer.plugins.cff2.cfile.CFile')
    

    def _views_default(self):
        """ Trait initializer. """
        return [self._cfile_view_factory]


    def _cfile_view_factory(self, window, **traits):
        """ Factory method for connectome file views. """
        from pyface.workbench.traits_ui_view import \
                TraitsUIView
        
        from cviewer.plugins.cff2.ui.cff_view import CFFView
        
        cfile = self._get_cff_service()
        cfile._workbenchwin = window
        cff_view = CFFView(cfile=cfile)
        tui_engine_view = TraitsUIView(obj=cff_view,
                                       id=CFFVIEW,
                                       name='Connectome File View',
                                       window=window,
                                       position='left',
                                       **traits
                                       )
        return tui_engine_view

######################################################################
