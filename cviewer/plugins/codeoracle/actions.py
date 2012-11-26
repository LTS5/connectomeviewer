import logging

from apptools.io.api import File
from pyface.api import FileDialog, OK
from pyface.action.api import Action
from traits.api import Any

from cviewer.plugins.text_editor.editor.text_editor import TextEditor
from cviewer.plugins.ui.preference_manager import preference_manager

# Logging imports
import logging
logger = logging.getLogger('root.'+__name__)
from nipype.interfaces.base import isdefined
from interfaces import (plot_nodes, plot_edges, plot_surfaces, 
plot_labels_by_phrase, plot_labels_by_degree, plot_volumes, create_rotation_frames)

class PlotTracks(Action):
    tooltip = "Plots tracks using Dipy's FVTK module"
    description = "Plots tracks using Dipy's FVTK module"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):               
        from ctrack_action import TrackParameter
        import nibabel as nb, nibabel.trackvis as trk
        from dipy.viz import fvtk
        import numpy as np
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')       
        so = TrackParameter(cfile)
        choices = False
        if not len(so.tracks.keys()) == 1:
            choices = True
        if choices:
            so.edit_traits(kind='livemodal')       
        track_name = so.tracks[so.tracks.keys()[0]]['name']
        if not track_name == "None":
            tracks, header = cfile.obj.get_by_name(track_name).data
            tmpname = '/tmp/' + track_name + '.trk'
            trk.write(tmpname, tracks, header)
            del tracks
            tracks, hdr = trk.read(tmpname, True, None)           
            streams_fixed = ((ii[0]) for ii in tracks)
            #streams = list(streams_fixed)
            streams = streams_fixed
            r=fvtk.ren()
            for stream in streams:
                fvtk.add(r, fvtk.line(stream, fvtk.blue, opacity=0.2, linewidth=2) )
            fvtk.show(r, title = "Fibers", size = (500,500))

class ShowHideNetworkName(Action):
    tooltip = "Show/Hide Network Name"
    description = "Show/Hide Network Name"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from mayavi import mlab
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        currentfig = mlab.gcf()
        figure_title = currentfig.name
        #network = no.netw[no.graph]['name']
        #graph = cfile.obj.get_by_name(network).data
        x = 0.02
        y = 0.02
        width = 0.3
        text = figure_title
        mlab.text(x,y,text,width=width)
        
class ShowHideNodeLegend(Action):
    tooltip = "Show/Hide Node Legend"
    description = "Show/Hide Node Legend"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        from mayavi import mlab
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        mlab.scalarbar(orientation='horizontal')


class ShowHideEdgeLegend(Action):
    tooltip = "Show/Hide Edge Legend"
    description = "Show/Hide Edge Legend"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from mayavi import mlab
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        mlab.scalarbar(orientation='vertical')
        
class PlotVolume(Action):
    """ Open a new file in the text editor
    """
    tooltip = "Plot Volume"
    description = "Plot Volume"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from cvolume_action import VolumeParameter
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        import nibabel as nb
        from mayavi import mlab
        currentfig = mlab.gcf()
        figure_title = currentfig.name
        
        so = VolumeParameter(cfile)
        choices = False
        if not len(so.volumes.keys()) == 1:
            choices = True
        if choices:
            so.edit_traits(kind='livemodal')       
        
        volume_name = so.volumes[so.volume]['name']
        if not volume_name == "None":
            volume = cfile.obj.get_by_name(volume_name).data
            tmpname = '/tmp/' + volume_name + '.nii'
            nb.save(volume, tmpname)
            if isdefined(figure_title):
                mlab.figure(figure_title)
            else:
                mlab.figure(tmpname)
            plot_volumes(tmpname)
        
class NewFigure(Action):
    tooltip = "New Figure"
    description = "New Figure"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from mayavi import mlab
        mlab.figure()

class ClearFigure(Action):
    tooltip = "Clear Figure"
    description = "Clear Figure"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from mayavi import mlab
        mlab.clf()

class PlotSurface(Action):
    """ Open a new file in the text editor
    """
    tooltip = "Plot Surface"
    description = "Plot Surface"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from csurface_action import SurfaceFileParameter
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        import nibabel.gifti as gifti
        from mayavi import mlab
        currentfig = mlab.gcf()
        figure_title = currentfig.name
        
        so = SurfaceFileParameter(cfile)
        choices = False
        if not len(so.surface_da.keys()) == 1:
            choices = True
        if choices:
            so.edit_traits(kind='livemodal')
        surface_name = so.surface_da[so.surface]['name']
        label_name = so.labels_da[so.labels]['name']
        if not surface_name == "None":
            surface = cfile.obj.get_by_name(surface_name).data
            tmpname = '/tmp/' + surface_name + '.gii'
            gifti.write(surface, tmpname)
            if isdefined(figure_title):
                mlab.figure(figure_title)
            else:
                mlab.figure(tmpname)
                
            if not label_name == "None":
                labels = cfile.obj.get_by_name(label_name).data
                tmplabelname = '/tmp/' + label_name + '.gii'
                gifti.write(labels, tmplabelname)
                plot_surfaces(tmpname, tmplabelname)
            else:
                plot_surfaces(tmpname)            
            
            
class PlotLabelsByDegree(Action):
    tooltip = "Plot labels by degree"
    description = "Plots node labels for nodes with the specified degree (e.g. 3)"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        from cnetwork_action import NodeLabelByDegreeParameter
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        from mayavi import mlab
        currentfig = mlab.gcf()
        figure_title = currentfig.name
        
        no = NodeLabelByDegreeParameter(cfile)
        choices = False
        if len(no.netw.keys()) == 1:
            for key in no.netw[no.netw.keys()[0]].keys():
                options = no.netw[no.netw.keys()[0]][key]
                if isinstance(options, str):
                    options = [options]
                if not len(options) == 1:
                    choices = True
                    break
        else:
            choices = True
        if choices:
            no.edit_traits(kind='livemodal')
        if not no.netw[no.graph]['name'] == "None":
            import tempfile
            import networkx as nx
            
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            network = no.netw[no.graph]['name']
            graph = cfile.obj.get_by_name(network).data
            tmpname = '/tmp/' + network + '.pck'
            nx.write_gpickle(graph, tmpname)
            node_position = no.node_position
            node_label_key = no.node_label
            degree = no.degree

            if isdefined(figure_title):
                mlab.figure(figure_title)
            else:
                mlab.figure(tmpname)
            plot_labels_by_degree(tmpname, degree, node_position, node_label_key)

class PlotNetwork(Action):
    tooltip = "Plot Network"
    description = "Plots nodes and edges from a selected network"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        from cnetwork_action import NoLabelNetworkParameter
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        from mayavi import mlab
        currentfig = mlab.gcf()
        figure_title = currentfig.name

        no = NoLabelNetworkParameter(cfile)
        choices = False
        if len(no.netw.keys()) == 1:
            for key in no.netw[no.netw.keys()[0]].keys():
                options = no.netw[no.netw.keys()[0]][key]
                if isinstance(options, str):
                    options = [options]
                if not len(options) == 1:
                    choices = True
                    break
        else:
            choices = True
        if choices:
            no.edit_traits(kind='livemodal')
        if not no.netw[no.graph]['name'] == "None":
            import tempfile
            import networkx as nx
            
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            network = no.netw[no.graph]['name']
            graph = cfile.obj.get_by_name(network).data
            tmpname = '/tmp/' + network + '.pck'
            nx.write_gpickle(graph, tmpname)
            node_position = no.node_position
            edge_key = no.edge_value

            if isdefined(figure_title):
                mlab.figure(figure_title)
            else:
                mlab.figure(tmpname)
            plot_edges(tmpname, node_position, edge_key)
            plot_nodes(tmpname, node_position, scalar_key='value')

class PlotEdges(Action):
    tooltip = "Plot edges"
    description = "Plots edges from a selected network"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        from cnetwork_action import EdgeParameter
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        from mayavi import mlab
        currentfig = mlab.gcf()
        figure_title = currentfig.name
        
        no = EdgeParameter(cfile)
        choices = False
        if len(no.netw.keys()) == 1:
            for key in no.netw[no.netw.keys()[0]].keys():
                options = no.netw[no.netw.keys()[0]][key]
                if isinstance(options, str):
                    options = [options]
                if not len(options) == 1:
                    choices = True
                    break
        else:
            choices = True
        if choices:
            no.edit_traits(kind='livemodal')

        if not no.netw[no.graph]['name'] == "None":
            import tempfile
            import networkx as nx
            
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            network = no.netw[no.graph]['name']
            graph = cfile.obj.get_by_name(network).data
            tmpname = '/tmp/' + network + '.pck'
            nx.write_gpickle(graph, tmpname)
            node_position = no.node_position
            edge_key = no.edge_value

            if isdefined(figure_title):
                mlab.figure(figure_title)
            else:
                mlab.figure(tmpname)
            plot_edges(tmpname, node_position, edge_key)

        
class PlotLabelsByPhrase(Action):
    tooltip = "Plot labels"
    description = "Plots node labels that contain the entered text (e.g. occipital)"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        from cnetwork_action import NodeLabelByPhraseParameter
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        from mayavi import mlab
        currentfig = mlab.gcf()
        figure_title = currentfig.name
        
        no = NodeLabelByPhraseParameter(cfile)
        no.edit_traits(kind='livemodal')

        if not no.netw[no.graph]['name'] == "None":
            import tempfile
            import networkx as nx
            
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            network = no.netw[no.graph]['name']
            graph = cfile.obj.get_by_name(network).data
            tmpname = '/tmp/' + network + '.pck'
            nx.write_gpickle(graph, tmpname)
            node_position = no.node_position
            node_label_key = no.node_label
            phrase = no.phrase
            
            if isdefined(figure_title):
                mlab.figure(figure_title)
            else:
                mlab.figure(in_file)
            plot_labels_by_phrase(tmpname, phrase, node_position, node_label_key)
        
class PlotNodes(Action):
    tooltip = "Plot Nodes"
    description = "Plots nodes from a selected network"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        from cnetwork_action import NodeParameter
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        from mayavi import mlab
        currentfig = mlab.gcf()
        figure_title = currentfig.name
        
        no = NodeParameter(cfile)
        choices = False
        if len(no.netw.keys()) == 1:
            for key in no.netw[no.netw.keys()[0]].keys():
                options = no.netw[no.netw.keys()[0]][key]
                if isinstance(options, str):
                    options = [options]
                if not len(options) == 1:
                    choices = True
                    break
        else:
            choices = True
        if choices:
            no.edit_traits(kind='livemodal')

        if not no.netw[no.graph]['name'] == "None":
            import tempfile
            import networkx as nx
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            network = no.netw[no.graph]['name']
            graph = cfile.obj.get_by_name(network).data
            tmpname = '/tmp/' + network + '.pck'
            nx.write_gpickle(graph, tmpname)
            node_position = no.node_position
            if isdefined(figure_title):
                mlab.figure(figure_title)
            else:
                mlab.figure(in_file)
            plot_nodes(tmpname, node_position, scalar_key='value')

class NetworkVizTubes(Action):
    tooltip = "Show 3D Network with Tubes"
    description = "Show 3D Network with Tubes and colorcoded Nodes"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from scripts import threedviz2

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(threedviz2)
        f.close()

        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)


class NetworkReport(Action):
    tooltip = "Network Report"
    description = "Network Report"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from scripts import reportlab

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(reportlab)
        f.close()

        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

class WriteGEXF(Action):
    tooltip = "Write Gephi GEXF file"
    description = "Write Gephi GEXF file"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from scripts import writegexf

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(writegexf)
        f.close()

        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

class CorticoCortico(Action):
    tooltip = "Extract cortico-cortico fibers"
    description = "Extract cortico-cortico fibers"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from scripts import corticocortico

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(corticocortico)
        f.close()

        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)


class NipypeBet(Action):
    tooltip = "Brain extraction using BET"
    description = "Brain extraction using BET"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from scripts import nipypebet

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(nipypebet)
        f.close()

        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)


class ShowTracks(Action):
    tooltip = "Show tracks between two regions"
    description = "Show tracks between two regions"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from scripts import ctrackedge

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(ctrackedge)
        f.close()
        
        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

class XNATPushPull(Action):
    tooltip = "Push and pull files from and to XNAT Server"
    description = "Push and pull files from and to XNAT Server"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from scripts import pushpull

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(pushpull)
        f.close()

        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

class ComputeNBS(Action):
    tooltip = "Compute NBS"
    description = "Compute NBS"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

#        from cnetwork_nbs_action import NBSNetworkParameter, NBSMoreParameter
        from scripts import nbsscript
#        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
#
#        no = NBSNetworkParameter(cfile)
#        no.edit_traits(kind='livemodal')
#
#        if (len(no.selected1) == 0 or len(no.selected2) == 0):
#            return
#
#        mo = NBSMoreParameter(cfile, no.selected1[0], no.selected2[0])
#        mo.edit_traits(kind='livemodal')
#
#        import datetime as dt
#        a=dt.datetime.now()
#        ostr = '%s%s%s' % (a.hour, a.minute, a.second)
        
#        if not (len(no.selected1) == 0 or len(no.selected2) == 0):
#            # if cancel, not create surface
#            # create a temporary file
#            import tempfile
#            myf = tempfile.mktemp(suffix='.py', prefix='my')
#            f=open(myf, 'w')
#            f.write(nbsscript % (str(no.selected1),
#                                 mo.first_edge_value,
#                                 str(no.selected2),
#                                 mo.second_edge_value,
#                                 mo.THRES,
#                                 mo.K,
#                                 mo.TAIL,
#                                 ostr))
#            f.close()
#
#            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

        import tempfile
        myf = tempfile.mktemp(suffix='.py', prefix='my')
        f=open(myf, 'w')
        f.write(nbsscript)
        f.close()
    
        self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)


class ShowNetworks(Action):
    tooltip = "Create a 3D Network"
    description = "Create a 3D Network"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from cnetwork_action import NetworkParameter
        from scripts import netscript
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        
        no = NetworkParameter(cfile)
        no.edit_traits(kind='livemodal')

        if not no.netw[no.graph]['name'] == "None":
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            f.write(netscript % (no.netw[no.graph]['name'],
                                  no.node_position,
                                  no.edge_value,
                                  no.node_label))
            f.close()
            
            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

class ConnectionMatrix(Action):
    tooltip = "Show connection matrix"
    description = "Show connection matrix"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from cnetwork_action import MatrixNetworkParameter
        from scripts import conmatrix
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        
        no = MatrixNetworkParameter(cfile)
        no.edit_traits(kind='livemodal')

        if not no.netw[no.graph]['name'] == "None":
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            f.write(conmatrix % (no.netw[no.graph]['name'],
                                  no.node_label))
            f.close()
            
            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

class SimpleConnectionMatrix(Action):
    tooltip = "Show simple connection matrix"
    description = "Show simple connection matrix"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):

        from cnetwork_action import MatrixEdgeNetworkParameter
        from scripts import conmatrixpyplot
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
        
        no = MatrixEdgeNetworkParameter(cfile)
        no.edit_traits(kind='livemodal')

        if not no.netw[no.graph]['name'] == "None":
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            f.write(conmatrixpyplot % (no.netw[no.graph]['name'],
                                  no.edge_label))
            f.close()

            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

class ShowSurfaces(Action):
    """ Open a new file in the text editor
    """
    tooltip = "Create a surface"
    description = "Create a surface"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from csurface_action import SurfaceParameter
        from scripts import surfscript
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
                
        so = SurfaceParameter(cfile)
        so.edit_traits(kind='livemodal')
        
        if not so.pointset_da[so.pointset]['name'] == "None":
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            if so.labels_da[so.labels].has_key('da_idx'):
                labels = so.labels_da[so.labels]['da_idx']
            else:
                labels = 0
            f.write(surfscript % (so.pointset_da[so.pointset]['name'],
                                  so.pointset_da[so.pointset]['da_idx'],
                                  so.faces_da[so.faces]['name'], 
                                  so.faces_da[so.faces]['da_idx'],
                                  so.labels_da[so.labels]['name'],
                                  labels))
            f.close()
            
            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)


class ShowVolumes(Action):
    """ Open a new file in the text editor
    """
    tooltip = "Create a volume"
    description = "Create a volume"

    # The WorkbenchWindow the action is attached to.
    window = Any()

    def perform(self, event=None):
        
        from cvolume_action import VolumeParameter
        from scripts import volslice
        cfile = self.window.application.get_service('cviewer.plugins.cff2.cfile.CFile')
                
        so = VolumeParameter(cfile)
        so.edit_traits(kind='livemodal')
        
        if True: #not so.pointset_da[so.pointset]['name'] == "None":
            # if cancel, not create surface
            # create a temporary file
            import tempfile
            myf = tempfile.mktemp(suffix='.py', prefix='my')
            f=open(myf, 'w')
            f.write(volslice % so.volumes[so.myvolume]['name'])
            f.close()
            
            self.window.workbench.edit(File(myf), kind=TextEditor,use_existing=False)

