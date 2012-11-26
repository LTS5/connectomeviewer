""" Helper class to load CFF files """

# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
import os.path

# Enthought library imports
from apptools.io.api import File
from traits.api import Str, Bool
from pyface.action.api import Action
from pyface.api import FileDialog, OK
from pyface.image_resource import ImageResource

# Connectome Viewer imports
from common import IMAGE_PATH
from cviewer.plugins.ui.preference_manager import preference_manager
import cfflib

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)


class OpenFile(Action):
    name        = "Open file"
    description = "Open the File Dialog where you can select a neuroimaging file"
    tooltip     = "Open file"
    path        = Str("MenuBar/File/LoadDataMenu")
    image       = ImageResource("cff-open.png", search_path=[IMAGE_PATH])

	# Is the action enabled?
    enabled = Bool(True)

	# Is the action visible?
    visible = Bool(True)

	###########################################################################
	# 'Action' interface.
	###########################################################################
	
    def perform(self, event, cfile=None):
        """ Performs the action. """

        logger.info('Performing add data file action')

        # helper variable to use this function not only in the menubar
        exec_as_funct = True

        if cfile is None:
            # get the instance of the current CFile
            # with the help of the Service Registry
            cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
            exec_as_funct = False

        wildcard = "All files (*.*)|*.*|" \
                   'All files (*)|*|' \
                   "Nifti-1 (*.nii.gz)|*.nii.gz|" \
                   "Gifti (*.gii)|*.gii|" \
                   "TrackVis tracks (*.trk)|*.trk|" \
                   "NetworkX Pickle (*.gpickle)|*.gpickle|" \
                   "NetworkX Pickle (*.pck)|*.pck|" \
                   "NetworkX GraphML (*.graphml)|*.graphml|" \
                   "Numpy Data (*.npy)|*.npy|" \
                   "Pickle Data (*.pkl)|*.pkl|" \
                   "Text Data (*.txt)|*.txt|" \
                   "CSV Data (*.csv)|*.csv"

        dlg = FileDialog(wildcard=wildcard,title="Add Neuroimaging or network data",\
                         resizeable=False, \
                         default_directory=preference_manager.cviewerui.cffpath)

        if dlg.open() == OK:

            if not os.path.isfile(dlg.path):
                logger.error("File '%s' does not exist!"%dlg.path)
                return

            logger.info('Read file: %s'%dlg.path)
            fname = os.path.split(dlg.paths[0])[-1]

            if os.path.exists(dlg.paths[0]) and (fname.endswith('.nii.gz') or
                fname.endswith('.nii')):
                cvol = cfflib.CVolume.create_from_nifti(
                            name = fname,nii_filename = dlg.paths[0])
                cfile.obj.add_connectome_volume(cvol)

            elif os.path.exists(dlg.paths[0]) and fname.endswith('.gii'):
                csurf = cfflib.CSurface.create_from_gifti(
                            name = fname, gii_filename = dlg.paths[0])
                cfile.obj.add_connectome_surface(csurf)

            elif os.path.exists(dlg.paths[0]) and (dlg.paths[0]).endswith('.trk'):
                    ctrk = cfflib.CTrack(
                                name = fname, src = dlg.paths[0])
                    cfile.obj.add_connectome_track(ctrk)

            elif os.path.exists(dlg.paths[0]) and (fname.endswith('.gpickle') or fname.endswith('.pck')):
                    cntwk = cfflib.CNetwork(name = fname, src = dlg.paths[0],
                                fileformat="NXGPickle")
                    cfile.obj.add_connectome_network(cntwk)

            elif os.path.exists(dlg.paths[0]) and fname.endswith('.graphml'):
                    cntwk = cfflib.CNetwork.create_from_graphml(name = fname,
                                ml_filename = dlg.paths[0])
                    cfile.obj.add_connectome_network(cntwk)

            elif os.path.exists(dlg.paths[0]) and fname.endswith('.npy'):
                    cdat = cfflib.CData(name = fname, src = dlg.paths[0],
                                        fileformat = 'NumPy')
                    cfile.obj.add_connectome_data(cdat)

            elif os.path.exists(dlg.paths[0]) and fname.endswith('.csv'):
                    cdat = cfflib.CData(name = fname, src = dlg.paths[0],
                                        fileformat = 'CSV')
                    cfile.obj.add_connectome_data(cdat)

            elif os.path.exists(dlg.paths[0]) and fname.endswith('.txt'):
                    cdat = cfflib.CData(name = fname, src = dlg.paths[0],
                                        fileformat = 'TXT')
                    cfile.obj.add_connectome_data(cdat)

            elif os.path.exists(dlg.paths[0]) and fname.endswith('.pkl'):
                    cdat = cfflib.CData(name = fname, src = dlg.paths[0],
                                        fileformat = 'Pickle')
                    cfile.obj.add_connectome_data(cdat)

            else:
                logger.info('Could not load file: '+ dlg.paths)

            cfile.update_children()

######################################################################
# `OpenCFFFile` class.
######################################################################
class OpenCFile(Action):
    """ An action that opens a data file depending on the supported
    extensions.  """

    name        = "Open CFF File"
    description = "Open the File Dialog where you can select a .cff or meta.cml file"
    tooltip     = "Open a CFF file"
    path        = Str("MenuBar/File/LoadDataMenu")
    image       = ImageResource("cff-open.png", search_path=[IMAGE_PATH])

    # Is the action enabled?
    enabled = Bool(True)

    # Is the action visible?
    visible = Bool(True)

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event, cfile=None):
        """ Performs the action. """
        
        logger.info('Performing open connectome file action')
        
        # helper variable to use this function not only in the menubar
        exec_as_funct = True
        
        if cfile is None:
            # get the instance of the current CFile
            # with the help of the Service Registry
            cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
            exec_as_funct = False

        wildcard = "Connectome File Format v2.0 (*.cff)|*.cff|" \
            "Connectome Markup File v2.0 (meta.cml)|meta.cml|" \
            "All files (*.*)|*.*"
            
        dlg = FileDialog(wildcard=wildcard,title="Choose a Connectome File",\
                         resizeable=False, \
                         default_directory=preference_manager.cviewerui.cffpath)

        if dlg.open() == OK:
            
            if not os.path.isfile(dlg.path):
                logger.error("File '%s' does not exist!"%dlg.path)
                return
            
            # if file exists and has .cff ending
            if os.path.exists(dlg.paths[0]) and (dlg.paths[0]).endswith('.cff'):
                
                # close the cfile if one is currently loaded
                cfile.close_cfile()
                
                # load cfile data
                cfile.load_cfile(dlg.paths[0])
                
                self.window.status_bar_manager.message=''
            elif os.path.exists(dlg.paths[0]) and (dlg.paths[0]).endswith('meta.cml'):
                cfile.close_cfile()
                cfile.load_cfile(dlg.paths[0], ismetacml = True)
            else:
                logger.info('Could not load file: '+ dlg.paths)
                
                
class SaveCFile(Action):
    """ An action that save aconnectome file """

    name        = "Save CFF File"
    description = "Save the connectome file"
    tooltip     = "Save a CFF file"
    path        = Str("MenuBar/File/LoadDataMenu")
    image       = ImageResource("cff-open.png", search_path=[IMAGE_PATH])

    # Is the action enabled?
    enabled = Bool(True)

    # Is the action visible?
    visible = Bool(True)

    ###########################################################################
    # 'Action' interface.
    ###########################################################################

    def perform(self, event, cfile=None):
        """ Performs the action. """
        
        logger.info('Performing save connectome file action')
        
        # helper variable to use this function not only in the menubar
        exec_as_funct = True
        
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')

        wildcard = "Connectome File Format v2.0 (*.cff)|*.cff|" \
                    "All files (*.*)|*.*"
                    
        dlg = FileDialog(wildcard=wildcard,title="Save as Connectome File",\
                         resizeable=False, action = 'save as',\
                         default_directory=preference_manager.cviewerui.cffpath,)
        
        if dlg.open() == OK:

            if (dlg.paths[0]).endswith('.cff'):
                
                cfflib.save_to_cff(cfile.obj, dlg.paths[0])
                logger.info("Saved connectome file to %s" % dlg.paths[0])
