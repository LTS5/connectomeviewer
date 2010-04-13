""" Helper utility to parse data """

from ctrack import CTrack
from cvolume import CVolume
from csurface import CSurface
#from cnetwork import CNetwork

try:
    import networkx as nx
except ImportError, m:
    raise
try:
    from lxml import etree
except ImportError, m:
    raise
try:
    import cviewer.io.gifti.gifti as gifti
except ImportError, m:
    msg = '''%s
%s
You need to install ConnectomeViewer for Gifti IO!

        ''' % (m, '_'*80)
    raise ImportError(msg)
    
try:
    from cviewer.io.nipy.api import load_image
except ImportError, m:
    msg = '''%s
%s
You need to install ConnectomeViewer for Nifti IO!

        ''' % (m, '_'*80)
    raise ImportError(msg)

try:
    import cviewer.plugins.dipy.io.trackvis as tv
except ImportError, m:
    msg = '''%s
%s
You need to install ConnectomeViewer for TrackVis File IO!

        ''' % (m, '_'*80)
    raise ImportError(msg)
    

def parse_track(xmlstring):
    """ Parses meta.xml xmlstring and creates ctrack objects
    
    Returns
    -------
    trkdict : dict
      dictionary keyed by track name with CTrack objects as value
      
    """

    trkdict = {}

    tree = etree.fromstring(xmlstring)
    
    # first child, loop through the meta-info
    nsprefix = "{%s}" % tree.nsmap[None]
    
    # Parse the KEYs
    for child in tree.iterchildren():
        if child.tag == (nsprefix+'connectome-track'):
            
            # create ctrack
            ctrack = CTrack()
            
            name = child.attrib['name']
            # check if name not yet existing
            if trkdict.has_key(name):
                raise RuntimeError('Duplicate connectome-track name in meta.xml!')
               
            src = child.attrib['src']
            fileformat = child.attrib['fileformat']
            
            description = ''
            for mchildren in child.iterchildren():
                if mchildren.tag == (nsprefix + 'description'):
                    if not mchildren.text is None:
                        description = mchildren.text
                    
            ctrack.set(name = name, src = src, fileformat = fileformat, \
                       description = description)
            
            trkdict[name] = ctrack
            
    return trkdict

def parse_volume(xmlstring):
    """ Parses meta.xml xmlstring and creates cvolume objects
    
    Returns
    -------
    voldict : dict
      dictionary keyed by volume name with CVolume objects as value
      
    """

    voldict = {}

    tree = etree.fromstring(xmlstring)
    
    # first child, loop through the meta-info
    nsprefix = "{%s}" % tree.nsmap[None]
    
    # Parse the KEYs
    for child in tree.iterchildren():
        if child.tag == (nsprefix+'connectome-volume'):
            
            # create cvolume
            cvolume = CVolume()
            
            name = child.attrib['name']
            # check if name not yet existing
            if voldict.has_key(name):
                raise RuntimeError('Duplicate connectome-volume name in meta.xml!')
               
            src = child.attrib['src']
            dtype = child.attrib['dtype']
            fileformat = child.attrib['fileformat']

            description = ''
            for mchildren in child.iterchildren():
                if mchildren.tag == (nsprefix + 'description'):
                    if not mchildren.text is None:
                        description = mchildren.text
                    
            cvolume.set(name = name, src = src, fileformat = fileformat, \
                       dtype = dtype, description = description)
            
            voldict[name] = cvolume
            
    return voldict

def parse_surface(xmlstring):
    """ Parses meta.xml xmlstring and creates csurface objects
    
    Returns
    -------
    surdict : dict
      dictionary keyed by surface name with CSurface objects as value
      
    """

    surdict = {}

    tree = etree.fromstring(xmlstring)
    
    # first child, loop through the meta-info
    nsprefix = "{%s}" % tree.nsmap[None]
    
    # Parse the KEYs
    for child in tree.iterchildren():
        if child.tag == (nsprefix+'connectome-surface'):
            
            # create csurface
            csurface = CSurface()
            
            name = child.attrib['name']
            # check if name not yet existing
            if surdict.has_key(name):
                raise RuntimeError('Duplicate connectome-surface name in meta.xml!')
               
            src = child.attrib['src']
            dtype = child.attrib['dtype']
            fileformat = child.attrib['fileformat']
            
            description = ''
            for mchildren in child.iterchildren():
                if mchildren.tag == (nsprefix + 'description'):
                    if not mchildren.text is None:
                        description = mchildren.text
                    
            csurface.set(name = name, src = src, fileformat = fileformat, \
                       dtype = dtype, description = description)
            
            surdict[name] = csurface
            
    return surdict
