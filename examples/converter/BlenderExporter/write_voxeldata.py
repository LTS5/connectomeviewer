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

import struct
import numpy as np



# create data
frames = 10
size = (256,256,256,frames)

matrix = np.zeros( shape=size , dtype = float)
#matrix[:,:,:] = np.eye( 256,256)
matrix[0:50,0:50,:,0] = 1
matrix[0:50,0:50,:,1] = 2
matrix[0:50,0:50,:,2] = 3
matrix[0:50,0:50,:,3] = 5
matrix[0:50,0:50,:,4] = 10
matrix[0:50,0:50,:,5] = 100
matrix[0:50,0:50,:,6] = 200

# write data to binary file
binfile = open('bin.dat', 'wb')
x = struct.pack('i', size[0])
y = struct.pack('i', size[1])
z = struct.pack('i', size[2])
f = struct.pack('i', frames)

binfile.write(x)
binfile.write(y)
binfile.write(z)
binfile.write(f)

matrix.tofile(binfile)

binfile.close()
