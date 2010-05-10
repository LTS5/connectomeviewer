#>>> import struct
#>>> binfile = open('bin.dat', 'wb')
#>>> for num in range(50):
#...   data = struct.pack('i', num)
#...   binfile.write(data)
#...
#>>> binfile = open('bin.dat', 'rb')
#>>> intsize = struct.calcsize('i')
#>>> while 1:
#>>>   data = binfile.read(intsize)
#...   if data == '':
#...     break
#...   num = struct.unpack('i', data)
#...   print num

def generate_blender_voxeldata(fname, data):
    """ Write a binary file readable by Blender
    
    Parameters
    ----------
    fname : String
        Filename to store the output, ending with .bin
    data : ndarray
        If the array is 3 dimensional, assume a structural image
        If the array is 4 dimensional, assuma a functional image
    """
    

    import struct
    import numpy as np

    if len(data.shape) == 4:
        frames = data.shape[3]
    elif len(data.shape) == 3:
        frames = 1
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
    #binfile.write(z)
    #binfile.write(y)
    #binfile.write(x)
    #binfile.write(f)

    # convert to float32
    data = np.array(data, dtype = np.float32)

    data.tofile(binfile)

    binfile.close()
