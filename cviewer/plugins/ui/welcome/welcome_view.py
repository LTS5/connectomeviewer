#------------------------------------------------------------------------------
# Copyright (C) 2009 Richard W. Lincoln
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to
#  deal in the Software without restriction, including without limitation the
#  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
#  sell copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
#  IN THE SOFTWARE.
#------------------------------------------------------------------------------

""" Defines the ConnectomeViewer WelcomeView
"""

#------------------------------------------------------------------------------
#  Imports:
#------------------------------------------------------------------------------

from os.path import exists, dirname, join
from enthought.traits.api import Instance, File, Event, Str, Font, Color
from enthought.traits.ui.api import View, Item, TreeEditor
from enthought.pyface.image_resource import ImageResource
from enthought.pyface.workbench.api import View as WorkbenchView
from enthought.enable.api import Viewport, Canvas, Component, Pointer
from enthought.enable.component_editor import ComponentEditor
from enthought.kiva.backend_image import Image as KivaImage
from enthought.kiva.fonttools.font import str_to_font

from enthought.enable.colors import ColorTrait

from cviewer.action.common import IMAGE_PATH

#------------------------------------------------------------------------------
#  "Image" class:
#------------------------------------------------------------------------------

class Image(Component):
    """ Defines an image component.
    """
    image_file = File

    bgcolor = "transparent"

    #---------------------------------------------------------------------------
    #  Draw component on the graphics context:
    #---------------------------------------------------------------------------

    def _draw_mainlayer(self, gc, view_bounds=None, mode="default"):

        if exists(self.image_file):
            gc.save_state()

            img = KivaImage(self.image_file)

            w, h = img.width(), img.height()
            self.bounds = [w, h]
            gc.draw_image(img, (self.x, self.y, w, h))

            gc.restore_state()

#------------------------------------------------------------------------------
#  "RelativeText" class:
#------------------------------------------------------------------------------

class RelativeText(Component):
    """ Defines an image component.
    """

    #--------------------------------------------------------------------------
    #  "RelativeText" interface:
    #--------------------------------------------------------------------------

    text = Str

    font = Font

    font_color = Color

    #--------------------------------------------------------------------------
    #  Component interface:
    #--------------------------------------------------------------------------

    #bgcolor = "fuchsia"

    #--------------------------------------------------------------------------
    #  Draw component on the graphics context:
    #--------------------------------------------------------------------------

    def _draw_mainlayer(self, gc, view_bounds=None, mode="default"):

        gc.save_state()

        font = str_to_font( str(self.font) )
        gc.set_font(font)

        gc.set_fill_color(self.font_color_)
#        gc.set_fill_color(self.font_color)

        x = gc.width() * 0.25
        y = gc.height() * 0.19

        gc.show_text_at_point(self.text, x, y)

        self.position = [x, y]
#            self.bounds = [w, h]

        gc.restore_state()

#------------------------------------------------------------------------------
#  "RelativeImage" class:
#------------------------------------------------------------------------------

class RelativeImage(Component):
    """ Defines an image component.
    """
    normal_pointer = Pointer("arrow")
    hover_pointer = Pointer("hand")

    image_file = File#(filter="Image Files (*.png, *.jpg, *.gif)|" \
#        "*.png;*.jpg;*.gif|All Files (*.*)|*.*")

    bgcolor = "transparent"

    selected = Event

    #---------------------------------------------------------------------------
    #  Draw component on the graphics context:
    #---------------------------------------------------------------------------

    def _draw_mainlayer(self, gc, view_bounds=None, mode="default"):

        if exists(self.image_file):
            gc.save_state()

    #        self.image_file.seek(0)
            img = KivaImage(self.image_file)

            x = gc.width() * 0.2
            y = gc.height() * 0.25
            w, h = img.width(), img.height()

            # Use Image's ability to draw itself onto a gc to paint the window.
            gc.draw_image(img, (x, y, w, h))

            self.position = [x, y]
            self.bounds = [w, h]

            gc.restore_state()


    def hover_left_down(self, event):
        """ Handles left mouse button clicks in 'normal' mode.
        """
        self.selected = True


    def normal_mouse_enter(self, event):
        """ Handles the mouse entering the component in 'normal' mode.
        """
        self.event_state = "hover"
        event.window.set_pointer(self.hover_pointer)
#        event.window.set_mouse_owner(self, event.net_transform())
        event.handled = True


    def hover_mouse_leave(self, event):
        """ Handles the mouse leaving the component in 'normal' mode.
        """
        self.event_state = "normal"
        event.window.set_pointer(self.normal_pointer)
#        event.window.set_mouse_owner(None)
        event.handled = True
        self.request_redraw()

#------------------------------------------------------------------------------
#  "WelcomeView" class:
#------------------------------------------------------------------------------

class WelcomeView(WorkbenchView):
    """ Defines the ConnectomeViewer WelcomeView
    """

    #--------------------------------------------------------------------------
    #  "IView" interface:
    #--------------------------------------------------------------------------

    # The view's globally unique identifier:
    id = "cviewer.plugins.cff.ui.cff_view.welcome_view"

    # The view's name:
    name = "Welcome"

    # The default position of the view relative to the item specified in the
    # "relative_to" trait:
    position = "left"
    
    # Path used to search for images
    _image_path = [IMAGE_PATH, ]

    # An image used to represent the view to the user (shown in the view tab
    # and in the view chooser etc).
    image = ImageResource("welcome", search_path=_image_path)

    # The width of the item (as a fraction of the window width):
    width = 1.0

    # The category sed to group views when they are displayed to the user:
    category = "General"

    #--------------------------------------------------------------------------
    #  "WelcomeView" interface:
    #--------------------------------------------------------------------------

    canvas = Instance(Canvas)

    # A view into a sub-region of the canvas.
    viewport = Instance(Viewport, desc="canvas sub-region view")

    #--------------------------------------------------------------------------
    #  "IView" interface:
    #--------------------------------------------------------------------------

    def create_control(self, parent):
        """ Create the view contents.
        """
        ui = self.edit_traits(parent=parent, view=self._create_view(),
            kind="subpanel")

        return ui.control

    #--------------------------------------------------------------------------
    #  "WelcomeView" interface:
    #--------------------------------------------------------------------------

    def _canvas_default(self):
        """ Trait initialiser.
        """


        logo_path = join(IMAGE_PATH, "logos", "python.png")
        logo = Image(image_file=logo_path, bounds=[100, 196],
            position=[25, 25])

        workbench_path = join(IMAGE_PATH, "logos", "cviewer.jpg")
        
        workbench = RelativeImage(image_file=workbench_path, bounds=[128, 128],
                                  position=[0,0])
        workbench.on_trait_change(self._on_workbench, "selected")

        app_name = "Select ConnectomeViewer Perspective ..." #self.window.application.name
        title = RelativeText(text=app_name, font="DEFAULT 20",
            font_color="black", bounds=[66, 66])

        canvas = Canvas(bgcolor="white")
        canvas.add(logo)
        canvas.add(workbench)
        canvas.add(title)

        return canvas


    def _viewport_default(self):
        """ Trait initialiser.
        """
        vp = Viewport(component=self.canvas)

        vp.enable_zoom=False
        vp.view_position = [-5, -5]

        return vp


    def _create_view(self):
        """ Create a view with a component editor.
        """
        view = View(Item(name="viewport", show_label=False,
            editor=ComponentEditor()))

        return view


    def _on_workbench(self):
        """ Handle selection of the workbench option.
        """
        welcome_perspective = self.window.get_perspective_by_id(
            "cviewer.perspective.viewer_perspective")

        self.window.active_perspective = welcome_perspective

# EOF -------------------------------------------------------------------------
