""" Handling of the template atlases

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

# Standard library import
import os.path
from os.path import join

def get_template_atlas(atlastype):
    """ Returns the path to the gifti file
    
    Parameters
    ----------
    atlastype: {'homo_sapiens_01'}
        Only one human atlas so far included
        
    """
    if atlastype == 'homo_sapiens_01':
        return join(os.path.dirname(__file__),'template_atlas_' + atlastype,'Gifti', 'fsaverage.gii')
    
def get_template_atlas_label(atlastype):
    """ Returns the path to the gifti label file
    
    Parameters
    ----------
    atlastype: {'homo_sapiens_01'}
        Only one human atlas so far included
        
    """
    if atlastype == 'homo_sapiens_01':
        return join(os.path.dirname(__file__),'template_atlas_' + atlastype, 'Gifti', 'fsaverage_labels.gii')
