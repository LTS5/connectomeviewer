src = dragged.rendermanager.datasourcemanager.get_sourceobject()
points = nodes.mlab_source.points

# to be sent to Danny Holten
# http://www.win.tue.nl/~dholten/

# you need to drag first the network
# then you need to drag the glpyh nodes
# then set the resolution
res = '1015'

# setting z to zero
points[:,2] = 0

yoff = 40
xof = -20 + 20

idx = src.labels.index('brstRes'+res)
points[idx,0:2] = [0+xof, -60+yoff]

idx = src.labels.index('thRes'+res+'_RH')
points[idx,0:2] = [10+xof, -80+yoff]

idx = src.labels.index('thRes'+res+'_LH')
points[idx,0:2] = [-10+xof, -80+yoff]

idx = src.labels.index('hippRes'+res+'_RH')
points[idx,0:2] = [20+xof, -100+yoff]

idx = src.labels.index('hippRes'+res+'_LH')
points[idx,0:2] = [-20+xof, -100+yoff]

idx = src.labels.index('amgRes'+res+'_RH')
points[idx,0:2] = [30+xof, -120+yoff]

idx = src.labels.index('amgRes'+res+'_LH')
points[idx,0:2] = [-30+xof, -120+yoff]

xof = 20 - 20
xof2 = 30 - 20
yoff = -60

idx = src.labels.index('gpRes'+res+'_RH')
points[idx,0:2] = [10+xof, 80+yoff]

idx = src.labels.index('gpRes'+res+'_LH')
points[idx,0:2] = [-10+xof2, 80+yoff]

idx = src.labels.index('puRes'+res+'_RH')
points[idx,0:2] = [20+xof, 100+yoff]

idx = src.labels.index('puRes'+res+'_LH')
points[idx,0:2] = [-20+xof2, 100+yoff]

idx = src.labels.index('canRes'+res+'_RH')
points[idx,0:2] = [30+xof, 120+yoff]

idx = src.labels.index('canRes'+res+'_LH')
points[idx,0:2] = [-30+xof2, 120+yoff]

idx = src.labels.index('accRes'+res+'_RH')
points[idx,0:2] = [40+xof, 140+yoff]

idx = src.labels.index('accRes'+res+'_LH')
points[idx,0:2] = [-40+xof2, 140+yoff]

nodes.mlab_source.update()

#raise('End of script')

edg = src.edges
# loop over edges, remove self-loops and store to file
# given the position

# open file
fh = open('res'+res+'.edg', 'w')
for pos in range(edg.shape[0]):
    if edg[pos, 0] == edg[pos, 1]:
        continue
    a = points[edg[pos, 0],0:2]
    b = points[edg[pos, 1],0:2]
    line = ';'.join(map(str,a[0:2]))  + ';' + ';'.join(map(str,b[0:2]))
    fh.write('%s\n' % line)

fh.close()
