"""
Helper function to render volumes

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
    