.. _basicinter:

========================================
Basics of ConnectomeViewer Interactivity
========================================

You have the ConnectomeViewer up and running. You downloaded a sample connectome file
and opened it. Now, this tutorial demonstrates the basic steps to visualize the data.

1. You need to activate a network first. This loads the surfaces and volumes into memory.
And the network layouting is calculated. When nothing else is specified, the nodes are positioned
at the center of gravity of their corresponding surfaces patches. Double-click the network
you want to activate. In the console, you should see debug messages when you started
the ConnectomeViewer with the verbose (-v) option.

2. You should now see nodes placed in the 3D View. But without edges. Usually, it is messy
to look at all the edges at once. So there are ways to select edges, but that's for later.
To show all the edges, you have to right-click on the network to open the context menu
and select ``Select All Nodes and Edges``. You should know see your complete rendered network including edges.
(For our Mac users, right-clicking is substituted by pressing the Control key and clicking).
Using the option ``Unselect All Nodes and Edges`` will turn off all the edges again.
 
3. There are a bunch of key bindings usable in the 3D View. To display them, click on the main
menu *Help -> Key Bindings*. A HTML pages pops up in your browser, listing all the keys. We will
focus on a few for the start.

4. First, unselect all nodes. In the 3D View, the nodes are shown as cubes (this can be changed
in the *Visualization Object Editor*). The nodes can have two states: either selected or unselected.
Selected nodes show a transparent yellow sphere around the cubes. To toggle the selection of nodes,
you need to point your mouse cursor directly over the node you want to select in the 3D View. Now
press the ``key t`` on your keyboard to change the selection. You need to take care to really
place the cursor above the node, you can zoom and drag the network for this.

5. Similarily, if you press the ``key i`` when over a node with the mouse cursor,
an information box pops up with information about this particular node. If the nodes have an
attribute ``dn_label``, you can toggle the label by pressing the ``key g``.

6. Similarily, you can press the ``key 1`` when over a node to display all adjacent edges for
undirected graphs. The neighboring nodes are thereby not selected. Pressing ``key 1`` again unselects
them again. For the same functionality but to select the neighboring nodes automatically, use ``key 2``.
This might become later useful when you have a very dense graph which you want to explore. Selected nodes
can be used then to define a subgraph. This subgraph might then be used for further analysis.

7. You can also selectively show surfaces patches. Namely for selected nodes. Right-click on the activated
network in the *Connectome File View* and select ``Toggle Surface Visibility``.

8. Finally, we want to know how to threshold networks and select particular edge attributes.
Select all nodes and edges as explained above. Now, find the *Mayavi Visualization Tree* View. This
tree defines all the visualization objects in the 3D View. Under the ``Connectivity Source``, which
represents all the edges in the visualization, there two filters, ``Set Edge Attribute`` and ``Thresholding``.

9. If you click on ``Set Edge Attribute``, the corresponding dialog in the *Visualization Object Editor* gets
updated. Here you can select particular edge attributes for the entry "Point scalars name". The network
in the 3D View is updated immediately.

10. Similarily, when you click on ``Thresholding`` in the *Mayavi Visualiztion Tree*, you can now
choose an upper and a lower threshold for your edge values in the *Visualization Object Editor*.

.. Using the IPython Shell with dragged objects.
