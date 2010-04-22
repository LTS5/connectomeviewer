""" Routines to load, save and validate a connectome file

CNetwork: Load/Parse GraphML
CVolume: Load Nifti
CSurface. Load Gifti
CTrack: Load TrackVis

Dependencies
------------
* lxml
* NetworkX
* NiPy (for Nifti IO)
* DiPy (for TrackVis IO)
* ConnectomeViewer (for Gifti IO)

"""

from cmetadata import CMetadata
from cfile import CFile
import util

def load(filename, *args, **kwargs):
    """ Load connectome file given filename
    
        Returns
        -------
        cfile : ``CFile``
                Connectome File Instance
                
    """
    
    if isinstance(filename, basestring):
        # check if file exists
        from os.path import isfile, abspath
        
        if isfile(filename):
            if filename.endswith('.cff'):
                fname = abspath(filename)
            else:
                raise RuntimeError('%s must end with .cff' % filename)
            
        else:
            raise RuntimeError('%s seems not to be a valid file string' % filename)
    else:
        raise RuntimeError('%s seems not to be a valid file string' % filename)


    # extract meta.xml from connectome file
    from zipfile import ZipFile, ZIP_DEFLATED
    _zipfile = ZipFile(fname, 'r', ZIP_DEFLATED)
    try:
        metadata_string = _zipfile.read('meta.xml')
    except: # XXX: what is the correct exception for read error?
        raise RuntimeError('Can not extract meta.xml from connectome file.')
    
    # create connectome file instance
    cfile = CFile()
    cfile.set(_src = fname)
    
    # try to parse meta.xml
    cmd = CMetadata(metadata_string)
    
    # set metadata
    cfile.set(metadata = cmd)
    
    # add tracks, lazy load data
    trkdict = util.parse_track(metadata_string)
    for name, ctr in trkdict.items():
        cfile.add_ctrack(ctr)
    
    # add volumes, lazy load data
    voldict = util.parse_volume(metadata_string)
    for name,cvo in voldict.items():
        cfile.add_cvolume(cvo)
    
    # add surfaces, lazy load data
    surdict = util.parse_surface(metadata_string)
    for name,csur in surdict.items():
        cfile.add_csurface(csur)
    
    # add networks with references to tracks, volumes, surfaces and metadata
    # XXX
    
    return cfile

def save(filename, cfile):
    """ Save connectome file to file on disk """
    cfile.to_filename(filename)
    
    