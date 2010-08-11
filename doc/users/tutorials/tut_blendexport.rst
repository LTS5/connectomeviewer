.. _blendexport:

=====================================
Export volumetric datasets to Blender
=====================================

It is straightforward to generate a binary file from volumetric data that can be used
by Blender. The script is available in *examples/Blender/Exporter*. The following function will do::

	def generate_blender_voxeldata(fname, data):
	    """ Write a binary file readable by Blender
	    
	    Parameters
	    ----------
	    fname : String
	        Filename to store the output, ending with .bin
	    data : ndarray
	        If the array is 3 dimensional, assume a structural image
	        If the array is 4 dimensional, assume a functional image
	        
	    Note
	    ----
	    For instructions on how to render the structural or functional images,
	    see http://brainblend.sourceforge.net/index.html
	    """
	    
	    import struct
	    import numpy as np
	
	    if len(data.shape) == 4:
	        frames = data.shape[3]
	        time = True
	    elif len(data.shape) == 3:
	        frames = 1
	        Time = False
	    else:
	        raise Exception('Data array has no valid dimension!')
	        
	    xdim = data.shape[0]
	    ydim = data.shape[1]
	    zdim = data.shape[2]
	    
	    # write data to binary file
	    binfile = open(fname, 'wb')
	    x = struct.pack('i', xdim)
	    y = struct.pack('i', ydim)
	    z = struct.pack('i', zdim)
	    f = struct.pack('i', frames)
	
	    binfile.write(x)
	    binfile.write(y)
	    binfile.write(z)
	    binfile.write(f)
	
	    # convert to float32
	    data = np.array(data, dtype = np.float32)
	
	    if time:
	        for i in range(frames):
	            print 'write frame... ', i
	            data[:,:,:,i].tofile(binfile)
	    else:
	        data.tofile(binfile)
	
	    binfile.close()

You can use the generated file with structural or functional information as volumetric texture.
Please refer to the `BrainBlend documentation <http://brainblend.sourceforge.net/index.html>`_.
You might to want to do some data preprocessing as well.

The `Blender documentation about Volume Rendering <http://www.blender.org/development/release-logs/blender-250/volume-rendering/>`_
has more information. A template Blender file *importing_voxeldata_struct.blend* is provided in the example folder.

