.. _nbs:

=============================================
Network Based Statistics for Group Comparison
=============================================

What is NBS?
------------
The Network Based Statistics (NBS) is a nonparametric statistical
test used to isolate the 
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

#. Perfrom a two-sample T-test at each edge indepedently to test the hypothesis that the value of connectivity between the two populations come from distributions with equal means. 
#. Threshold the T-statistic available at each edge to form a set of suprathreshold edges. 
#. Identify any components in the adjacency matrix defined by the set of suprathreshold edges. These are referred to as observed  components. Compute the size of each observed component  identified; that is, the number of edges it comprises. 
#. Repeat K times steps 1-3, each time randomly permuting members of
   the two populations and storing the size of the largest component 
   identified for each permuation. This yields an empirical estimate
   of the null distribution of maximal component size. A corrected 
   p-value for each observed component is then calculated using this
   null distribution.

[1] Zalesky A, Fornito A, Bullmore ET (2010) Network-based statistic: Identifying differences in brain networks. NeuroImage. 10.1016/j.neuroimage.2010.06.041

Original code written by Andrew Zalesky, rewritten for Python by Stephan Gerhard.
    
How to prepare the data?
------------------------
The data preparation steps is explained using Matlab matrices or NumPy arrays.
In a future version of ConnectomeViewer, it will be easier to directly group network from a loaded connectome file.

We need two input matrices for the NBS algorithm. These have dimension N x N x K, where N is the number
of nodes in the network, and K is the number of subjects in the group respectively.

The full working script and example data is available in the *examples/nbs/* folder in the source distribution.

Make the necessary imports::

	import numpy as np
	import cviewer.libs.pyconto.algorithms.statistics.nbs as nbs
	from pylab import imshow, show, title

If you are using nbs from within the ConnectomeViewer IPython Shell, you can directly use nbs without importing.
It is exposed through loading of the NBS plugin on startup.

Steps to import the Matlab matrices (you might need to adapt the path)::

	import scipy.io as io
	Xmat = io.loadmat('X.mat')
	Ymat = io.loadmat('Y.mat')
	X = Xmat['X']
	Y = Ymat['Y']
	n = X.shape[0]

The Matlab matrices are converted in Numpy arrays. Of course, you can start directly from here if you
have your matrices already in Numpy arrays.

Compute the NBS
---------------
We need to set a few parameters to invoke the NBS algorithm. Read the docstring and some more hints below::
	
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
	    """
	    
For our purpose, the following parametes need to be set.

Run the NBS with the following parameter options: 
Set an appropriate threshold. It is difficult to provide a rule of thumb 
to guide the choice of this threshold. Trial-and-error is always an option
with the number of permutations generated per trial set low.:: 

	THRESH=3
	 
Generate 1000 permutations. Many more permutations are required in practice
to yield a reliable estimate (e.g. 5000).::
 
	K=1000

Set TAIL to left, and thus test the alternative hypothesis that mean of 
population X < mean of population Y::

	TAIL='left'

Run the NBS::

PVAL, ADJ, NULL = nbs.compute_nbs(X,Y,THRESH,K,TAIL)

Visualize the results (components)
----------------------------------

First, we use matplotlib to look at the connectivity matrix output that should
contain the (enumerated) components found.

We can print the results for PVAL and NULL::

	print "pval", PVAL
	print "null", NULL

And also show the connectivity matrix with::

	imshow(ADJ, interpolation='nearest')
	title('Edges identified by the NBS')
	show()

To show the results in 3D, we need to position the nodes of course. If you have a position
Numpy array, perfect. It should have dimensions N x 3 of course, where N is the number of nodes.
If you do not have such information, you can also apply a graph layouting algorithm. See the corresponding
tutorial, :doc:`tut_graphlayout`.

I assume the position Numpy array is *P*. Then, you can simply do::

	cfile.add_network_from_matrix_with_pos(name='Identified edges NBS', matrix = ADJ, pos = P, directed = False)
	cfile.networks[0].active = True
	cfile.networks[0].select_all()
 
Now, you can apply the edges threshold (Right-click on the network and select *Edge Parameters*) to look at the
individual components separately.

