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

""" Defines an action for displaying the welcome perspective.
"""

#------------------------------------------------------------------------------
#  Imports:
#------------------------------------------------------------------------------

from os.path import join, dirname

from enthought.traits.api import Instance, Bool, Str
from enthought.pyface.api import ImageResource
from enthought.pyface.action.api import Action as PyFaceAction
from enthought.envisage.ui.action.api import Action, ActionSet, Group, Menu

#from enthought.pyface.workbench.action.workbench_action import WorkbenchAction
#
#from enthought.envisage.ui.workbench.action.api import \
#    ExitAction, EditPreferencesAction, AboutAction

from enthought.envisage.ui.workbench.workbench_window import WorkbenchWindow

#------------------------------------------------------------------------------
#  "WelcomeAction" class:
#------------------------------------------------------------------------------

class WelcomeAction(PyFaceAction):
    """ An action that displays the welcome perspective.
    """

    #--------------------------------------------------------------------------
    #  "Action" interface:
    #--------------------------------------------------------------------------

    # A longer description of the action.
    description = "Welcome to Puddle"

    # The action"s image (displayed on tool bar tools etc).
    image = ImageResource("welcome")

    # The action"s name (displayed on menus/tool bar tools etc).
    name = "&Welcome"

    # A short description of the action used for tooltip text etc.
    tooltip = "Welcome to Puddle"

    #--------------------------------------------------------------------------
    #  "NewWindowAction" interface:
    #--------------------------------------------------------------------------

    window = Instance(WorkbenchWindow)

    #--------------------------------------------------------------------------
    #  "Action" interface:
    #--------------------------------------------------------------------------

    def perform(self, event):
        """ Perform the action.
        """
        welcome_perspective = self.window.get_perspective_by_id(
            "puddle.perspective.welcome_perspective")

        self.window.active_perspective = welcome_perspective


welcome_action = Action(
    path        = "MenuBar/Help",
    class_name  = __name__ + '.WelcomeAction',
    name        = "Welcome",
    group       = "WelcomeGroup",
)

#------------------------------------------------------------------------------
#  "PuddleActionSet" class:
#------------------------------------------------------------------------------

class PuddleActionSet(ActionSet):
    """ The default action set for the Puddle plugin.
    """
    menus = [Menu(name="&Help", path="MenuBar",
                groups=["WelcomeGroup", "AboutGroup"], after="Tools")]

    actions = [welcome_action]

# EOF -------------------------------------------------------------------------
