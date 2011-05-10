"""
Helper function to render volumes

"""
# Copyright (C) 2009-2010, Ecole Polytechnique Federale de Lausanne (EPFL) and
# University Hospital Center and University of Lausanne (UNIL-CHUV)
#
# Modified BSD License

# Inspired by the neurspin nipy trunk, thanks to Gael Varoquaux


def render_vol(data, new_window = True):
    """ Renders simple volume data such as ROI """

    from enthought.mayavi import mlab
    import numpy as np
    
    if new_window:
        mlab.figure(1,fgcolor=(0, 0, 0), bgcolor=(1, 1, 1))
        mlab.clf()
        
    src = mlab.pipeline.scalar_field(data)
    src.image_data.update_data()
    
    volume = mlab.pipeline.contour(src)
    volume_surf = mlab.pipeline.surface(volume)
    

if __name__ == '__main__':
    import nifti
    import numpy as np
    a = np.random.random((10,10,10))
    render_vol(data=a)
    