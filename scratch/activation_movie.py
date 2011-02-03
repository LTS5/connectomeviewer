"""
Rendering a movie of activation.
"""
import glob
import sys

# Major scientific library imports
import numpy as np
from scipy import ndimage, linalg

from enthought.mayavi import mlab

# Neuroimaging library imports
from nipy.neurospin.datasets import as_volume_img, VolumeImg
from nipy.neurospin import mask as mask_utils
from nipy.neurospin import viz3d
from nipy.neurospin.viz_tools import anat_cache

from joblib import Memory
REDUCE = 1

mlab.options.offscreen = True

f = mlab.figure(512, bgcolor=(0, 0, 0), size=(1025/REDUCE, 769/REDUCE))
mlab.clf()
f.scene.disable_render = True

################################################################################
# Constants

mem = Memory(cachedir='cache', mmap_mode='r', debug=True)

################################################################################
# Volume rendering of the anat
anat_data, anat_affine, anat_vmax = anat_cache._AnatCache.get_anat()
anat_src = viz3d.affine_img_src(anat_data, anat_affine, name='Anat')

anat_blurred = ndimage.gaussian_filter(
                (ndimage.morphology.binary_fill_holes(
                    ndimage.gaussian_filter(
                            (anat_data > 4800).astype(np.float), 6)
                        > 0.5
                    )).astype(np.float),
                2)

# The cortex shell
vol = mlab.pipeline.volume(anat_src, color=(1, 1, 1))

# Change the opacity function
from enthought.tvtk.util.ctf import PiecewiseFunction

shell_size = .1
otf = PiecewiseFunction()
otf.add_point(0, 0)
otf.add_point((.5-shell_size)*anat_vmax, 0.)
# If black background, use 0.2, if white, use .15
otf.add_point(.5*anat_vmax, 0.2)
otf.add_point((.5+shell_size)*anat_vmax, 0.)
otf.add_point(anat_vmax, 0)
vol._volume_property.set_scalar_opacity(otf)
vol.update_ctf = True

mlab.view(25, 70, 310, (1.3, -16.1, 3.27))
mlab.savefig('glass_brain.png', size=(960/REDUCE, 768/REDUCE))

################################################################################
# Render activation
filenames = sorted(glob.glob(
    '/volatile/varoquau/data/data/subject1/functional/fMRI/session1/swfga*.hdr'
    ))


def load_data(filenames):
    mask      = mask_utils.compute_mask_files(filenames)
    series, _ = mask_utils.series_from_mask([filenames, ], mask)
    series    = series.squeeze()
    series -= series.mean(axis=-1)[:, np.newaxis]
    std = series.std(axis=-1)
    std[std==0] = 1
    series /= std[:, np.newaxis]

    u, s, v   = linalg.svd(series, full_matrices=False)
    s[0] = 0
    series = np.dot(u*s, v)
    return mask, series

mask, series = mem.cache(load_data)(filenames)

FMRI_MAX = np.abs(series).max()

ref_img  = as_volume_img(filenames[0], world_space='mni152')
fmri_affine = ref_img.affine

anat_img = VolumeImg(anat_blurred, anat_affine, world_space='mni152')
anat_img = anat_img.resampled_to_img(ref_img)
blurred_data = anat_img.get_data()


def render_fmri(volume):
    data = np.zeros(mask.shape)
    data[mask] = volume
    data  *= blurred_data

    fmri_src = viz3d.affine_img_src(data, fmri_affine,
                                        name='fMRI')
    vol = mlab.pipeline.volume(fmri_src)

    # Change the opacity function
    from enthought.tvtk.util.ctf import PiecewiseFunction

    width = .1
    otf = PiecewiseFunction()
    otf.add_point(-FMRI_MAX, .2)
    otf.add_point(-width*FMRI_MAX, 0.)
    otf.add_point(width*FMRI_MAX, 0.)
    otf.add_point(FMRI_MAX, .2)
    vol._volume_property.set_scalar_opacity(otf)
    vol.update_ctf = True
    return fmri_src


mlab.view(25, 70, 310, (1.3, -16.1, 3.27))


for i, data in enumerate(series.T[:782]):
    fmri_src = render_fmri(data)
    print >> sys.stderr, i
    f.scene.camera.azimuth(1)
    #f.render()
    mlab.savefig('movie/rotation_3d_networks%03i.png' % i, 
            size=(960/REDUCE, 768/REDUCE))
    fmri_src.remove()
    #f.scene.disable_render = False

"""
mencoder -ovc lavc -mf fps=25 'mf://rot*.png' -of avi -lavcopts vcodec=mjpeg -ofps 25 -noskip -o output.avi
"""


# EOF ##########################################################################
