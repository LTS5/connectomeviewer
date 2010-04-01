#!/usr/bin/env python

metacontent_start = """<?xml version="1.0" encoding="UTF-8"?>
<viewer xmlns="http://www.connectome.ch/2009/Connectome/xmlns"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.connectome.ch/2009/Connectome/xmlns
	                                    connectome.xsd">

<viewer-meta version="%s">
  <generator>%s</generator>
  <initial-creator>%s</initial-creator>
  <creation-date>%s</creation-date>
  <modification-date>%s</modification-date>
  <name>%s</name>
  <species>%s</species>
  <legal-notice>%s</legal-notice>
  <references>%s</references>
  <url>%s</url>
  <description>%s</description>
  <nr_of_networks>%s</nr_of_networks>
</viewer-meta>
"""
sex = ['U','F','M','M','M','F','F','M','M','F','F','M','F','M','M','F','F','M','M','F','F','F','F','F','M','F','F','M','F','F','M','M','F','F']
age = ['2','3','4','6','6','7','9','11','12','14','14','14','18','2','3','5','14','6','1.5','3','17','2','3','5','1.35','17','9','1.65','14','7','1.75','3','9','17']


metacontent_end = """
</viewer>
"""

def generate_meta_xml():
    """ Creates a meta.xml file """

    version = '1.0'
    generator = 'Handmade Python Script'
    initial_creator = 'Stephan Gerhard'
    creation_date = '16-10-2009'
    modification_date = '17-10-2009'
    name = 'Developmental Data Res83'
    species = 'Homo sapiens'
    legal_notice = 'Free for research use'
    references = 'None so far'
    url = ''
    description = 'Developmental data aquired from subjects aged between 3 and 18 years.'
    nr_of_networks = '34'

    # adding start
    out = metacontent_start % (version, generator, initial_creator, creation_date, \
                               modification_date, name, species, legal_notice, \
                               references, url, description, nr_of_networks)

    # adding networks etc.
    resolution_id = 'Resolution 83'
    
    for subject in range(1, 35):
        networkname = 'Subject ' + str(subject)
        src = 'Network/subject' + str(subject) + '.graphml'
        network_string = """
<viewer-network name="%s" src="%s" hierarchical="0" hypergraph="0" directed="0">
    <network-metadata>
        <data key="sex">%s</data>
        <data key="age">%s</data>
    </network-metadata>
    <network-surface name="Desikan-Killiany Atlas" addatlas="template_atlas_homo_sapiens_01">
       <surface-label labelid="Resolution 83" />
    </network-surface>
</viewer-network>""" % (networkname, src, sex[subject-1], age[subject-1])
        out += network_string
        

  
    # adding end
    out += metacontent_end
    
    # open file, write and close
    print out
    
if __name__ == '__main__':
    generate_meta_xml()
    
    