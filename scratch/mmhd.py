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
        