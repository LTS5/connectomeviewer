""" Datasets shipped with the Brain Connectivity Toolbox exposed as
NumPy arrays """

# this information is almost useless if one has not a rough idea of
# what the nodes mean, i.e. what brain regions they correspond to

import _bct as bct
import numpy as np

'''
      fve30.mat; fve32.mat (BD networks).
      Two version the macaque visual cortex.
      fve30.mat: 30 nodes, 311 links.
      fve32.mat: 32 nodes, 320 links.
      Reference: Felleman and van Essen (1991). Contributor: OS.
      Used in e.g. Sporns et al. (2000), Sporns and Kotter (2004).
'''
fve32 = np.asarray(bct.from_gsl(bct.to_gslm(bct.fve32,32,32)))
fve30 = np.asarray(bct.from_gsl(bct.to_gslm(bct.fve30,30,30)))


'''
      macaque47.mat (BD network).
      Large scale cortico-cortical connectivity matrix of the visual and
      sensorimotor areas in the macaque. 47 nodes; 505 links.
      Used in e.g. Honey et al. (2007). Contributor: RK.
'''
macaque47 = np.asarray(bct.from_gsl(bct.to_gslm(bct.macaque47,47,47)))

'''
      macaque71.mat (BD network).
      Macaque cortical connectivity: 71 nodes, 746 links.
      Reference: Young (1993). Contributor: OS.
      Used in e.g. Sporns (2002).
'''
macaque71 = np.asarray(bct.from_gsl(bct.to_gslm(bct.macaque71,71,71)))


"""
      cat.mat (WD networks).
      Connection matrices of cat cortex.
      CIJall contains all cortical and thalamic areas: 95 nodes, 2126 links.
      CIJctx contains only 52 cortical areas: 52 nodes, 820 links.
      Reference: Scannell et al. (1999). Contributor: OS.
      Used in e.g. Sporns and Zwi (2004), Sporns and Kotter (2004).
"""
cat_all = np.asarray(bct.from_gsl(bct.to_gslm(bct.cat_all,95,95)))
cat_ctx = np.asarray(bct.from_gsl(bct.to_gslm(bct.cat_ctx,52,52)))
