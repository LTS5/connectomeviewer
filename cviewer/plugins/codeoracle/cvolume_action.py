from enthought.mayavi import mlab
import numpy as np

v = np.array( [ [0,0,0], [1,1,0], [0,1,0]], dtype=np.float)
f = [(0,1,2)]

#mlab.triangular_mesh(v[:,0],v[:,1],v[:,2], f)

from oracle import Oracle
#from enthought.scripting.api import Recorder, start_recording, stop_recording

a=Oracle()
a.lines.append("test")
a.configure_traits()