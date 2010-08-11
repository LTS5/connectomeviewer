import numpy as np
import networkx as netwx

def ttest2(X,Y):
    """ Compute the two-sided t-statistic of X,Y
    
    """
    t = np.mean(X) - np.mean(Y)
    n1 = len(X) * 1.
    n2 = len(Y) * 1.
    s = np.sqrt( ( (n1-1) * np.var(X,ddof=1) + (n2-1)*np.var(Y,ddof=1) ) / (n1+n2-2.) )
    t = t / (s*np.sqrt(1/n1+1/n2))
    return t

def compute_nbs(X, Y, THRESH, K = 1000, TAIL = 'both'):
    """ Computes the network-based statistic (NBS) as described in [1]. 
    
    Performs the NBS for populations X and Y for a
    T-statistic threshold of THRESH. The third dimension of X and Y 
    references a particular member of the populations. The first two 
    dimensions reference the connectivity value of a particular edge 
    comprising the connectivity matrix. For example, X[i,j,k] stores the 
    connectivity value corresponding to the edge between i and j for the
    kth memeber of the population. PVAL is a vector of corrected p-values 
    for each component identified. If at least one of the p-values is 
    less than 0.05, then the omnibus null hypothesis can be rejected at 
    5% significance. The null hypothesis is that the value of 
    connectivity at each edge comes from distributions of equal mean 
    between the two populations.
    
    Parameters
    ----------
    X, Y : ndarray
    
    THRES : float
        
    K : integer, default = 1000, optional.
        Enables specification of the number of
        permutations to be generated to estimate the empirical null
        distribution of maximal component size. 

    TAIL : {'equal', 'left', 'right'}, optional
        Enables specification of the type
        of alternative hypothesis to test. If TAIL:
        'equal' - alternative hypothesis is means are not equal (default)
        'left'  - mean of population X < mean of population Y
        'right' - mean of population X > mean of population Y
            
    Returns
    -------
    PVAL : ndarray
        p-values for each component
        
    ADJ : ndarray
        Returns an adjacency matrix identifying the edges comprising each component.
        Edges corresponding to the first p-value stored in the vector PVAL are assigned
        the value 1 in the adjacency matrix ADJ, edges corresponding to the second 
        p-value are assigned the value 2, etc. 
    
    NULL : ndarray
        Returns a vector of K samples 
        from the the null distribution of maximal component size. 

    ALGORITHM DESCRIPTION 
    The NBS is a nonparametric statistical test used to isolate the 
    components of an N x N undirected connectivity matrix that differ 
    significantly between two distinct populations. Each element of the 
    connectivity matrix stores a connectivity value and each member of 
    the two populations possesses a distinct connectivity matrix. A 
    component of a connectivity matrix is defined as a set of 
    interconnected edges. 

    The NBS is essentially a procedure to control the family-wise error 
    rate, in the weak sense, when the null hypothesis is tested 
    independently at each of the N(N-1)/2 edges comprising the 
    connectivity matrix. The NBS can provide greater statistical power 
    than conventional procedures for controlling the family-wise error 
    rate, such as the false discovery rate, if the set of edges at which
    the null hypothesis is rejected constitues a large component or
    components.
    The NBS comprises fours steps:
    1. Perfrom a two-sample T-test at each edge indepedently to test the
       hypothesis that the value of connectivity between the two
       populations come from distributions with equal means. 
    2. Threshold the T-statistic available at each edge to form a set of
       suprathreshold edges. 
    3. Identify any components in the adjacency matrix defined by the set
       of suprathreshold edges. These are referred to as observed 
       components. Compute the size of each observed component 
       identified; that is, the number of edges it comprises. 
    4. Repeat K times steps 1-3, each time randomly permuting members of
       the two populations and storing the size of the largest component 
       identified for each permuation. This yields an empirical estimate
       of the null distribution of maximal component size. A corrected 
       p-value for each observed component is then calculated using this
       null distribution.

    [1] Zalesky A, Fornito A, Bullmore ET (2010) Network-based statistic:
        Identifying differences in brain networks. NeuroImage.
        10.1016/j.neuroimage.2010.06.041

    Written by: Andrew Zalesky, azalesky@unimelb.edu.au
    Rewritten for Python: Stephan Gerhard, connectome@unidesign.ch

    """

    # check input matrices
    Ix,Jx,nx = X.shape
    Iy,Jy,ny = Y.shape
    
    assert Ix == Iy
    assert Jx == Jy
    assert Ix == Jx
    assert Iy == Jy
    
    # number of nodes
    N = Ix

    # Only consider elements above upper diagonal due to symmetry
    ind_mask = ( np.triu(np.ones( (N,N) ),1) == 1 )
    ind_i, ind_j = np.nonzero( np.triu(np.ones( (N,N) ),1) )
    
    # Number of edges
    M = N * (N - 1) / 2
    
    # Look up table
    ind2ij = np.zeros( (M,2) , dtype = np.int16)
    ind2ij[:,0] = ind_i
    ind2ij[:,1] = ind_j
    
    # Vectorize connectivity matrices
    # Not necessary, but may speed up indexing
    # Uses more memory since cmat temporarily replicates X
    cmat = np.zeros( (M, nx) )
    pmat = np.zeros( (M, ny) )
    for i in range(nx):
        cmat[:,i] = X[ind2ij[:,0], ind2ij[:,1],i].ravel()
    for i in range(ny):
        pmat[:,i] = Y[ind2ij[:,0], ind2ij[:,1],i].ravel()
    
    # Perform T-test at each edge
    t_stat = np.zeros( M )
    for i in range(M):
        # compute ttest2, assume independent random samples
        t = ttest2(cmat[i,:], pmat[i,:])
    
        t_stat[i] = t

    if TAIL == 'both':
        t_stat = np.abs( t_stat )
    elif TAIL == 'left':
        t_stat = -t_stat
    elif TAIL == 'right':
        pass
    else:
        raise('Tail option not recognized')
 
    # Threshold   
    ind_t = np.where( t_stat > THRESH )
    
    # Suprathreshold adjacency matrix
    ADJ = np.zeros( (N,N) )
    reledg = ind2ij[ind_t[0]] # relevant edges
    ADJ[ reledg[:,0], reledg[:,1] ] = 1 # this yields a binary matrix, selecting the edges that are above threshold
    ADJ = ADJ + ADJ.T

    # Find network components
    G = netwx.from_numpy_matrix(ADJ)
    # Return connected components as subgraphs.
    comp_list = netwx.connected_component_subgraphs(G)

    # store the number of edges for each subgraph component 
    nr_edges_per_component = np.zeros( len(comp_list) )
    nr_edges_per_component_bigenough = []
    
    for idx, componentG in enumerate(comp_list):
        nr_edges_per_component[idx] = componentG.number_of_edges()
        
        # if number of edges bigger than zero
        if nr_edges_per_component[idx] > 0:
            nr_edges_per_component_bigenough.append(nr_edges_per_component[idx])
            
            # store the number of edges of the component as value in the adjacency matrix
            for ed in componentG.edges():
                ADJ[ed[0],ed[1]] = ADJ[ed[1],ed[0]] = idx + 1
                # if we would like to store the number of edges per component
                # ADJ[ed[0],ed[1]] = ADJ[ed[1],ed[0]] = nr_edges_per_component[idx]
    
    # renaming
    sz_links = nr_edges_per_component_bigenough
    
    if len(sz_links) > 0:
        max_sz = np.max(sz_links)
    else:
        max_sz = 0        

    if False:
        # additionally, store all the components in the matrix with the value of their number of edges
        all_components = np.zeros( (N,N) )
        for idx, componentG in enumerate(comp_list):
            
            tmp_max = netwx.to_numpy_matrix( componentG , nodelist = range(N) )
            # set nonzero to number of edges
            tmp_max[tmp_max!=0.0] = componentG.number_of_edges()
            all_components[:,:] = all_components[:,:] + tmp_max
    
    print "Max component size is: %s" % max_sz
        
    # Empirically estimate null distribution of maximum component size by
    # generating K independent permutations.
    print "=====================================================" 
    print "Estimating null distribution with permutation testing"
    print "====================================================="
    
    hit=0.0
    NULL = np.zeros( (K, 1) )
    # stack matrices for permutation
    d_stacked = np.hstack( (cmat, pmat) )

    for k in range(K):
        # Randomize
        indperm = np.random.permutation( nx+ny )
        d = d_stacked[:, indperm].copy()

        #################
        
        # Perform T-test at each edge
        t_stat_perm = np.zeros( M )
        for i in range(M):
            # assume independent random samples
            t = ttest2(d[i,:nx], d[i,nx:nx+ny])
        
            t_stat_perm[i] = t
        
        if TAIL == 'both':
            t_stat_perm = np.abs( t_stat_perm )
        elif TAIL == 'left':
            t_stat_perm = -t_stat_perm
        elif TAIL == 'right':
            pass
        else:
            raise('Tail option not recognized')
     
        # Threshold   
        ind_t = np.where( t_stat_perm > THRESH )
        
        # Suprathreshold adjacency matrix
        adj_perm = np.zeros( (N,N) )
        reledg = ind2ij[ind_t[0]] # relevant edges
        adj_perm[ reledg[:,0], reledg[:,1] ] = 1 # this yields a binary matrix, selecting the edges that are above threshold
        adj_perm = adj_perm + adj_perm.T
        
        # Find network components
        G = netwx.from_numpy_matrix(adj_perm)
        # Return connected components as subgraphs.
        comp_list = netwx.connected_component_subgraphs(G)
        
        # store the number of edges for each subgraph component 
        nr_edges_per_component = np.zeros( len(comp_list) )
        for idx, componentG in enumerate(comp_list):
            nr_edges_per_component[idx] = componentG.number_of_edges()
        
        # more then one node (= at least one edge)
        nr_edges_per_component_bigenough = nr_edges_per_component[nr_edges_per_component>0]
        
        # renaming
        sz_links_perm = nr_edges_per_component_bigenough
        
        if len(sz_links_perm) > 0:
            sz_links_perm_max = np.max(sz_links_perm)
        else:
            sz_links_perm_max = 0
    
        NULL[k] = sz_links_perm_max        

        # if the component size of this random permutation is bigger than
        # the component size of the group difference computed above, this is a hit
        if NULL[k] >= max_sz:
            hit = hit + 1
            
        print "Perm %d of %d. Perm max is: %d. Observed max is: %d. P-val estimate is: %0.3f" % ((k+1),K,NULL[k],max_sz,hit/(k+1))

    # Calculate p-values for each component
    PVAL = np.zeros( len(sz_links) )
    for i in range( len(sz_links) ):
        PVAL[i] = len( NULL[NULL >= sz_links[i]] ) * 1.0 / K
        
    return (PVAL, ADJ, NULL)


def setdiff2d(X, Y):
    """ Differences of two index arrays
    
    Parameters
    ----------
    X, Y : ndarray
           (n,2) arrays representing indices
           
    Returns
    -------
    Z : ndarray
        array of elements in X, that are not in Y
    """
    n = X.shape[0]
    m = Y.shape[0]
    Z = []
    for i in xrange(n):
        test = X[i,:]
        hit = False
        for j in xrange(m):
            if test[0] == Y[j,0] and test[1] == Y[j,1]:
                hit = True
                break
        if not hit:
            Z.append(test)
    return np.array(Z)

def intersect2d(X, Y):
    """ Intersection of two index arrays
    
    Parameters
    ----------
    X, Y : ndarray
           (n,2) arrays representing indices
           
    Returns
    -------
    Z : ndarray
        array of elements in X, that are also in Y
    """
    n = X.shape[0]
    m = Y.shape[0]
    Z = []
    for i in xrange(n):
        test = X[i,:]
        hit = False
        for j in xrange(m):
            if test[0] == Y[j,0] and test[1] == Y[j,1]:
                hit = True
                break
        if hit:
            Z.append(test)
    return np.array(Z)

