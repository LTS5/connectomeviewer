""" Multi-Modal Hierarchical Dynamic Format """

/object
    /timepoint
        /macro
            /volumes
                /volume1
                    /affine
                    /scalarfield
            /surfaces
                /surfaceset1
                    /vertices_pos
                    /connectivity,faces
                    /offset // distinguishing the different brain structures
                    /affine
                    /metadata // string coded network with node attributes
                /surfaceset2
                /labelings
                    /labeling1_onvertices
                    /labeling2_onfaces
            /denseconnectome
            /parcelconnectome
            /tractography
                /fibers
                /offset
                
            
        /micro
            /affine
            /spatial_datastructure
            /metadata // infonodes
            /geometry
                /vertices
                /connectivity
                /offset
                /colors
                /labels
                /clusters
                
        basic geometry types:
        - skeleton (=treeline)
        - connector: from, to, type, directionality, metadata // indexes into the vertices array ids
        - sphere
        - areatree
        - 2d polygons. closed and open.
        
principle: separation between semantics and geometry

open:
- atlas vs. individual
- multiple surface representation vs. canonical
- store fibers?
- brainregion: region or regionset?
- labelings? e.g. for multi-resolution
- clusterings?
- skeletons?

file
====
represents an datasets of an individual or atlas

structural
----------

/brainregions
        
    /regiongroup <of class, id>
        /attribute: JSON encoded with 1:{}, 2:{},...
        /group:surface
            /group:labelings ?
            /group:graphics
                /dataset:affine
                /dataset:aabb
                /dataset:spatialdatastructure ?
            /group:geometry
                /group:resolution_highest
                    /dataset:vertices
                    /dataset:connectivity
                    /dataset:offset
            
        /volume
            /group:geometry
                /attribute:dim,pixdim,unit,
                /group:resolution_highest
                    /dataset:3d-array
        /mesh
        /slices (with polygons)
        
        
/brainregionconnections
    /connectionmatrix (subgraph of connections)
        /mapping row index to region id
        /matrix
    /fibergroup (tractography)

/cells
    /cellgroup <of class, id> -> NeuronGroup / CellGroup Actor
        /cell-basic-geometry-types:
        /skeleton: pointset, connectivity, radius, confidence
        /mesh: pointset, faces
        /volume: e.g. labeling indices
        /areatree: closed or open polygons
        /sphere: point, radius
        -> all in contiguous array and storing corresponding integer ids (as offsets)  
        
/synapses
    /synapsegroup <of class, id>
        /synapse-basic-geometry-types:
        /connector: one-to-many / many-to-many -> store ids from cells
        /polygon (e.g. for cleft area)
        
        
dynamic
-------
path/../../vertices -> time dimension array with sampling rate
- flat list of regions and connections
- mapping of row index implicit to id, an IPv6 string for identitiy in space and time devided by point
- links. 2 pairs with 0/1 saying if it has arrow. also tri-partitite relations
- add properties for links in table columns. weights/plastic/directed