# This file was automatically generated by SWIG (http://www.swig.org).
# Version 1.3.40
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.
# This file is compatible with both classic and new-style classes.

from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_bct', [dirname(__file__)])
        except ImportError:
            import _bct
            return _bct
        if fp is not None:
            try:
                _mod = imp.load_module('_bct', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _bct = swig_import_helper()
    del swig_import_helper
else:
    import _bct
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0



def assortativity_dir(*args):
  """assortativity_dir(gsl_matrix CIJ) -> double"""
  return _bct.assortativity_dir(*args)

def assortativity_und(*args):
  """assortativity_und(gsl_matrix CIJ) -> double"""
  return _bct.assortativity_und(*args)

def degrees_dir(*args):
  """degrees_dir(gsl_matrix CIJ) -> gsl_vector"""
  return _bct.degrees_dir(*args)

def degrees_und(*args):
  """degrees_und(gsl_matrix CIJ) -> gsl_vector"""
  return _bct.degrees_und(*args)

def density_dir(*args):
  """density_dir(gsl_matrix CIJ) -> double"""
  return _bct.density_dir(*args)

def density_und(*args):
  """density_und(gsl_matrix CIJ) -> double"""
  return _bct.density_und(*args)

def jdegree(*args):
  """jdegree(gsl_matrix CIJ) -> gsl_matrix"""
  return _bct.jdegree(*args)

def jdegree_bl(*args):
  """jdegree_bl(gsl_matrix J) -> int"""
  return _bct.jdegree_bl(*args)

def jdegree_id(*args):
  """jdegree_id(gsl_matrix J) -> int"""
  return _bct.jdegree_id(*args)

def jdegree_od(*args):
  """jdegree_od(gsl_matrix J) -> int"""
  return _bct.jdegree_od(*args)

def matching_ind(*args):
  """matching_ind(gsl_matrix CIJ) -> gsl_matrix"""
  return _bct.matching_ind(*args)

def matching_ind_in(*args):
  """matching_ind_in(gsl_matrix CIJ) -> gsl_matrix"""
  return _bct.matching_ind_in(*args)

def matching_ind_out(*args):
  """matching_ind_out(gsl_matrix CIJ) -> gsl_matrix"""
  return _bct.matching_ind_out(*args)

def strengths_dir(*args):
  """strengths_dir(gsl_matrix CIJ) -> gsl_vector"""
  return _bct.strengths_dir(*args)

def strengths_und(*args):
  """strengths_und(gsl_matrix CIJ) -> gsl_vector"""
  return _bct.strengths_und(*args)

def clustering_coef_bd(*args):
  """clustering_coef_bd(gsl_matrix A) -> gsl_vector"""
  return _bct.clustering_coef_bd(*args)

def clustering_coef_bu(*args):
  """clustering_coef_bu(gsl_matrix G) -> gsl_vector"""
  return _bct.clustering_coef_bu(*args)

def clustering_coef_wd(*args):
  """clustering_coef_wd(gsl_matrix W) -> gsl_vector"""
  return _bct.clustering_coef_wd(*args)

def clustering_coef_wu(*args):
  """clustering_coef_wu(gsl_matrix W) -> gsl_vector"""
  return _bct.clustering_coef_wu(*args)

def efficiency_local(*args):
  """efficiency_local(gsl_matrix G) -> gsl_vector"""
  return _bct.efficiency_local(*args)

def breadth(*args):
  """breadth(gsl_matrix CIJ, int source) -> gsl_vector"""
  return _bct.breadth(*args)

def breadthdist(*args):
  """breadthdist(gsl_matrix CIJ) -> gsl_matrix"""
  return _bct.breadthdist(*args)

def charpath_ecc(*args):
  """charpath_ecc(gsl_matrix D) -> gsl_vector"""
  return _bct.charpath_ecc(*args)

def charpath_lambda(*args):
  """charpath_lambda(gsl_matrix D) -> double"""
  return _bct.charpath_lambda(*args)

def connectivity_length(*args):
  """connectivity_length(gsl_matrix D) -> double"""
  return _bct.connectivity_length(*args)

def cycprob_fcyc(*args):
  """cycprob_fcyc(std::vector<(p.gsl_matrix)> Pq) -> gsl_vector"""
  return _bct.cycprob_fcyc(*args)

def cycprob_pcyc(*args):
  """cycprob_pcyc(std::vector<(p.gsl_matrix)> Pq) -> gsl_vector"""
  return _bct.cycprob_pcyc(*args)

def distance_bin(*args):
  """distance_bin(gsl_matrix G) -> gsl_matrix"""
  return _bct.distance_bin(*args)

def distance_wei(*args):
  """distance_wei(gsl_matrix G) -> gsl_matrix"""
  return _bct.distance_wei(*args)

def efficiency_global(*args):
  """efficiency_global(gsl_matrix G) -> gsl_matrix"""
  return _bct.efficiency_global(*args)

def findpaths(*args):
  """findpaths(gsl_matrix CIJ, gsl_vector sources, int qmax) -> std::vector<(p.gsl_matrix)>"""
  return _bct.findpaths(*args)

def findwalks(*args):
  """findwalks(gsl_matrix CIJ) -> std::vector<(p.gsl_matrix)>"""
  return _bct.findwalks(*args)

def reachdist(*args):
  """reachdist(gsl_matrix CIJ) -> gsl_matrix"""
  return _bct.reachdist(*args)

def betweenness_bin(*args):
  """betweenness_bin(gsl_matrix G) -> gsl_vector"""
  return _bct.betweenness_bin(*args)

def betweenness_wei(*args):
  """betweenness_wei(gsl_matrix G) -> gsl_vector"""
  return _bct.betweenness_wei(*args)

def edge_betweenness_bin(*args):
  """edge_betweenness_bin(gsl_matrix G) -> gsl_matrix"""
  return _bct.edge_betweenness_bin(*args)

def edge_betweenness_wei(*args):
  """edge_betweenness_wei(gsl_matrix G) -> gsl_matrix"""
  return _bct.edge_betweenness_wei(*args)

def erange(*args):
  """erange(gsl_matrix CIJ) -> gsl_matrix"""
  return _bct.erange(*args)
MILO = _bct.MILO
SPORNS = _bct.SPORNS

def get_motif_mode():
  """get_motif_mode() -> motif_mode_enum"""
  return _bct.get_motif_mode()

def set_motif_mode(*args):
  """set_motif_mode(motif_mode_enum motif_mode)"""
  return _bct.set_motif_mode(*args)

def motif3funct_bin(*args):
  """motif3funct_bin(gsl_matrix W) -> gsl_vector"""
  return _bct.motif3funct_bin(*args)

def motif3funct_wei(*args):
  """motif3funct_wei(gsl_matrix W) -> gsl_matrix"""
  return _bct.motif3funct_wei(*args)

def motif3funct_wei_v(*args):
  """motif3funct_wei_v(gsl_matrix W) -> gsl_vector"""
  return _bct.motif3funct_wei_v(*args)

def motif3generate():
  """motif3generate() -> gsl_matrix"""
  return _bct.motif3generate()

def motif3struct_bin(*args):
  """motif3struct_bin(gsl_matrix A) -> gsl_vector"""
  return _bct.motif3struct_bin(*args)

def motif3struct_wei(*args):
  """motif3struct_wei(gsl_matrix W) -> gsl_matrix"""
  return _bct.motif3struct_wei(*args)

def motif3struct_wei_v(*args):
  """motif3struct_wei_v(gsl_matrix W) -> gsl_vector"""
  return _bct.motif3struct_wei_v(*args)

def motif4generate():
  """motif4generate() -> gsl_matrix"""
  return _bct.motif4generate()

def motif4funct_bin(*args):
  """motif4funct_bin(gsl_matrix W) -> gsl_vector"""
  return _bct.motif4funct_bin(*args)

def motif4funct_wei(*args):
  """motif4funct_wei(gsl_matrix W) -> gsl_matrix"""
  return _bct.motif4funct_wei(*args)

def motif4funct_wei_v(*args):
  """motif4funct_wei_v(gsl_matrix W) -> gsl_vector"""
  return _bct.motif4funct_wei_v(*args)

def motif4struct_bin(*args):
  """motif4struct_bin(gsl_matrix A) -> gsl_vector"""
  return _bct.motif4struct_bin(*args)

def motif4struct_wei(*args):
  """motif4struct_wei(gsl_matrix W) -> gsl_matrix"""
  return _bct.motif4struct_wei(*args)

def motif4struct_wei_v(*args):
  """motif4struct_wei_v(gsl_matrix W) -> gsl_vector"""
  return _bct.motif4struct_wei_v(*args)

def modularity_dir(*args):
  """modularity_dir(gsl_matrix A) -> double"""
  return _bct.modularity_dir(*args)

def modularity_und(*args):
  """modularity_und(gsl_matrix A) -> double"""
  return _bct.modularity_und(*args)

def module_degree_zscore(*args):
  """module_degree_zscore(gsl_matrix A, gsl_vector Ci) -> gsl_vector"""
  return _bct.module_degree_zscore(*args)

def participation_coef(*args):
  """participation_coef(gsl_matrix A, gsl_vector Ci) -> gsl_vector"""
  return _bct.participation_coef(*args)

def makeevenCIJ(*args):
  """makeevenCIJ(int N, int K, int sz_cl) -> gsl_matrix"""
  return _bct.makeevenCIJ(*args)

def makefractalCIJ(*args):
  """makefractalCIJ(int mx_lvl, double E, int sz_cl) -> gsl_matrix"""
  return _bct.makefractalCIJ(*args)

def makelatticeCIJ(*args):
  """makelatticeCIJ(int N, int K) -> gsl_matrix"""
  return _bct.makelatticeCIJ(*args)

def makerandCIJ_bd(*args):
  """makerandCIJ_bd(int N, int K) -> gsl_matrix"""
  return _bct.makerandCIJ_bd(*args)

def makerandCIJ_bu(*args):
  """makerandCIJ_bu(int N, int K) -> gsl_matrix"""
  return _bct.makerandCIJ_bu(*args)

def makerandCIJ_wd(*args):
  """makerandCIJ_wd(int N, int K, double wmin, double wmax) -> gsl_matrix"""
  return _bct.makerandCIJ_wd(*args)

def makerandCIJ_wd_wp(*args):
  """makerandCIJ_wd_wp(gsl_matrix m) -> gsl_matrix"""
  return _bct.makerandCIJ_wd_wp(*args)

def makerandCIJ_wu(*args):
  """makerandCIJ_wu(int N, int K, double wmin, double wmax) -> gsl_matrix"""
  return _bct.makerandCIJ_wu(*args)

def makerandCIJ_wu_wp(*args):
  """makerandCIJ_wu_wp(gsl_matrix m) -> gsl_matrix"""
  return _bct.makerandCIJ_wu_wp(*args)

def makeringlatticeCIJ(*args):
  """makeringlatticeCIJ(int N, int K) -> gsl_matrix"""
  return _bct.makeringlatticeCIJ(*args)

def maketoeplitzCIJ(*args):
  """maketoeplitzCIJ(int N, int K, double s) -> gsl_matrix"""
  return _bct.maketoeplitzCIJ(*args)

def latmio_dir(*args):
  """latmio_dir(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.latmio_dir(*args)

def latmio_dir_connected(*args):
  """latmio_dir_connected(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.latmio_dir_connected(*args)

def latmio_und(*args):
  """latmio_und(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.latmio_und(*args)

def latmio_und_connected(*args):
  """latmio_und_connected(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.latmio_und_connected(*args)

def randmio_dir(*args):
  """randmio_dir(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.randmio_dir(*args)

def randmio_dir_connected(*args):
  """randmio_dir_connected(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.randmio_dir_connected(*args)

def randmio_und(*args):
  """randmio_und(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.randmio_und(*args)

def randmio_und_connected(*args):
  """randmio_und_connected(gsl_matrix R, int ITER) -> gsl_matrix"""
  return _bct.randmio_und_connected(*args)
SQUARE = _bct.SQUARE
RECTANGULAR = _bct.RECTANGULAR
UNDIRECTED = _bct.UNDIRECTED
DIRECTED = _bct.DIRECTED
BINARY = _bct.BINARY
WEIGHTED = _bct.WEIGHTED
POSITIVE = _bct.POSITIVE
SIGNED = _bct.SIGNED
NO_LOOPS = _bct.NO_LOOPS
LOOPS = _bct.LOOPS

def get_safe_mode():
  """get_safe_mode() -> bool"""
  return _bct.get_safe_mode()

def set_safe_mode(*args):
  """set_safe_mode(bool safe_mode)"""
  return _bct.set_safe_mode(*args)

def check_status(*args):
  """check_status(gsl_matrix m, int flags, string text) -> bool"""
  return _bct.check_status(*args)

def is_square(*args):
  """is_square(gsl_matrix m) -> bool"""
  return _bct.is_square(*args)

def is_rectangular(*args):
  """is_rectangular(gsl_matrix m) -> bool"""
  return _bct.is_rectangular(*args)

def is_undirected(*args):
  """is_undirected(gsl_matrix m) -> bool"""
  return _bct.is_undirected(*args)

def is_directed(*args):
  """is_directed(gsl_matrix m) -> bool"""
  return _bct.is_directed(*args)

def is_binary(*args):
  """is_binary(gsl_matrix m) -> bool"""
  return _bct.is_binary(*args)

def is_weighted(*args):
  """is_weighted(gsl_matrix m) -> bool"""
  return _bct.is_weighted(*args)

def is_positive(*args):
  """is_positive(gsl_matrix m) -> bool"""
  return _bct.is_positive(*args)

def is_signed(*args):
  """is_signed(gsl_matrix m) -> bool"""
  return _bct.is_signed(*args)

def has_loops(*args):
  """has_loops(gsl_matrix m) -> bool"""
  return _bct.has_loops(*args)

def has_no_loops(*args):
  """has_no_loops(gsl_matrix m) -> bool"""
  return _bct.has_no_loops(*args)

def invert_elements(*args):
  """invert_elements(gsl_matrix m) -> gsl_matrix"""
  return _bct.invert_elements(*args)

def remove_loops(*args):
  """remove_loops(gsl_matrix m) -> gsl_matrix"""
  return _bct.remove_loops(*args)

def to_binary(*args):
  """to_binary(gsl_matrix m) -> gsl_matrix"""
  return _bct.to_binary(*args)

def to_positive(*args):
  """to_positive(gsl_matrix m) -> gsl_matrix"""
  return _bct.to_positive(*args)

def to_undirected_bin(*args):
  """to_undirected_bin(gsl_matrix m) -> gsl_matrix"""
  return _bct.to_undirected_bin(*args)

def to_undirected_wei(*args):
  """to_undirected_wei(gsl_matrix m) -> gsl_matrix"""
  return _bct.to_undirected_wei(*args)

def gsl_error_handler(*args):
  """gsl_error_handler(char reason, char file, int line, int gsl_errno)"""
  return _bct.gsl_error_handler(*args)

def init():
  """init()"""
  return _bct.init()

def number_of_edges_dir(*args):
  """number_of_edges_dir(gsl_matrix m) -> int"""
  return _bct.number_of_edges_dir(*args)

def number_of_edges_und(*args):
  """number_of_edges_und(gsl_matrix m) -> int"""
  return _bct.number_of_edges_und(*args)

def number_of_nodes(*args):
  """number_of_nodes(gsl_matrix m) -> int"""
  return _bct.number_of_nodes(*args)

def threshold_absolute(*args):
  """threshold_absolute(gsl_matrix W, double thr) -> gsl_matrix"""
  return _bct.threshold_absolute(*args)

def threshold_proportional_dir(*args):
  """threshold_proportional_dir(gsl_matrix W, double p) -> gsl_matrix"""
  return _bct.threshold_proportional_dir(*args)

def threshold_proportional_und(*args):
  """threshold_proportional_und(gsl_matrix W, double p) -> gsl_matrix"""
  return _bct.threshold_proportional_und(*args)

def to_gsl3dm(*args):
  """to_gsl3dm(PyObject list) -> std::vector<(p.gsl_matrix)>"""
  return _bct.to_gsl3dm(*args)

def inv(*args):
  """inv(gsl_matrix m) -> gsl_matrix"""
  return _bct.inv(*args)

def randperm(*args):
  """randperm(int size) -> gsl_permutation"""
  return _bct.randperm(*args)

def div_left(*args):
  """div_left(gsl_matrix m1, gsl_matrix m2) -> gsl_matrix"""
  return _bct.div_left(*args)

def div_right(*args):
  """div_right(gsl_matrix m1, gsl_matrix m2) -> gsl_matrix"""
  return _bct.div_right(*args)

def get_gsl_rng():
  """get_gsl_rng() -> gsl_rng"""
  return _bct.get_gsl_rng()

def seed_rng(*args):
  """seed_rng(gsl_rng rng, unsigned long seed)"""
  return _bct.seed_rng(*args)

def nonzeros(*args):
  """nonzeros(gsl_matrix m) -> gsl_vector"""
  return _bct.nonzeros(*args)

def normpdf(*args):
  """normpdf(gsl_vector v, double mean, double stdev) -> gsl_vector"""
  return _bct.normpdf(*args)

def ones_vector_double(*args):
  """ones_vector_double(int size) -> gsl_vector"""
  return _bct.ones_vector_double(*args)

def rand_vector_double(*args):
  """rand_vector_double(int size) -> gsl_vector"""
  return _bct.rand_vector_double(*args)

def reverse(*args):
  """reverse(gsl_vector v) -> gsl_vector"""
  return _bct.reverse(*args)

def setxor(*args):
  """setxor(gsl_vector v1, gsl_vector v2) -> gsl_vector"""
  return _bct.setxor(*args)

def unique_rows(*args):
  """unique_rows(gsl_matrix m, string first_or_last) -> gsl_matrix"""
  return _bct.unique_rows(*args)

def zeros_vector_double(*args):
  """zeros_vector_double(int size) -> gsl_vector"""
  return _bct.zeros_vector_double(*args)

def mul(*args):
  """mul(gsl_matrix m1, gsl_matrix m2) -> gsl_matrix"""
  return _bct.mul(*args)

def pow(*args):
  """pow(gsl_matrix m, int power) -> gsl_matrix"""
  return _bct.pow(*args)

def fp_compare(*args):
  """fp_compare(double x, double y) -> int"""
  return _bct.fp_compare(*args)

def fp_zero(*args):
  """fp_zero(double x) -> bool"""
  return _bct.fp_zero(*args)

def fp_nonzero(*args):
  """fp_nonzero(double x) -> bool"""
  return _bct.fp_nonzero(*args)

def fp_equal(*args):
  """fp_equal(double x, double y) -> bool"""
  return _bct.fp_equal(*args)

def fp_not_equal(*args):
  """fp_not_equal(double x, double y) -> bool"""
  return _bct.fp_not_equal(*args)

def fp_less(*args):
  """fp_less(double x, double y) -> bool"""
  return _bct.fp_less(*args)

def fp_less_or_equal(*args):
  """fp_less_or_equal(double x, double y) -> bool"""
  return _bct.fp_less_or_equal(*args)

def fp_greater(*args):
  """fp_greater(double x, double y) -> bool"""
  return _bct.fp_greater(*args)

def fp_greater_or_equal(*args):
  """fp_greater_or_equal(double x, double y) -> bool"""
  return _bct.fp_greater_or_equal(*args)

def compare_vectors(*args):
  """compare_vectors(gsl_vector v1, gsl_vector v2) -> int"""
  return _bct.compare_vectors(*args)

def vector_less(*args):
  """vector_less(gsl_vector v1, gsl_vector v2) -> bool"""
  return _bct.vector_less(*args)

def compare_matrices(*args):
  """compare_matrices(gsl_matrix m1, gsl_matrix m2) -> int"""
  return _bct.compare_matrices(*args)

def matrix_less(*args):
  """matrix_less(gsl_matrix m1, gsl_matrix m2) -> bool"""
  return _bct.matrix_less(*args)

def ord_log_index(*args):
  """ord_log_index(gsl_matrix m, gsl_vector rows, gsl_vector logical_columns) -> gsl_matrix"""
  return _bct.ord_log_index(*args)

def log_ord_index(*args):
  """log_ord_index(gsl_matrix m, gsl_vector logical_rows, gsl_vector columns) -> gsl_matrix"""
  return _bct.log_ord_index(*args)

def to_array(*args):
  """to_array(gsl_vector v, double array)"""
  return _bct.to_array(*args)

def to_vector_float(*args):
  """to_vector_float(gsl_vector v) -> gsl_vector_float"""
  return _bct.to_vector_float(*args)

def to_vector_long_double(*args):
  """to_vector_long_double(gsl_vector v) -> gsl_vector_long_double"""
  return _bct.to_vector_long_double(*args)

def to_vector(*args):
  """to_vector(gsl_matrix m) -> gsl_vector"""
  return _bct.to_vector(*args)

def to_matrix_float(*args):
  """to_matrix_float(gsl_matrix m) -> gsl_matrix_float"""
  return _bct.to_matrix_float(*args)

def to_matrix_double(*args):
  """to_matrix_double(gsl_matrix m) -> gsl_matrix"""
  return _bct.to_matrix_double(*args)

def to_matrix_long_double(*args):
  """to_matrix_long_double(gsl_matrix m) -> gsl_matrix_long_double"""
  return _bct.to_matrix_long_double(*args)

def to_column_matrix(*args):
  """to_column_matrix(gsl_vector v) -> gsl_matrix"""
  return _bct.to_column_matrix(*args)

def to_row_matrix(*args):
  """to_row_matrix(gsl_vector v) -> gsl_matrix"""
  return _bct.to_row_matrix(*args)

def to_permutation(*args):
  """to_permutation(gsl_vector v) -> gsl_permutation"""
  return _bct.to_permutation(*args)

def permute_columns(*args):
  """permute_columns(gsl_permutation p, gsl_matrix m) -> gsl_matrix"""
  return _bct.permute_columns(*args)

def permute_rows(*args):
  """permute_rows(gsl_permutation p, gsl_matrix m) -> gsl_matrix"""
  return _bct.permute_rows(*args)


def normalized_path_length(*args):
  """
    normalized_path_length(gsl_matrix D, double wmax = 1.0) -> double
    normalized_path_length(gsl_matrix D) -> double
    """
  return _bct.normalized_path_length(*args)
cvar = _bct.cvar

def find_motif34(*args):
  """
    find_motif34(int m, int n) -> std::vector<(p.gsl_matrix)>
    find_motif34(gsl_matrix m) -> int
    """
  return _bct.find_motif34(*args)

def modularity_und_louvain(*args):
  """
    modularity_und_louvain(gsl_matrix W, int N = 100) -> double
    modularity_und_louvain(gsl_matrix W) -> double
    """
  return _bct.modularity_und_louvain(*args)

def makerandCIJdegreesfixed(*args):
  """
    makerandCIJdegreesfixed(gsl_vector _in, gsl_vector out) -> gsl_matrix
    makerandCIJdegreesfixed(gsl_matrix m) -> gsl_matrix
    """
  return _bct.makerandCIJdegreesfixed(*args)
cat_all = cvar.cat_all
cat_ctx = cvar.cat_ctx
fve30 = cvar.fve30
fve32 = cvar.fve32
macaque47 = cvar.macaque47
macaque71 = cvar.macaque71

def gsl_free(*args):
  """
    gsl_free(gsl_vector v)
    gsl_free(gsl_matrix m)
    gsl_free(std::vector<(p.gsl_matrix)> m)
    """
  return _bct.gsl_free(*args)

def printf(*args):
  """
    printf(gsl_vector v, string format)
    printf(gsl_matrix m, string format)
    printf(gsl_permutation p, string format)
    """
  return _bct.printf(*args)

def from_gsl(*args):
  """
    from_gsl(gsl_vector v) -> PyObject
    from_gsl(gsl_matrix m) -> PyObject
    from_gsl(std::vector<(p.gsl_matrix)> m) -> PyObject
    """
  return _bct.from_gsl(*args)

def to_gslv(*args):
  """
    to_gslv(double array, int size) -> gsl_vector
    to_gslv(PyObject list) -> gsl_vector
    """
  return _bct.to_gslv(*args)

def to_gslm(*args):
  """
    to_gslm(double array, int size1, int size2) -> gsl_matrix
    to_gslm(PyObject list) -> gsl_matrix
    """
  return _bct.to_gslm(*args)

def dec2bin(*args):
  """
    dec2bin(int n) -> string
    dec2bin(int n, int len) -> string
    """
  return _bct.dec2bin(*args)

def abs(*args):
  """
    abs(gsl_vector v) -> gsl_vector
    abs(gsl_matrix m) -> gsl_matrix
    """
  return _bct.abs(*args)

def all(*args):
  """
    all(gsl_vector v) -> int
    all(gsl_matrix m, int dim = 1) -> gsl_vector
    all(gsl_matrix m) -> gsl_vector
    """
  return _bct.all(*args)

def any(*args):
  """
    any(gsl_vector v) -> int
    any(gsl_matrix m, int dim = 1) -> gsl_vector
    any(gsl_matrix m) -> gsl_vector
    """
  return _bct.any(*args)

def diag(*args):
  """
    diag(gsl_vector v, int k = 0) -> gsl_matrix
    diag(gsl_vector v) -> gsl_matrix
    diag(gsl_matrix m, int k = 0) -> gsl_vector
    diag(gsl_matrix m) -> gsl_vector
    """
  return _bct.diag(*args)

def eye_double(*args):
  """
    eye_double(int size) -> gsl_matrix
    eye_double(int size1, int size2) -> gsl_matrix
    """
  return _bct.eye_double(*args)

def find(*args):
  """
    find(gsl_vector v, int n = std::numeric_limits< int >::max(), 
        string direction = "first") -> gsl_vector
    find(gsl_vector v, int n = std::numeric_limits< int >::max()) -> gsl_vector
    find(gsl_vector v) -> gsl_vector
    find(gsl_matrix m, int n = std::numeric_limits< int >::max(), 
        string direction = "first") -> gsl_vector
    find(gsl_matrix m, int n = std::numeric_limits< int >::max()) -> gsl_vector
    find(gsl_matrix m) -> gsl_vector
    """
  return _bct.find(*args)

def find_ij(*args):
  """
    find_ij(gsl_matrix m, int n = std::numeric_limits< int >::max(), 
        string direction = "first") -> gsl_matrix
    find_ij(gsl_matrix m, int n = std::numeric_limits< int >::max()) -> gsl_matrix
    find_ij(gsl_matrix m) -> gsl_matrix
    """
  return _bct.find_ij(*args)

def hist(*args):
  """
    hist(gsl_vector v, int n = 10) -> gsl_vector
    hist(gsl_vector v) -> gsl_vector
    hist(gsl_vector v, gsl_vector centers) -> gsl_vector
    """
  return _bct.hist(*args)

def length(*args):
  """
    length(gsl_vector v) -> int
    length(gsl_matrix m) -> int
    """
  return _bct.length(*args)

def max(*args):
  """
    max(double x, double y) -> double
    max(gsl_vector v) -> double
    max(gsl_matrix m, int dim = 1) -> gsl_vector
    max(gsl_matrix m) -> gsl_vector
    """
  return _bct.max(*args)

def mean(*args):
  """
    mean(gsl_vector v, string opt = "a") -> double
    mean(gsl_vector v) -> double
    mean(gsl_matrix m, int dim = 1, string opt = "a") -> gsl_vector
    mean(gsl_matrix m, int dim = 1) -> gsl_vector
    mean(gsl_matrix m) -> gsl_vector
    """
  return _bct.mean(*args)

def min(*args):
  """
    min(double x, double y) -> double
    min(gsl_vector v) -> double
    min(gsl_matrix m, int dim = 1) -> gsl_vector
    min(gsl_matrix m) -> gsl_vector
    """
  return _bct.min(*args)

def nnz(*args):
  """
    nnz(gsl_vector v) -> int
    nnz(gsl_matrix m) -> int
    """
  return _bct.nnz(*args)

def ones_double(*args):
  """
    ones_double(int size) -> gsl_matrix
    ones_double(int size1, int size2) -> gsl_matrix
    """
  return _bct.ones_double(*args)

def prod(*args):
  """
    prod(gsl_vector v) -> double
    prod(gsl_matrix m, int dim = 1) -> gsl_vector
    prod(gsl_matrix m) -> gsl_vector
    """
  return _bct.prod(*args)

def rand_double(*args):
  """
    rand_double(int size) -> gsl_matrix
    rand_double(int size1, int size2) -> gsl_matrix
    """
  return _bct.rand_double(*args)

def sort(*args):
  """
    sort(gsl_vector v, string mode) -> gsl_vector
    sort(gsl_matrix m, int dim, string mode) -> gsl_matrix
    """
  return _bct.sort(*args)

def sortrows(*args):
  """
    sortrows(gsl_vector v) -> gsl_vector
    sortrows(gsl_matrix m) -> gsl_matrix
    """
  return _bct.sortrows(*args)

def std(*args):
  """
    std(gsl_vector v, int opt = 0) -> double
    std(gsl_vector v) -> double
    std(gsl_matrix m, int opt = 0, int dim = 1) -> gsl_vector
    std(gsl_matrix m, int opt = 0) -> gsl_vector
    std(gsl_matrix m) -> gsl_vector
    """
  return _bct.std(*args)

def sum(*args):
  """
    sum(gsl_vector v) -> double
    sum(gsl_matrix m, int dim = 1) -> gsl_vector
    sum(gsl_matrix m) -> gsl_vector
    """
  return _bct.sum(*args)

def toeplitz(*args):
  """
    toeplitz(gsl_vector column, gsl_vector row = None) -> gsl_matrix
    toeplitz(gsl_vector column) -> gsl_matrix
    """
  return _bct.toeplitz(*args)

def tril(*args):
  """
    tril(gsl_matrix m, int k = 0) -> gsl_matrix
    tril(gsl_matrix m) -> gsl_matrix
    """
  return _bct.tril(*args)

def triu(*args):
  """
    triu(gsl_matrix m, int k = 0) -> gsl_matrix
    triu(gsl_matrix m) -> gsl_matrix
    """
  return _bct.triu(*args)

def unique(*args):
  """
    unique(gsl_vector v, string first_or_last) -> gsl_vector
    unique(gsl_matrix m, string first_or_last) -> gsl_vector
    """
  return _bct.unique(*args)

def zeros_double(*args):
  """
    zeros_double(int size) -> gsl_matrix
    zeros_double(int size1, int size2) -> gsl_matrix
    """
  return _bct.zeros_double(*args)

def concatenate(*args):
  """
    concatenate(gsl_vector v, double x) -> gsl_vector
    concatenate(double x, gsl_vector v) -> gsl_vector
    concatenate(gsl_vector v1, gsl_vector v2) -> gsl_vector
    """
  return _bct.concatenate(*args)

def concatenate_columns(*args):
  """
    concatenate_columns(gsl_vector v1, gsl_vector v2) -> gsl_matrix
    concatenate_columns(gsl_matrix m, gsl_vector v) -> gsl_matrix
    concatenate_columns(gsl_vector v, gsl_matrix m) -> gsl_matrix
    concatenate_columns(gsl_matrix m1, gsl_matrix m2) -> gsl_matrix
    """
  return _bct.concatenate_columns(*args)

def concatenate_rows(*args):
  """
    concatenate_rows(gsl_vector v1, gsl_vector v2) -> gsl_matrix
    concatenate_rows(gsl_matrix m, gsl_vector v) -> gsl_matrix
    concatenate_rows(gsl_vector v, gsl_matrix m) -> gsl_matrix
    concatenate_rows(gsl_matrix m1, gsl_matrix m2) -> gsl_matrix
    """
  return _bct.concatenate_rows(*args)

def copy(*args):
  """
    copy(gsl_vector v) -> gsl_vector
    copy(gsl_matrix m) -> gsl_matrix
    """
  return _bct.copy(*args)

def logical_and(*args):
  """
    logical_and(gsl_vector v1, gsl_vector v2) -> gsl_vector
    logical_and(gsl_matrix m1, gsl_matrix m2) -> gsl_matrix
    """
  return _bct.logical_and(*args)

def logical_not(*args):
  """
    logical_not(gsl_vector v) -> gsl_vector
    logical_not(gsl_matrix m) -> gsl_matrix
    """
  return _bct.logical_not(*args)

def logical_or(*args):
  """
    logical_or(gsl_vector v1, gsl_vector v2) -> gsl_vector
    logical_or(gsl_matrix m1, gsl_matrix m2) -> gsl_matrix
    """
  return _bct.logical_or(*args)

def pow_elements(*args):
  """
    pow_elements(gsl_vector v, double power) -> gsl_vector
    pow_elements(gsl_vector v, gsl_vector powers) -> gsl_vector
    pow_elements(gsl_matrix m, double power) -> gsl_matrix
    pow_elements(gsl_matrix m, gsl_matrix powers) -> gsl_matrix
    """
  return _bct.pow_elements(*args)

def sequence_double(*args):
  """
    sequence_double(int start, int end) -> gsl_vector
    sequence_double(int start, int step, int end) -> gsl_vector
    """
  return _bct.sequence_double(*args)

def compare_elements(*args):
  """
    compare_elements(gsl_vector v, fp_cmp_fn_double compare, double x) -> gsl_vector
    compare_elements(gsl_vector v1, fp_cmp_fn_double compare, gsl_vector v2) -> gsl_vector
    compare_elements(gsl_matrix m, fp_cmp_fn_double compare, double x) -> gsl_matrix
    compare_elements(gsl_matrix m1, fp_cmp_fn_double compare, gsl_matrix m2) -> gsl_matrix
    """
  return _bct.compare_elements(*args)

def ord_log_index_assign(*args):
  """
    ord_log_index_assign(gsl_matrix m, gsl_vector rows, gsl_vector logical_columns, 
        double value)
    ord_log_index_assign(gsl_matrix m, gsl_vector rows, gsl_vector logical_columns, 
        gsl_matrix values)
    """
  return _bct.ord_log_index_assign(*args)

def log_ord_index_assign(*args):
  """
    log_ord_index_assign(gsl_matrix m, gsl_vector logical_rows, gsl_vector columns, 
        double value)
    log_ord_index_assign(gsl_matrix m, gsl_vector logical_rows, gsl_vector columns, 
        gsl_matrix values)
    """
  return _bct.log_ord_index_assign(*args)

def ordinal_index(*args):
  """
    ordinal_index(gsl_vector v, gsl_vector indices) -> gsl_vector
    ordinal_index(gsl_matrix m, int index) -> double
    ordinal_index(gsl_matrix m, gsl_vector indices) -> gsl_vector
    ordinal_index(gsl_matrix m, gsl_vector rows, gsl_vector columns) -> gsl_matrix
    ordinal_index(gsl_matrix m, gsl_matrix indices) -> gsl_matrix
    """
  return _bct.ordinal_index(*args)

def ordinal_index_assign(*args):
  """
    ordinal_index_assign(gsl_vector v, gsl_vector indices, double value)
    ordinal_index_assign(gsl_vector v, gsl_vector indices, gsl_vector values)
    ordinal_index_assign(gsl_matrix m, int index, double value)
    ordinal_index_assign(gsl_matrix m, gsl_vector indices, double value)
    ordinal_index_assign(gsl_matrix m, gsl_vector indices, gsl_vector values)
    ordinal_index_assign(gsl_matrix m, gsl_vector rows, gsl_vector columns, 
        double value)
    ordinal_index_assign(gsl_matrix m, gsl_vector rows, gsl_vector columns, 
        gsl_matrix values)
    ordinal_index_assign(gsl_matrix m, gsl_matrix indices, double value)
    ordinal_index_assign(gsl_matrix m, gsl_matrix indices, gsl_matrix values)
    """
  return _bct.ordinal_index_assign(*args)

def logical_index(*args):
  """
    logical_index(gsl_vector v, gsl_vector logical_v) -> gsl_vector
    logical_index(gsl_matrix m, gsl_vector logical_v) -> gsl_vector
    logical_index(gsl_matrix m, gsl_vector logical_rows, gsl_vector logical_columns) -> gsl_matrix
    logical_index(gsl_matrix m, gsl_matrix logical_m) -> gsl_vector
    """
  return _bct.logical_index(*args)

def logical_index_assign(*args):
  """
    logical_index_assign(gsl_vector v, gsl_vector logical_v, double values)
    logical_index_assign(gsl_vector v, gsl_vector logical_v, gsl_vector values)
    logical_index_assign(gsl_matrix m, gsl_vector logical_v, double value)
    logical_index_assign(gsl_matrix m, gsl_vector logical_v, gsl_vector values)
    logical_index_assign(gsl_matrix m, gsl_vector logical_rows, gsl_vector logical_columns, 
        double value)
    logical_index_assign(gsl_matrix m, gsl_vector logical_rows, gsl_vector logical_columns, 
        gsl_matrix values)
    logical_index_assign(gsl_matrix m, gsl_matrix logical_m, double value)
    logical_index_assign(gsl_matrix m, gsl_matrix logical_m, gsl_vector values)
    """
  return _bct.logical_index_assign(*args)

def to_bool(*args):
  """
    to_bool(gsl_vector v) -> bool
    to_bool(gsl_matrix m) -> bool
    """
  return _bct.to_bool(*args)

def to_vector_double(*args):
  """
    to_vector_double(gsl_vector v) -> gsl_vector
    to_vector_double(gsl_permutation p) -> gsl_vector
    """
  return _bct.to_vector_double(*args)

