#!/usr/bin/env python

from scipy.io import loadmat, savemat
import numpy as np
from time import ctime
import os.path

# init variables
cur_res = '83'
subjects = ['subject1']

# pipeline variables
from pipeline import folderid2res, res

startstring = """<?xml version="1.0" encoding="UTF-8"?>

<graphml xmlns="http://graphml.graphdrawing.org/xmlns"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:xlink="http://www.w3.org/1999/xlink"

	xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns 
	http://www.connectome.ch/graphml+xlink+gifti.xsd">

<key id="dn_label" for="node" attr.name="label" attr.type="string">
	<desc>The label of the node</desc>
</key>
<key id="dn_freesurfer_structname" for="node" attr.name="fs_structname" attr.type="string">
	<desc>Derived from the labeling with an atlas of the Freesurfer pipeline</desc>
</key>
<key id="dn_intensityvalue" for="node" attr.name="intensity" attr.type="string">
	<desc>The value corresponding to the labelid in the label file for the Freesurfer atlas.</desc>
</key>
<key id="dn_colorcode" for="node" attr.name="colorcode" attr.type="string">
	<desc>The color-code: R,G,B</desc>
</key>

<key id="de_density" for="edge" attr.name="density" attr.type="double"><default>0</default>
<desc>Connection density</desc></key>

<key id="de_length" for="edge" attr.name="length" attr.type="double"><default>0</default>
<desc>Summed fiber length</desc></key>

<key id="de_adc_mean" for="edge" attr.name="adc.mean" attr.type="double"><default>0</default></key>
<key id="de_adc_std" for="edge" attr.name="adc.std" attr.type="double"><default>0</default></key>
<key id="de_adc_max" for="edge" attr.name="adc.max" attr.type="double"><default>0</default></key>

<key id="de_fa_mean" for="edge" attr.name="fa.mean" attr.type="double"><default>0</default></key>
<key id="de_fa_std" for="edge" attr.name="fa.std" attr.type="double"><default>0</default></key>
<key id="de_fa_max" for="edge" attr.name="fa.max" attr.type="double"><default>0</default></key>

<key id="de_t1_mean" for="edge" attr.name="t1.mean" attr.type="double"><default>0</default></key>
<key id="de_t1_std" for="edge" attr.name="t1.std" attr.type="double"><default>0</default></key>
<key id="de_t1_max" for="edge" attr.name="t1.max" attr.type="double"><default>0</default></key>

<graph id="Subject %s" edgedefault="undirected">
"""
endstring = """
</graph>
</graphml>
"""

def isNaN(x):
    return (x == x) == False

for subjid, subj in enumerate(subjects):
    
    print 'Calculating for subject: %s' % str(subjid + 1)

    # load all the data
    # loop through the subjects and create a graphml file
    
    print 'Loading data...'
    dsi = loadmat(os.path.join(subj + '.mat'), \
		  matlab_compatible = True)
    
    matrix = dsi['matrix'][0][0]
    
    length = matrix.length
    density = matrix.density
    adc = matrix.adc[0][0]
    fa = matrix.fa[0][0]
    t1 = matrix.t1[0][0]
    
    ###################
    # WRITE NETWORK GRAPHML FILE
    ###################
    
    graphml = open('CFF/Network/subject' + str(subjid + 1) + '.graphml', 'w')
    graphml.write(startstring % str(subjid + 1))
    
    nr_regions = length.shape[0]
    print 'Number of regions: %s' % nr_regions
    print 'Adding nodes...'
    for i in range(nr_regions):

	j = i+1

	label = res[cur_res]['label'][i]
	fs_name = res[cur_res]['fs_name'][i]
	color = res[cur_res]['color'][i]
	
	# write graphml
	nodestring = """
<node id="n%s" xlink:href="http://www.connectome.ch/wiki/%s_(Homo_sapiens)">
    <data key="dn_label">%s</data>
    <data key="dn_freesurfer_structname">%s</data>
    <data key="dn_intensityvalue">%s</data>
    <data key="dn_colorcode">%s</data>
</node>""" % (str(j), label, label, fs_name, str(j), color)

	graphml.write(nodestring)
    
    print 'Adding edges...'
    # write the connectivity
    for i in range(nr_regions):
	for j in range(nr_regions):
	    
	    if isNaN(length[i][j]) or length[i][j] == 0:
		#print 'found nan', matrix[i][j]
		continue
	    edgestring = """
<edge id="e%s_%s" source="n%s" target="n%s">
    <data key="de_density">%s</data>
    <data key="de_length">%s</data>
    <data key="de_adc_mean">%s</data>
    <data key="de_adc_std">%s</data>
    <data key="de_adc_max">%s</data>
    <data key="de_fa_mean">%s</data>
    <data key="de_fa_std">%s</data>
    <data key="de_fa_max">%s</data>
    <data key="de_t1_mean">%s</data>
    <data key="de_t1_std">%s</data>
    <data key="de_t1_max">%s</data>
</edge>""" % (str(i+1), str(j+1), str(i+1), str(j+1), \
	      str(density[i][j]),
	      str(length[i][j]),
	      str(adc.mean[i][j]),
	      str(adc.std[i][j]),
	      str(adc.max[i][j]),
	      str(fa.mean[i][j]),
	      str(fa.std[i][j]),
	      str(fa.max[i][j]),
	      str(t1.mean[i][j]),
	      str(t1.std[i][j]),
	      str(t1.max[i][j]),
	    )
	    graphml.write(edgestring)
    
    graphml.write(endstring)
    graphml.close()
    print 'Finished.'
