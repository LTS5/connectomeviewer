#!/usr/bin/env python

#
# Generated Wed May 19 11:14:00 2010 by generateDS.py version 1.20f.
#

import sys
from string import lower as str_lower
from xml.dom import minidom

import cff as supermod

#
# Globals
#

ExternalEncoding = 'ascii'

#
# Data representation classes
#

class Connectome(supermod.connectome):
    def __init__(self, connectome_meta=None, connectome_network=None, connectome_surface=None, connectome_volume=None, connectome_track=None, connectome_timeserie=None):
        supermod.connectome.__init__(self, connectome_meta, connectome_network, connectome_surface, connectome_volume, connectome_track, connectome_timeserie)
supermod.connectome.subclass = Connectome
# end class connectome


class CMetadata(supermod.AbstractCMetadata):
    def __init__(self, version=None, generator=None, initial_creator=None, institution=None, creation_date=None, modification_date=None, name=None, species=None, legal_notice=None, reference=None, url=None, description=None, metadata=None):
        supermod.AbstractCMetadata.__init__(self, version, generator, initial_creator, institution, creation_date, modification_date, name, species, legal_notice, reference, url, description, metadata)
supermod.AbstractCMetadata.subclass = CMetadata
# end class CMetadata


class CNetwork(supermod.AbstractCNetwork):
    def __init__(self, src=None, name=None, dtype=None, network_metadata=None, network_surface=None, network_volume=None, network_track=None, network_timeserie=None, description=None):
        supermod.AbstractCNetwork.__init__(self, src, name, dtype, network_metadata, network_surface, network_volume, network_track, network_timeserie, description)
supermod.AbstractCNetwork.subclass = CNetwork
# end class CNetwork


class CSurface(supermod.AbstractCSurface):
    def __init__(self, src=None, dtype=None, name=None, fileformat=None, description=None):
        supermod.AbstractCSurface.__init__(self, src, dtype, name, fileformat, description)
supermod.AbstractCSurface.subclass = CSurface
# end class CSurface


class CVolume(supermod.AbstractCVolume):
    def __init__(self, src=None, dtype=None, name=None, fileformat=None, description=None):
        supermod.AbstractCVolume.__init__(self, src, dtype, name, fileformat, description)
supermod.AbstractCVolume.subclass = CVolume
# end class CVolume


class CTrack(supermod.AbstractCTrack):
    def __init__(self, src=None, name=None, fileformat=None, description=None):
        supermod.AbstractCTrack.__init__(self, src, name, fileformat, description)
supermod.AbstractCTrack.subclass = CTrack
# end class CTrack


class CTimeserie(supermod.AbstractCTimeserie):
    def __init__(self, src=None, name=None, fileformat=None, description=None):
        supermod.AbstractCTimeserie.__init__(self, src, name, fileformat, description)
supermod.AbstractCTimeserie.subclass = CTimeserie
# end class CTimeserie


class KVPair(supermod.AbstractKVPair):
    def __init__(self, data=None):
        supermod.AbstractKVPair.__init__(self, data)
supermod.AbstractKVPair.subclass = KVPair
# end class KVPair


class data(supermod.data):
    def __init__(self, key=None, valueOf_=None):
        supermod.data.__init__(self, key, valueOf_)
supermod.data.subclass = data
# end class data


class CNetworkSurface(supermod.AbstractCNetworkSurface):
    def __init__(self, labelid=None, name=None, labelname=None, valueOf_=None):
        supermod.AbstractCNetworkSurface.__init__(self, labelid, name, labelname, valueOf_)
supermod.AbstractCNetworkSurface.subclass = CNetworkSurface
# end class CNetworkSurface


class CNetworkVolume(supermod.AbstractCNetworkVolume):
    def __init__(self, segmentationname=None, name=None, valueOf_=None):
        supermod.AbstractCNetworkVolume.__init__(self, segmentationname, name, valueOf_)
supermod.AbstractCNetworkVolume.subclass = CNetworkVolume
# end class CNetworkVolume


class CNetworkTrack(supermod.AbstractCNetworkTrack):
    def __init__(self, name=None, valueOf_=None):
        supermod.AbstractCNetworkTrack.__init__(self, name, valueOf_)
supermod.AbstractCNetworkTrack.subclass = CNetworkTrack
# end class CNetworkTrack


class CNetworkTimeserie(supermod.AbstractCNetworkTimeserie):
    def __init__(self, name=None, valueOf_=None):
        supermod.AbstractCNetworkTimeserie.__init__(self, name, valueOf_)
supermod.AbstractCNetworkTimeserie.subclass = CNetworkTimeserie
# end class CNetworkTimeserie



def parse(inFilename):
    doc = minidom.parse(inFilename)
    rootNode = doc.documentElement
    rootObj = supermod.connectome.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    sys.stdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(sys.stdout, 0, name_="connectome",
        namespacedef_='xmlns="http://www.connectomics.ch/2010/Connectome/xmlns"')
    doc = None
    return rootObj


def parseString(inString):
    doc = minidom.parseString(inString)
    rootNode = doc.documentElement
    rootObj = supermod.connectome.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    sys.stdout.write('<?xml version="1.0" ?>\n')
    rootObj.export(sys.stdout, 0, name_="connectome",
        namespacedef_='xmlns="http://www.connectomics.ch/2010/Connectome/xmlns"')
    return rootObj


def parseLiteral(inFilename):
    doc = minidom.parse(inFilename)
    rootNode = doc.documentElement
    rootObj = supermod.connectome.factory()
    rootObj.build(rootNode)
    # Enable Python to collect the space used by the DOM.
    doc = None
    sys.stdout.write('#from ??? import *\n\n')
    sys.stdout.write('import ??? as model_\n\n')
    sys.stdout.write('rootObj = model_.connectome(\n')
    rootObj.exportLiteral(sys.stdout, 0, name_="connectome")
    sys.stdout.write(')\n')
    return rootObj


USAGE_TEXT = """
Usage: python ???.py <infilename>
"""

def usage():
    print USAGE_TEXT
    sys.exit(1)


def main():
    args = sys.argv[1:]
    if len(args) != 1:
        usage()
    infilename = args[0]
    root = parse(infilename)


if __name__ == '__main__':
    #import pdb; pdb.set_trace()
    main()


