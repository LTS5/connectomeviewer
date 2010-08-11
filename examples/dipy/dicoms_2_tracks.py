from cviewer.libs.dipy.io import dicomreaders as dcm
import cviewer.libs.dipy.core.generalized_q_sampling as gq
import cviewer.libs.dipy.core.dti as dt
import cviewer.libs.dipy.core.track_propagation as tp
from cviewer.libs.dipy.viz import fos

import time
import numpy as np

# path to your dicom files
dname ='/path/to/your/dicoms'

# dicom file endings
endings = '*.dcm'

#The datasets need to have isotropic voxel size.

t1=time.clock()
data,affine,bvals,gradients=dcm.read_mosaic_dir(dname, globber=endings)

t2=time.clock()
print ('load data in %d secs' %(t2-t1))

x,y,z,g = data.shape
print('data shape is ',data.shape)

#calculate QA
gqs=gq.GeneralizedQSampling(data,bvals,gradients)
print('gqs.QA.shape ',gqs.QA.shape)

t3=time.clock()
print ('Generate QA in %d secs' %(t3-t2))

T=tp.FACT_Delta(gqs.QA,gqs.IN,seeds_no=10000).tracks

t4=time.clock()
print ('Create %d QA tracks in %d secs' %(len(T),t4-t3))

#calculate single tensor
ten=dt.Tensor(data,bvals,gradients,thresh=50)
t5=time.clock()
print('Create FA in %d secs' %(t5-t4))
     
T2=tp.FACT_Delta(ten.FA,ten.IN,seeds_no=10000,qa_thr=0.2).tracks
t6=time.clock()
print ('Create %d FA tracks in %d secs' %(len(T2),t6-t5))

T2=[t+np.array([100,0,0]) for t in T2]

print('Red tracks propagated based on QA')
print('Green tracks propagated based on FA')
r=fos.ren()
fos.add(r,fos.line(T,fos.red))
fos.add(r,fos.line(T2,fos.green))
fos.show(r)




