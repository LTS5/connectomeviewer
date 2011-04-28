.. _redbut:

=====================================
Recording interaction with red button
=====================================

One might want to change attributes of the visualization directly with the mouse. But for
subsequent visualizations, one does not want to go through all the property setting again,
and rather would like to add the changes as scripting commands.

This is possible using the record mode of Mayavi. The following series of images makes this clear.

Click in the menu bar on *View -> Other*.

.. figure:: ../../_static/tut_redbutton/1.png

Select the *Mayavi Visualization Tree*.

.. figure:: ../../_static/tut_redbutton/2.png

Now, click on the red button to activate the record mode. A window pops up.

.. figure:: ../../_static/tut_redbutton/3.png

Do your modifications to your visualization now. For example, here we select an arrow glyph source
for our current 3D View.

.. figure:: ../../_static/tut_redbutton/4.png

See the automatically generated scripting output in the recording window

.. figure:: ../../_static/tut_redbutton/5.png

You can copy and paste the relevant code now into your own script. This would be Line 12 and 13 in our case.

One might have to adapt the output script in some cases, for example when you have many 3D View
visualizations open for many networks. In line 12, it would read *engine.scenes[XX]...* with XX being the view you
want to manipulate.

The visualization objects (like vectors above) are also accessible using the render manager of an activated networks.

Type in the IPython shell::

	cfile.networks[0].rendermanager.print_traits()

In our case above, the *vectors* variable refers to the same objects as::

	cfile.networks[0].rendermanager.vectors

That's it for this tutorial. If you want to go more in-depth of Mayavi scripting, see the `Mayavi documentation <http://code.enthought.com/projects/mayavi/docs/development/html/mayavi/advanced_scripting.html>`_.
Happy visualization scripting!
