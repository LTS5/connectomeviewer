surfscript = """
# Imports
from enthought.mayavi import mlab

# Retrieve data from connectome file
surface_file_vertices = cfile.obj.get_by_name("%s")[0]
vertices = surface_file_vertices.data.darrays[%s].data

surface_file_faces = cfile.obj.get_by_name("%s")[0]
faces = surface_file_faces.data.darrays[%s].data

surface_file_labels = cfile.obj.get_by_name("%s")[0]
labels = surface_file_labels.data.darrays[%s].data

# Ensure that (triangluar) faces have dimension (N,3)
if len(faces.shape) == 1:
    faces = faces.reshape( (len(faces) / 3, 3) )

# Ensure that each vertices has a corresponding label
assert vertices.shape[0] == len(labels)

# Create triangular mesh
x, y, z = vertices[:,0], vertices[:,1], vertices[:,2]
mlab.triangular_mesh(x, y, z, faces, scalars = labels)

"""

surfscript_nola = """
# Imports
from enthought.mayavi import mlab

# Retrieve data from connectome file
surface_file_vertices = cfile.obj.get_by_name("%s")[0]
vertices = surface_file_vertices.data.darrays[%s].data

surface_file_faces = cfile.obj.get_by_name("%s")[0]
faces = surface_file_faces.data.darrays[%s].data

# Ensure that (triangluar) faces have dimension (N,3)
if len(faces.shape) == 1:
    faces = faces.reshape( (len(faces) / 3, 3) )

# Create triangular mesh
x, y, z = vertices[:,0], vertices[:,1], vertices[:,2]
mlab.triangular_mesh(x, y, z, faces)

"""