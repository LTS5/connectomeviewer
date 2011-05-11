""" The Connectome Viewer Envisage Workbench Application class

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Standard library imports
from enthought.traits.api import Bool

# Enthought library import
from enthought.envisage.ui.workbench.api import WorkbenchApplication
from enthought.pyface.api import AboutDialog, ImageResource, SplashScreen

# ConnectomeViewer imports
from cviewer.plugins.ui.preference_manager import preference_manager
from cviewer.action.common import IMAGE_PATH
from .info import version

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

class CViewerWorkbenchApplication(WorkbenchApplication):
    """ The ConnectoneViewer workbench application. """

    # Turn this off if you don't want the workbench to start a GUI event loop.
    start_gui_event_loop = Bool(True, desc='start a GUI event loop')

    # Create an Envisage application.
    id = 'ch.connectome.viewer'

    # Path used to search for images
    _image_path = [IMAGE_PATH, ]

    # The icon used on window title bars etc.
    icon = ImageResource('favicon.ico', search_path=_image_path)

    # The name of the application (also used on window title bars etc).
    name = "Connectome Viewer " + str(version)
    
    ###########################################################################
    # 'WorkbenchApplication' interface.
    ###########################################################################

    def run(self):
        """ Run the application.

        This does the following:

        1) Starts the application
        2) Creates and opens a workbench window
        3) Starts the GUI event loop (only if start_gui_event_loop is
           True)
        4) When the event loop terminates, stops the application

        This particular method is overridden from the parent class to
        allow the user to not run the gui event loop as would be
        necessary when the loop is started elsewhere or when run fron
        IPython.

        """

        # Make sure the GUI has been created (so that, if required, the splash
        # screen is shown).
        gui = self.gui

        # Start the application.
        if self.start():
            # Create and open the first workbench window.
            window = self.workbench.create_window(
                position=self.window_position, size=self.window_size
            )
            window.open()

            # We stop the application when the workbench has exited.
            #self.workbench.on_trait_change(self._on_workbench_exited, 'exited')

            # Start the GUI event loop if needed.
            if self.start_gui_event_loop:
                # THIS CALL DOES NOT RETURN UNTIL THE GUI IS CLOSED.
                gui.start_event_loop()

        return

    def _about_dialog_default(self):
        """ Initialize the About Dialog """

        #from vtk import vtkVersion
        #vtk_version = vtkVersion().GetVTKVersion()
        #'VTK version %s' % (vtk_version),
        from cviewer.version import version
        
        adds = ['Connectome Viewer - Version %s' % ( version ),
                '',
                'Copyright &copy; 2009-2011, Ecole Polytechnique F&eacute;d&eacute;rale de Lausanne (EPFL) and',
                'University Hospital Center and University of Lausanne (UNIL-CHUV)',
                '',
                'Author: Stephan Gerhard <em>info [at] connectomics.org</em>',
                'Contributors: <em>see README file</em>',
                '',
                'This program comes with ABSOLUTELY NO WARRANTY',
                'It is licensed under Modified BSD License',
                ]
        
        about_dialog = AboutDialog(
            parent = self.workbench.active_window.control,
            image  = ImageResource('cviewer_about.png',
                                   search_path=self._image_path),
            additions = adds,
        )

        return about_dialog

    def _splash_screen_default(self):
        """ Initialize the Splash Screen """

        if preference_manager.cviewerui.show_splash_screen:
            splash_screen = SplashScreen(
                image = ImageResource('cviewer_about.png',
                                        search_path=self._image_path),
                show_log_messages = True,
            )
        else:
            splash_screen = None

        return splash_screen
