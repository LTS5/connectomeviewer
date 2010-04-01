"""RenderManager class dealing with rendering of the networks

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# Standard library imports
import numpy as np
import sys

# Enthought library imports
from enthought.traits.api import HasTraits, Instance, Any, List
from enthought.traits.ui.api import View, Item, Group

# ConnectomeViewer imports
from cviewer.plugins.cff.ui.node_view import Node
from cviewer.visualization.node_picker import NodePicker
from cviewer.plugins.cff.interfaces.i_network import INetwork
from cviewer.plugins.ui.preference_manager import preference_manager

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)

    
class RenderManager(HasTraits):
    """ Handles the multiple views (scenes) on the network and surface
    data, handles picking as well. """
    
    from enthought.mayavi.core.scene import Scene
    from enthought.mayavi.api import Engine
    
    # the scene for the 3D view of the network
    scene3d = Instance(Scene)
    
    # VTK Graph Pipeline in 2D
    scene2d = Instance(Scene)
   
    # the mayavi engine
    engine = Instance(Engine)

    # the network this render manager manages
    network = Instance(INetwork)

    # nodepicker
    nodepicker = Instance(NodePicker)

    # cmatrix viewer
    cmatrix_viewer = Any
    
    # references to the data
    # -----
    # the node glyphs
    nodes = Any
    # the selected node glyphs (actor)
    nodes_selected = Any
    # the node source
    nodesrc = Any
    # a dictionary with references to created text3d labels
    # keyed by the node index
    node_labels = Any
    
    # connectivity source
    vectorsrc = Any
    # connectivity actor
    vectors = Any
    
    # surface source
    surfsrc = Any
    # surface triangular mesh actor
    surf = Any
    
    # threshold
    thres = Any
    # attribute selector
    attract = Any
    
    node_info_view = View(Group(
                            Group(Item(name='nodepicker', style='custom'),
                                    show_border=True, show_labels=False),
                              Group(Item(name='show_gui'),
                                    Item(name='auto_raise'), show_border=True),
                              ),
                        resizable=True,
                        buttons=['OK'])
    
    
    def __init__(self, network, **traits):
        super(RenderManager, self).__init__(**traits)
        
        from enthought.mayavi import mlab
        
        logger.info('Initalize RenderManager...')

        # set the datasource manager
        self.datasourcemanager = network.datasourcemanager
        
        # getting mlab engine
        self.engine = mlab.get_engine()
        
        # store reference to managed network
        self.network = network
        
        # create the scene instances
        self._create_scenes()
        
        # use nice trackball camera interactor
        # self.scene3d.scene.interactor.interactor_style = tvtk.InteractorStyleTrackballCamera()
        # otherwise just use tvtk.InteractorStyleSwitch()
                
        # create my node picker inclusive the NodePickHandler
        self.nodepicker = NodePicker(renwin=self.scene3d.scene)
                
        # add custom picker for the scene3d
        self.scene3d.scene.picker = self.nodepicker
        
        # adding observers to capture key events
        self.scene3d.scene.interactor.add_observer('KeyPressEvent', self.nodepicker.key_pressed)

        # add the callback function after a pick event  
        self.scene3d.scene.picker.pointpicker.add_observer('EndPickEvent', self._nodepicked_callback)
        
        # initialize node_labels
        self.node_labels = {}
        
        logger.info('RenderManager created.')


    ###############
    # Scene2D Handling
    ###############
    
    def _create2DScene(self):
        """ Create the Mayavi Scene2D and adds it to the current engine """
        
        figure = self.engine.new_scene(name="2D View: "+self.network.networkname)
        figure.scene.background = (0, 0, 0)
        return figure
    
    def visualize_scene2d(self):
        """ Creates the VTK Graph Pipeline to render the Graph """
        
        from enthought.tvtk.api import tvtk
        
        # precondition for the graph visualization
        if self.scene2d is None:
            self.scene2d = self._create2DScene()
            
        # alias for currently used scene
        fig = self.scene2d

        # get source object
        srcobj = self.datasourcemanager.get_sourceobject()
        
        # XXX_start: the following section for graph data structure generation
        # might be encapsulated in another function
        
        edges, weights = self.datasourcemanager.get_current_edges_and_weights()

        # create table
        T = tvtk.Table()
        
        col1 = tvtk.IntArray()
        col1.name = "From"
        for ele in edges:
            col1.insert_next_value(ele[0])
        T.add_column(col1)

        col2 = tvtk.IntArray()
        col2.name = "To"
        for ele in edges:
            col2.insert_next_value(ele[1])
        T.add_column(col2)

        col3 = tvtk.FloatArray()
        col3.name = "Weight"
        for ele in weights:
            col3.insert_next_value(ele)
        T.add_column(col3)

        # XXX_end
        
        # build the pipeline (using TVTK)

        graph = tvtk.TableToGraph()
        graph.add_input_connection(T.producer_port)
        #graph.AddLinkVertex("From", "Name", False)
        #graph.AddLinkVertex("To", "Name", False)
        graph.add_link_edge("From", "To")
        #graph.SetVertexTableConnection(vertex_table.GetOutputPort())

        view = tvtk.GraphLayoutView()
        view.add_representation_from_input_connection(graph.output_port)
        #view.vertex_label_array_name = "label"
        #view.vertex_label_visibility = True
        #view.vertex_color_array_name = "Age"
        view.color_vertices = False
        view.color_edges = True
        #view.set_layout_strategy_to_simple2d()
        view.set_layout_strategy_to_force_directed()
        # random, pass_trhough, fast2d, simple2d, community2d, circular, clustering2d

        # Add my new layout strategy
        # myFoo = vtkCircularLayoutStrategy()
        # view.SetLayoutStrategy(myFoo)
        
        theme = tvtk.ViewTheme().create_mellow_theme()
        theme.cell_color = (0.2,0.2,0.6)
        theme.line_width = 5
        theme.point_size = 10
        view.apply_view_theme(theme)
        view.vertex_label_font_size = 20

        # the rendering part is a bit different than
            #window = vtkRenderWindow()
            #window.SetSize(600, 600)
            #view.SetupRenderWindow(window)
            #view.GetRenderer().ResetCamera()
            #window.GetInteractor().Start()
        # for mayavi. maybe this can be simplified
        # XXX: where is the scene interactor set?
        
        # get renderer
        ren = view.renderer
        ren.reset_camera()
        props = ren.view_props
        props0 = props.get_item_as_object(0)
        map0 = props0.mapper
        graph0 = map0.get_input_data_object(0,0)
        
        # export to polydata
        pd = tvtk.GraphToPolyData()
        pd.set_input(graph0)
        pd.update()
        # same as  pd.get_output() or pd.output
        polydata = pd.get_output_data_object(0)
        
        a = tvtk.Actor(mapper=map0)
        
        # finally, add the actor to the scene2d
        fig.scene.add_actor(a)
        # and we are lucky, aren't we?
        
    def update_scene2d(self):
        """ Grabs the graph infromation from the DataSourceManager and
            updates the Scene2D rendering Pipeline """
            
        # XXX: is it at all possible to update the underlying datastructure
        # and then update the pipeline with the approach taken here?
        
        pass

    def _nodepicked_callback(self, picker_obj, evt):
        """ The callback function to determine what to do with a picked node """
        from enthought.tvtk.api import tvtk

        picker_obj = tvtk.to_tvtk(picker_obj)
        picked = picker_obj.actors

        # if picked actor are the glyphs
        if self.nodes.actor.actor._vtk_obj in [o._vtk_obj for o in picked]:

            pos = picker_obj.picked_positions[0]
            
            dist_small = sys.maxint # the biggest integer
            
            for i, points in enumerate(self.nodes.mlab_source.points):
                # if picked positions ON a node
                dist = np.linalg.norm((np.array(pos)-  points))
                # save the smallest distance node
                if dist <= dist_small:
                    j = i
                    dist_small = dist

            # get the id of the picked node
            picknodeid = 'n' + str(j+1)

            if self.nodepicker.show_info:
                
                # generate a nice view for the information
                nodedata = self.network.graph.node[picknodeid]

                nodedata['dn_internal_id'] = picknodeid

                deg = self.network.graph.degree(picknodeid)
                from types import IntType, StringType
                if type(deg) == IntType or type(deg) == StringType:
                    nodedata['degree'] = deg
                    
                # does this node has a self-loop?
                if self.network.graph.has_edge(picknodeid, picknodeid):
                    nodedata['has_selfloop'] = True
                else:
                    nodedata['has_selfloop'] = True
                    
                mynode = Node(nodedata=nodedata)
                mynode.configure_traits()
            

            elif self.nodepicker.toggle_selection:
                # toggles the selection of a node
                if self.datasourcemanager._srcobj.selected_nodes[j] == 1:
                    # unpick
                    self._select_nodes(selection_node_array = np.array([j]), \
                                       first_is_selected = True, change_vector = False)
                else:
                    # pick
                    self._select_nodes(selection_node_array = np.array([j]), activate = True, \
                                       first_is_selected = True, change_vector = False)

            elif self.nodepicker.select_node:
                # toggle the picking state              
                if self.datasourcemanager._srcobj.selected_nodes[j] == 1:
                    # unpick
                    self._select_nodes(selection_node_array = np.array([j]), \
                                       first_is_selected = True)
                else:
                    # pick
                    self._select_nodes(selection_node_array = np.array([j]), activate = True, \
                                       first_is_selected = True)

            elif self.nodepicker.select_node2:
                
                # get the neighbors of the picked node in an array
                nbrs = self.datasourcemanager._srcobj.relabled_graph.neighbors(j)
                # put the two lists together
                cur_nodes = np.append(j, np.array(nbrs))
                
                if self.datasourcemanager._srcobj.selected_nodes[j] == 1:
                    # unpick
                    self._select_nodes(selection_node_array = cur_nodes, \
                                       first_is_selected = True)
                else:
                    # pick
                    self._select_nodes(selection_node_array = cur_nodes, activate = True ,\
                                       first_is_selected = True)
                    
            elif self.nodepicker.toggle_label:
                
                if self.node_labels.has_key(j):
                    # we want to remove the node label from the pipeline first
                    # and then delete the entry in the dictionary
                    a = self.node_labels[j]
                    a.remove()
                    del self.node_labels[j]
                    
                else:
                    srcobj = self.datasourcemanager._srcobj
                    # create the text3d instance and add it to the dictionary
                    from enthought.mayavi import mlab
                    a = mlab.text3d(srcobj.positions[j,0], srcobj.positions[j,1], \
                                                srcobj.positions[j,2], \
                                                '     ' + srcobj.labels[j], # white spaces are hackish
                                                name = 'Node ' + srcobj.labels[j])
                    self.node_labels[j] = a

            else:
                logger.info("No valid pick operation.")
                

    def _select_nodes(self, selection_node_array, activate = False, first_is_selected = False, change_vector = True):
        """ Helper function to update the datasourceobject nodes selection status
        and update the currently rendered objects
        
        Parameters
        ----------
        fist_is_selected : boolean
            if the first node id in the selection_node_array is a clicked node
        change_vector : boolean
            add edges for visualization
        
        """
        logger.debug('_select_nodes invoked')
        import time
        
        # update the edges
        if first_is_selected:
            if change_vector:
                now=time.clock()
                self._select_edges(activate = activate, fis = selection_node_array[0])
                logger.debug('change_vector True: _select_edges used %s s' % str(time.clock()-now))
        else:
            now=time.clock()
            self._select_edges(activate = activate)
            logger.debug('first_is_selected False: _select_edges used %s s' % str(time.clock()-now))
        
        now=time.clock()
        tmparr = self.nodesrc.mlab_source.dataset.point_data.get_array('selected_nodes')
        num_tmparr = tmparr.to_array()
        logger.debug('getting nodesrc mlab array %s s' % str(time.clock()-now))

        now=time.clock()
        # setting the datastructure
        self.datasourcemanager._srcobj.selected_nodes[selection_node_array] = int(activate)
        logger.debug('setting selected_nodes in srcmng %s s' % str(time.clock()-now))
        
        # update rendering information
        num_tmparr[selection_node_array] = int(activate)
        
        now=time.clock()
        # update the nodes
        tmparr = num_tmparr
        self.nodesrc.mlab_source.update()
        logger.debug('update the nodes %s s' % str(time.clock()-now))

    def _select_edges(self, activate, fis = -1):
        """ Helper function to update the selected edges in the datasourceobject
        based on the currently selected nodes and update the currently rendered edges.
        """
        
        # get source object
        srcobj = self.datasourcemanager.get_sourceobject()

        # if there are no edges in this network, return
        if len(srcobj.edges) == 0:
            return

        if not fis == -1:
            # first value is the starting node
            if not srcobj.directed:
                idx, pos = np.where(srcobj.edges == fis)
                # idx stores the index, pos either from or to in tuple
                # for undirected, we can discard pos
                # now, we can (de)activate the edges corresponding to fis
                srcobj.selected_edges[idx] = int(activate)
                self.datasourcemanager._update_vector()
            else:
                # TODO
                # how to select the edges if the graph is directed?
                pass
        else:
            # just select the whole subgraph spanned by the selected_nodes
            if activate:
                tmp_selnodes = np.where(srcobj.selected_nodes == 0)
            else:
                tmp_selnodes = np.where(srcobj.selected_nodes == 1)
            if len(tmp_selnodes) >= 1:
                for i in tmp_selnodes[0]:
                    idx, pos = np.where(srcobj.edges == i)
                    srcobj.selected_edges[idx] = int(activate)
            self.datasourcemanager._update_vector()

        # update vectorsrc
        self.vectorsrc.mlab_source.set(u=srcobj.vectors[0],v=srcobj.vectors[1], w=srcobj.vectors[2])
        self.vectorsrc.mlab_source.update()
        

    def _show_surface(self, forced = False):
        """ Shows all the surfaces patches of the selected nodes depending
        on the show_surface status
        
        Parameters
        ----------
        forced : boolean
            If true, the surfaces have to be displayed
        
        
        """
        # precondition
        # can only be called when there is already a surface rendered
        if self.surfsrc is None: return
        
        # get the intensity value of the selected nodes
        sellist = self.network.get_selectiongraph_list()
        if not len(sellist) == 0:
            iva = np.zeros(len(sellist))
            for i, nodeid in enumerate(sellist):
                if self.network.graph.node[nodeid].has_key('dn_intensityvalue'):
                        iva[i] = int(self.network.graph.node[nodeid]['dn_intensityvalue'])
        else:
            iva = None

        # set the show_surfaces status to True if forced
        if forced:
            self.datasourcemanager._srcobj.show_surface = True

        # updates the surface label array
        self.datasourcemanager._update_labelarr(intensityvalue_array = iva)
        
        # toggle the surface visibility        
        # the shape of the scalars and vertices array have to be consistent at this point (use set)
        assert self.surfsrc.mlab_source.scalars.shape[0] == self.datasourcemanager._srcobj.labelarr.shape[0]
        
        self.surfsrc.mlab_source.set(scalars = self.datasourcemanager._srcobj.labelarr.ravel())
        self.surfsrc.mlab_source.update()

    def _toggle_surface(self):
        """ Toggles visibility of the surface of the selected nodes
        
        """

        # precondition
        # can only be called when there is already a surface rendered
        if self.surfsrc is None: return
        
        iva = None
        if not self.datasourcemanager._srcobj.show_surface:
            # get the intensity value of the selected nodes
            sellist = self.network.get_selectiongraph_list()
            iva = np.zeros(len(sellist))
            for i, nodeid in enumerate(sellist):
                if self.network.graph.node[nodeid].has_key('dn_intensityvalue'):
                        iva[i] = int(self.network.graph.node[nodeid]['dn_intensityvalue'])

        self.datasourcemanager._srcobj.show_surface = not self.datasourcemanager._srcobj.show_surface

        # updates the surface label array
        self.datasourcemanager._update_labelarr(intensityvalue_array = iva)
        
        # toggle the surface visibility
        # the shape of the scalars and vertices array have to be consistent at this point (use set)
        assert self.surfsrc.mlab_source.scalars.shape[0] == self.datasourcemanager._srcobj.labelarr.shape[0]

        self.surfsrc.mlab_source.set(scalars = self.datasourcemanager._srcobj.labelarr.ravel())
        self.surfsrc.mlab_source.update()
        
    def _create3DScene(self):
        """ Creates a 3D Scene """
        figure = self.engine.new_scene(name="3D View: "+self.network.networkname)
        figure.scene.show_axes = True
        figure.scene.background = (0, 0, 0)
        return figure

    def _create_scenes(self):
        """ Reopens the closed scenes """
        self.scene3d = self._create3DScene()
        # Uncomment to activate again
        #self.scene2d = self._create2DScene()
        
    def close_scenes(self):
        """ Closes all scenes for this render manager """
        
        from enthought.mayavi import mlab
        eng = mlab.get_engine()
        try:
            # Uncomment to activate again
            #eng.close_scene(self.scene2d)
            eng.close_scene(self.scene3d)
            
        except Exception:
            pass
        # mlab.close(self.scene3d)

        
    def layout_3dview(self, surfacecontainerid, surfaceid):
        """ Visualizes the 3D view, initial rendering if necessary!
        
        Parameters
        ----------
        surfacecontainerid : int
            The surface container id to use for layouting
        surfaceid : int
            The particular surface to use for layouting
        
        """
        import time
        
        tic = time.time()
        # compute the sourceobject for layouting
        self.datasourcemanager._compute_3DLayout(surfacecontainerid, surfaceid)
        toc = time.time()
        logger.debug("SourceObject has been changed successfully. (Time: %s)" % str(toc-tic))

        # disable rendering for speedup
        self.scene3d.scene.disable_render = True

        # set the correct scene for the update (scene3d)
        self.engine.current_scene = self.scene3d

        # is there already data in the scenes, when not, create.
        if self.nodesrc is None and self.vectorsrc is None and self.surfsrc is None:
            
            tic = time.time()
            self.visualize_graph()
            toc = time.time()
            logger.debug('Graph rendered. Time: %s' % str(toc-tic))
            
            tic = time.time()
            self.visualize_surface()
            toc = time.time()
            logger.debug('Surface rendered. Time: %s' % str(toc-tic))
        else:
            # get the source object
            tso = self.datasourcemanager.get_sourceobject()
            
            tic = time.time()
            # update the positions of the network nodes
            self.nodesrc.mlab_source.set(x=tso.positions[:,0],\
                                                 y=tso.positions[:,1],\
                                                 z=tso.positions[:,2])
            self.nodesrc.mlab_source.update()
            
            toc = time.time()
            logger.debug('Node position updated. Time: %s' % str(toc-tic))
            
            tic = time.time()
            # update start_positions and vectors for the network
            self.vectorsrc.mlab_source.set(x=tso.start_positions[0],\
                y=tso.start_positions[1],z=tso.start_positions[2],\
                u=tso.vectors[0],v=tso.vectors[1],w=tso.vectors[2])
            
            # update the rendering
            self.vectorsrc.mlab_source.update()
            toc = time.time()
            logger.debug('Connectivity source updated. Time: %s' % str(toc-tic))
            
            tic = time.time()
            # update surface coordinates, triangles and labelarray
            # important: use reset because dimensions might have changed
            self.surfsrc.mlab_source.reset(x=tso.daV[:,0],\
                y=tso.daV[:,1], z=tso.daV[:,2],\
                triangles=tso.daF,\
                scalars=tso.labelarr.ravel())
            toc = time.time()
            
            # update the surface visibility
            self._show_surface()
            
            logger.debug('Surface source updated. Time: %s' % str(toc-tic))
            
        # enable rendering again
        self.scene3d.scene.disable_render = False
        
        # update statusbar (with do_later)
        from enthought.pyface.timer.api import do_later
        do_later(self.network._parentcfile._workbenchwin.status_bar_manager.set, message = '')

    def visualize_surface(self):
        """ Visualizes the given surface """
        from enthought.mayavi import mlab
        
        # precondition for surface visualization
        if self.scene3d is None:
            self.scene3d = self._create3DScene()

        # alias for currently used scene
        fig = self.scene3d

        # get source object
        srcobj = self.datasourcemanager.get_sourceobject()

        # dummy scalar array includes zero up to max
        scalars_dummy = np.zeros(srcobj.labelarr_orig.ravel().shape )
        scalars_dummy[-1] = np.max(srcobj.labelarr_orig)

        self.surfsrc = mlab.pipeline.triangular_mesh_source(srcobj.daV[:,0],srcobj.daV[:,1],srcobj.daV[:,2],\
                             srcobj.daF, name='Surface', scalars = scalars_dummy) #srcobj.labelarr_orig.ravel() )
        
        # to ameliorate vtkLookupTable: Bad table range
        outsurf = mlab.pipeline.select_output(self.surfsrc)
        
        thr = mlab.pipeline.threshold(outsurf, low=0.0, up=np.max(srcobj.labelarr_orig) )
        thr.auto_reset_lower = False
        thr.auto_reset_upper = False
        thr.filter_type = 'cells'

        self.surf = mlab.pipeline.surface(thr, colormap='prism', opacity = 1.0)
        self.surf.actor.mapper.interpolate_scalars_before_mapping = True
        
        # setting surfsrc to labelarr
        self.surfsrc.mlab_source.scalars = srcobj.labelarr
        self.surfsrc.mlab_source.update()
        
        # set lower threshold to 1
        thr.lower_threshold = 1.0
        thr.upper_threshold = np.max(srcobj.labelarr_orig)
        
     
    def visualize_graph(self):
        """ Visualize the graph with the 3D view  """
        from enthought.mayavi import mlab
        from enthought.tvtk.api import tvtk
        
        # precondition for the graph visualization
        if self.scene3d is None:
            self.scene3d = self._create3DScene()
            
        # alias for currently used scene
        fig = self.scene3d

        # get source object
        srcobj = self.datasourcemanager.get_sourceobject()

        # create node source
        #####
        self.nodesrc = mlab.pipeline.scalar_scatter(srcobj.positions[:,0], srcobj.positions[:,1], srcobj.positions[:,2],\
                                               figure = fig, name = 'Node Source')

        # adding scalar data arrays
        # create a sequence array to map the node ids to color
        from numpy import array
        ar = array(range(1,len(srcobj.nodeids)+1))
        
        self.nodesrc.mlab_source.dataset.point_data.add_array(ar)
        self.nodesrc.mlab_source.dataset.point_data.get_array(0).name = 'nodeids'

        self.nodesrc.mlab_source.dataset.point_data.add_array(srcobj.selected_nodes)
        self.nodesrc.mlab_source.dataset.point_data.get_array(1).name = 'selected_nodes'
        
        # setting the active attributes
        actsel1 = mlab.pipeline.set_active_attribute(self.nodesrc, point_scalars='nodeids', \
                                                     name="Colored Nodes")
        actsel2 = mlab.pipeline.set_active_attribute(self.nodesrc, point_scalars='selected_nodes', \
                                                     name="Selected Nodes")
                
        # create glyphs
        self.nodes = mlab.pipeline.glyph(actsel1, scale_factor=3.0, scale_mode='none',\
                              name = 'Nodes', mode='cube')
        self.nodes.glyph.color_mode = 'color_by_scalar'
        # FIXME:
        # interpolate setting to true makes the color mapping wrong on linux
        # otherwise on windows, if not set to true, the cubes stay dark (why?)
        from sys import platform
        if 'win32' in platform:
            self.nodes.actor.mapper.interpolate_scalars_before_mapping = True
        
        # if there exists a colormap, use it
        l = self.nodes.glyph.module.module_manager.scalar_lut_manager.lut
        # number of colors has to be greater equal to 2
        if not len(srcobj.nodeids) < 2:
            l.number_of_colors = len(srcobj.nodeids)
            l.build()
        
            # getting colors from source object
            l.table = srcobj.colors
        
        # create glyphs for selected nodes
        self.nodes_selected = mlab.pipeline.glyph(actsel2, scale_mode = 'scalar', \
                                                  opacity = 0.2, scale_factor = 7.0, \
                                                    name = 'Nodes', color = (0.92, 0.98, 0.27))
        self.nodes_selected.glyph.color_mode = 'no_coloring'
        self.nodes_selected.glyph.glyph.clamping = False
        self.nodes_selected.glyph.glyph_source.glyph_source.phi_resolution = 12
        self.nodes_selected.glyph.glyph_source.glyph_source.theta_resolution = 12
        self.nodes_selected.actor.mapper.interpolate_scalars_before_mapping = True
        
        # should I load the node labels?
        # this is too slow for many nodes
        if preference_manager.cviewerui.labelload:
            for index, label in enumerate(srcobj.labels):
                tmplabel = mlab.text(srcobj.positions[index,0], srcobj.positions[index,1], \
                                     label, z=srcobj.positions[index,2], \
                                     width=0.010*len(label), \
                                    color = (1,1,1), name='Node Label ' + label)
                tmplabel.property.shadow = True

        # split up what was achieved before by quiver3d
        # add a vector_scatter data source with scalars    
        self.vectorsrc = mlab.pipeline.vector_scatter(srcobj.start_positions[0], 
                             srcobj.start_positions[1],
                             srcobj.start_positions[2],
                             srcobj.vectors[0],
                             srcobj.vectors[1],
                             srcobj.vectors[2],
                             name = 'Connectivity Source',
                             figure = fig)
        
        lastkey = ''
        # add all the scalars we have (from the srcobj scalarsdict)
        # this should be an ordered dict!
        for key, value in srcobj.scalarsdict.items():
            
            da = tvtk.DoubleArray(name=str(key))
            da.from_array(srcobj.scalarsdict[key])
            
            self.vectorsrc.mlab_source.dataset.point_data.add_array(da)
            self.vectorsrc.mlab_source.dataset.point_data.scalars = da.to_array()
            self.vectorsrc.mlab_source.dataset.point_data.scalars.name = str(key)
            lastkey = str(key)
            
        # this is the winning time-consuming line, and can be ommited in further mayavi releases
        self.vectorsrc.outputs[0].update()
        
        # add a set active attribute filter to select the scalars
        self.attract = mlab.pipeline.set_active_attribute(self.vectorsrc, point_scalars=lastkey, \
                                                     name="Set Edge Attribute")
        
        self.thres = mlab.pipeline.threshold(self.attract, name="Thresholding")
        # to prevent vtkLookupTable: Bad table range
        self.thres.filter_type = 'cells'
        #self.thres.threshold_filter.all_scalars = False

        if srcobj.directed:
            
            self.vectors = mlab.pipeline.vectors(self.thres,\
                                            colormap='OrRd',
                                            figure=fig,
                                            name='Connections',
                                            mode='arrow',
                                            scale_factor=1,
                                            resolution=20,
                                            scale_mode = 'vector')
            
            self.vectors.glyph.glyph_source.glyph_source.shaft_radius = 0.015
            self.vectors.glyph.glyph_source.glyph_source.shaft_resolution = 20
            
            self.vectors.glyph.glyph_source.glyph_source.tip_radius = 0.04
            self.vectors.glyph.glyph_source.glyph_source.tip_length = 0.15
            self.vectors.glyph.glyph_source.glyph_source.tip_resolution = 20
            
            self.vectors.glyph.color_mode = 'color_by_scalar'
            self.vectors.glyph.glyph.clamping = False
            
        else:
            # then create a .vectors filter to display
            self.vectors = mlab.pipeline.vectors(self.thres,\
                                            colormap='OrRd',
                                            #mode='cylinder',
                                            figure=fig,
                                            name='Connections',
                                            #scale_factor=1,
                                            #resolution=20,
                                            # make the opacity of the actor depend on the scalar.
                                            #transparent=True,
                                            scale_mode = 'vector')

            self.vectors.glyph.glyph_source.glyph_source.glyph_type = 'dash'
            # vectors.glyph.glyph_source.glyph_source.radius = 0.01
            self.vectors.glyph.color_mode = 'color_by_scalar'
            self.vectors.glyph.glyph.clamping = False
          
    def update_node_scale_factor(self, scale_factor):
        """ Updates the scale factor for both the colored and the selected nodes """
        self.nodes.glyph.glyph.scale_factor = float(scale_factor)
        self.nodes_selected.glyph.glyph.scale_factor = float(scale_factor) * 2.33

    def update_graph_visualization(self):
        """ Updates the graph visualization, taking the information
        from the source object.  """
        import time
        
        # update the node position

        # disable rendering for speedup
        self.scene3d.scene.disable_render = True

        # set the correct scene for the update (scene3d)
        self.engine.current_scene = self.scene3d

        tso = self.datasourcemanager.get_sourceobject()
        
        # update the positions of the network nodes
        tic = time.time()
        self.nodesrc.mlab_source.set(x=tso.positions[:,0],\
                                             y=tso.positions[:,1],\
                                             z=tso.positions[:,2])
        self.nodesrc.mlab_source.update()
        toc = time.time()
        logger.debug('Node positions updated. Time: %s' % str(toc-tic))
        
        # update start_positions and vectors for the network
        tic = time.time()
        self.vectorsrc.mlab_source.set(x=tso.start_positions[0],\
            y=tso.start_positions[1],z=tso.start_positions[2],\
            u=tso.vectors[0],v=tso.vectors[1],w=tso.vectors[2])
        
        # update the rendering
        self.vectorsrc.mlab_source.update()
        toc = time.time()
        logger.debug('Connectivity source updated. Time: %s' % str(toc-tic))
        tic = time.time()
    
        # enable rendering again
        self.scene3d.scene.disable_render = False
                
        # update the node size and color
        # XXX
            
    def invoke_matrix_viewer(self):
        """ Invoke the Conectome Matrix Viewer """
        from cviewer.visualization.matrix.cmatrix_viewer import CMatrixViewer
        self.cmatrix_viewer = CMatrixViewer(self.network)
        self.cmatrix_viewer.edit_traits()


