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

from cviewer.io.nipy.api import load_image
import struct
import numpy as np

fname = 'ROI.bin'
#a = load_image('/home/stephan/Research/Working Repository/Datasets/fMRI/Baltimore/sub17017/anat/mprage_skullstripped.nii.gz')
#a = load_image('/home/stephan/Research/Working Repository/Datasets/fMRI/Baltimore/sub17017/func/rest.nii.gz')
a = load_image('ROI.nii.gz')

newarr = np.array(a._data, dtype = np.float32)

#newarr = np.zeros( (datao.shape[0], datao.shape[1], datao.shape[2], datao.shape[3]), dtype = np.float32)

#newarr[:,:,:,0] = data[:,:,:,50]
#newarr[:,:,:,1] = data[:,:,:,60]
#newarr[:,:,:,2] = data[:,:,:,70]
#

#newarr[(newarr > 1000) ] = 0.0

# normalize slice-wise
#for j in range(newarr.shape[3]):
#    print 'normalize ', j
#    ma = np.max(newarr[:,:,:,j])
#    print 'maxval', ma
#    if ma == 0.0:
#        continue
#    newarr[:,:,:,j] = newarr[:,:,:,j] / ma


#newarr[(newarr < 900) ] = 0.0
#newarr[(newarr > 1500) ] = 0.0

#newarr = newarr * 10

newarr[(newarr != 0.0)] = 1000.0



#data = newarr / np.max(newarr)
data = newarr
#data = np.array(newarr, dtype = np.float32)

print 'max value after conversion', np.max(data)

if len(data.shape) == 4:
    frames = data.shape[3]
elif len(data.shape) == 3:
    frames = 1
else:
    raise Exception('Data array has no valid dimension!')
    
xdim = data.shape[0]
ydim = data.shape[1]
zdim = data.shape[2]

 #write data to binary file
binfile = open(fname, 'wb')
x = struct.pack('i', xdim)
y = struct.pack('i', ydim)
z = struct.pack('i', zdim)
f = struct.pack('i', frames)

binfile.write(x)
binfile.write(y)
binfile.write(z)
binfile.write(f)

for i in range(frames):
    print 'iteration ', i
    data[:,:,:].tofile(binfile)

binfile.close()
