from envisage.ui.action.api import Action, ActionSet, Group

class TextEditorActionSet(ActionSet):
    """ The default action set for the Text Editor plugin.
    """

    groups = [
        Group(
            id = "TextFileGroup",
            path = "MenuBar/File",
            before = "ExitGroup",
        )
    ]

    actions = [
        Action(
            id = "NewFileAction",
            name = "New Python File",
            class_name='cviewer.plugins.text_editor.actions.NewFileAction',
            group='TextFileGroup',
            path="MenuBar/File",
        ),

        Action(
            id = 'OpenFile',
            name = "Open Python File ...",
            class_name='cviewer.plugins.text_editor.actions.OpenFileAction',
            group='TextFileGroup',
            path="MenuBar/File",
        ),
    ]


