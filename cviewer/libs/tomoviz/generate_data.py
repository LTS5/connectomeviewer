import numpy as np
import os, os.path
from glob import glob

def generate_big_data(l=200, t=20):
    x, y, z = np.mgrid[0:5:l*1j, 0:5:l*1j, 0:5:l*1j]
    if os.path.exists('data'):
        [os.remove(filename) for filename in glob('data/data*.npy')]
    else:
        os.mkdir('data')
    for t in np.linspace(0, 1, t + 1):
        field = np.sin(2*np.pi * ((x % 1) * (y % 1) * (z % 1) + t))
        np.save(os.path.join('data', 
                    'data_%.2f.npy' % t), field)

def generate_quarters():
    if os.path.exists('data'):
        [os.remove(filename) for filename in glob('data/quarters*.npy')]
    else:
        os.mkdir('data')
    x, y, z = np.mgrid[0:1:100j, 0:1:100j, 0:1:100j]
    l_data = [y < 0.5,
              y > 0.5,
              z < 0.5,
              z > 0.5,
              np.logical_and(y < 0.5, z < 0.5),        
              np.logical_and(y < 0.5, z > 0.5),        
              np.logical_and(y > 0.5, z < 0.5),        
              np.logical_and(y > 0.5, z > 0.5)]        
    for t in np.arange(8):
        np.save(os.path.join('data', 'quarters_%i.npy' % t),\
                l_data[t].astype(np.float))
    

