%module bct_gsl
%include "std_string.i"
%include "swig.i"
%include "typemaps.i"
%feature("autodoc", "1");

%{
	#include "bct.h"
%}

%apply int* OUTPUT { int* qstop, int* K };
%apply double* OUTPUT { double* radius, double* diameter, double* eta, double* fs };

%typemap(in, numinputs = 0) gsl_vector** (gsl_vector* temp) { $1 = &temp; }
%typemap(argout) gsl_vector** { %append_output(SWIG_NewPointerObj(*$1, $descriptor(gsl_vector*), 0)); }

%typemap(in, numinputs = 0) gsl_matrix** (gsl_matrix* temp) { $1 = &temp; }
%typemap(argout) gsl_matrix** { %append_output(SWIG_NewPointerObj(*$1, $descriptor(gsl_matrix*), 0)); }

namespace bct {

	// Density, degree, and assortativity
	double assortativity_dir(const gsl_matrix* CIJ);
	double assortativity_und(const gsl_matrix* CIJ);
	gsl_vector* degrees_dir(const gsl_matrix* CIJ, gsl_vector** id, gsl_vector** od);
	gsl_vector* degrees_und(const gsl_matrix* CIJ);
	double density_dir(const gsl_matrix* CIJ);
	double density_und(const gsl_matrix* CIJ);
	gsl_matrix* jdegree(const gsl_matrix* CIJ);
	int jdegree_bl(const gsl_matrix* J);
	int jdegree_id(const gsl_matrix* J);
	int jdegree_od(const gsl_matrix* J);
	gsl_matrix* matching_ind(const gsl_matrix* CIJ);
	gsl_matrix* matching_ind_in(const gsl_matrix* CIJ);
	gsl_matrix* matching_ind_out(const gsl_matrix* CIJ);
	gsl_vector* strengths_dir(const gsl_matrix* CIJ, gsl_vector** _is, gsl_vector** os);
	gsl_vector* strengths_und(const gsl_matrix* CIJ);

	// Clustering
	gsl_vector* clustering_coef_bd(const gsl_matrix* A);
	gsl_vector* clustering_coef_bu(const gsl_matrix* G);
	gsl_vector* clustering_coef_wd(const gsl_matrix* W);
	gsl_vector* clustering_coef_wu(const gsl_matrix* W);
	gsl_vector* efficiency_local(const gsl_matrix* G);

	// Paths, distances, and cycles
	gsl_vector* breadth(const gsl_matrix* CIJ, int source, gsl_vector** branch);
	gsl_matrix* breadthdist(const gsl_matrix* CIJ, gsl_matrix** D);
	gsl_vector* charpath_ecc(const gsl_matrix* D, double* radius, double* diameter);
	double charpath_lambda(const gsl_matrix* D);
	double connectivity_length(const gsl_matrix* D);
	gsl_vector* cycprob_fcyc(const std::vector<gsl_matrix*>& Pq);
	gsl_vector* cycprob_pcyc(const std::vector<gsl_matrix*>& Pq);
	gsl_matrix* distance_bin(const gsl_matrix* G);
	gsl_matrix* distance_wei(const gsl_matrix* G); 
	gsl_matrix* efficiency_global(const gsl_matrix* G);
	std::vector<gsl_matrix*> findpaths(const gsl_matrix* CIJ, const gsl_vector* sources, int qmax, gsl_vector** plq, int* qstop, gsl_matrix** allpths, gsl_matrix** util);
	std::vector<gsl_matrix*> findwalks(const gsl_matrix* CIJ, gsl_vector** wlq);
	double normalized_path_length(const gsl_matrix* D, double wmax = 1.0);
	gsl_matrix* reachdist(const gsl_matrix* CIJ, gsl_matrix** D);

	// Centrality
	gsl_vector* betweenness_bin(const gsl_matrix* G);
	gsl_vector* betweenness_wei(const gsl_matrix* G);
	gsl_matrix* edge_betweenness_bin(const gsl_matrix* G, gsl_vector** BC);
	gsl_matrix* edge_betweenness_wei(const gsl_matrix* G, gsl_vector** BC);
	gsl_matrix* erange(const gsl_matrix* CIJ, double* eta, gsl_matrix** Eshort, double* fs);

	// Motifs
	enum motif_mode_enum { MILO, SPORNS };
	extern motif_mode_enum motif_mode;
	motif_mode_enum get_motif_mode();
	void set_motif_mode(motif_mode_enum motif_mode);
	std::vector<gsl_matrix*> find_motif34(int m, int n);
	int find_motif34(const gsl_matrix* m);
	gsl_vector* motif3funct_bin(const gsl_matrix* W, gsl_matrix** F);
	gsl_matrix* motif3funct_wei(const gsl_matrix* W, gsl_matrix** Q, gsl_matrix** F);
	gsl_vector* motif3funct_wei_v(const gsl_matrix* W, gsl_vector** Q, gsl_vector** F);
	gsl_matrix* motif3generate(gsl_vector** ID, gsl_vector** N);
	gsl_vector* motif3struct_bin(const gsl_matrix* A, gsl_matrix** F);
	gsl_matrix* motif3struct_wei(const gsl_matrix* W, gsl_matrix** Q, gsl_matrix** F);
	gsl_vector* motif3struct_wei_v(const gsl_matrix* W, gsl_vector** Q, gsl_vector** F);
	gsl_matrix* motif4generate(gsl_vector** ID, gsl_vector** N);
	gsl_vector* motif4funct_bin(const gsl_matrix* W, gsl_matrix** F);
	gsl_matrix* motif4funct_wei(const gsl_matrix* W, gsl_matrix** Q, gsl_matrix** F);
	gsl_vector* motif4funct_wei_v(const gsl_matrix* W, gsl_vector** Q, gsl_vector** F);
	gsl_vector* motif4struct_bin(const gsl_matrix* A, gsl_matrix** F);
	gsl_matrix* motif4struct_wei(const gsl_matrix* W, gsl_matrix** Q, gsl_matrix** F);
	gsl_vector* motif4struct_wei_v(const gsl_matrix* W, gsl_vector** Q, gsl_vector** F);

	// Modularity and community structure
	double modularity_dir(const gsl_matrix* A, gsl_vector** Ci);
	double modularity_und(const gsl_matrix* A, gsl_vector** Ci);
	double modularity_und_louvain(const gsl_matrix* W, gsl_vector** Ci, int N = 100);
	gsl_vector* module_degree_zscore(const gsl_matrix* A, const gsl_vector* Ci);
	gsl_vector* participation_coef(const gsl_matrix* A, const gsl_vector* Ci);
	
	// Synthetic connection networks
	gsl_matrix* makeevenCIJ(int N, int K, int sz_cl);
	gsl_matrix* makefractalCIJ(int mx_lvl, double E, int sz_cl, int* K);
	gsl_matrix* makelatticeCIJ(int N, int K);
	gsl_matrix* makerandCIJ_bd(int N, int K);
	gsl_matrix* makerandCIJ_bu(int N, int K);
	gsl_matrix* makerandCIJ_wd(int N, int K, double wmin, double wmax);
	gsl_matrix* makerandCIJ_wd_wp(const gsl_matrix* m);
	gsl_matrix* makerandCIJ_wu(int N, int K, double wmin, double wmax);
	gsl_matrix* makerandCIJ_wu_wp(const gsl_matrix* m);
	gsl_matrix* makerandCIJdegreesfixed(const gsl_vector* _in, const gsl_vector* out);
	gsl_matrix* makerandCIJdegreesfixed(const gsl_matrix* m);
	gsl_matrix* makeringlatticeCIJ(int N, int K);
	gsl_matrix* maketoeplitzCIJ(int N, int K, double s);
	
	// Graph randomization
	gsl_matrix* latmio_dir(const gsl_matrix* R, int ITER);
	gsl_matrix* latmio_dir_connected(const gsl_matrix* R, int ITER);	
	gsl_matrix* latmio_und(const gsl_matrix* R, int ITER);
	gsl_matrix* latmio_und_connected(const gsl_matrix* R, int ITER);
	gsl_matrix* randmio_dir(const gsl_matrix* R, int ITER);
	gsl_matrix* randmio_dir_connected(const gsl_matrix* R, int ITER);
	gsl_matrix* randmio_und(const gsl_matrix* R, int ITER);
	gsl_matrix* randmio_und_connected(const gsl_matrix* R, int ITER);

	// Data sets
	gsl_matrix* get_cat_all();
	gsl_matrix* get_cat_ctx();
	gsl_matrix* get_fve30();
	gsl_matrix* get_fve32();
	gsl_matrix* get_macaque47();
	gsl_matrix* get_macaque71();
	
	// Matrix status checking
	enum status {
		SQUARE = 1, RECTANGULAR = 2,
		UNDIRECTED = 4, DIRECTED = 8,
		BINARY = 16, WEIGHTED = 32,
		POSITIVE = 64, SIGNED = 128,
		NO_LOOPS = 256, LOOPS = 512
	};
	extern bool safe_mode;
	bool get_safe_mode();
	void set_safe_mode(bool safe_mode);
	bool check_status(const gsl_matrix* m, int flags, const std::string& text);
	bool is_square(const gsl_matrix* m);
	bool is_rectangular(const gsl_matrix* m);
	bool is_undirected(const gsl_matrix* m);
	bool is_directed(const gsl_matrix* m);
	bool is_binary(const gsl_matrix* m);
	bool is_weighted(const gsl_matrix* m);
	bool is_positive(const gsl_matrix* m);
	bool is_signed(const gsl_matrix* m);
	bool has_loops(const gsl_matrix* m);
	bool has_no_loops(const gsl_matrix* m);
	
	// Matrix conversion
	gsl_matrix* invert_elements(const gsl_matrix* m);
	gsl_matrix* remove_loops(const gsl_matrix* m);
	gsl_matrix* to_binary(const gsl_matrix* m);
	gsl_matrix* to_positive(const gsl_matrix* m);
	gsl_matrix* to_undirected_bin(const gsl_matrix* m);
	gsl_matrix* to_undirected_wei(const gsl_matrix* m);
	
	// Utility
	void gsl_error_handler(const char* reason, const char* file, int line, int gsl_errno);
	void gsl_free(gsl_vector* v);
	void gsl_free(gsl_matrix* m);
	void gsl_free(std::vector<gsl_matrix*>& m);
	void gsl_free(gsl_permutation* p);
	void init();
	int number_of_edges_dir(const gsl_matrix* m);
	int number_of_edges_und(const gsl_matrix* m);
	int number_of_nodes(const gsl_matrix* m);
	gsl_matrix* threshold_absolute(const gsl_matrix* W, double thr);
	gsl_matrix* threshold_proportional_dir(const gsl_matrix* W, double p);
	gsl_matrix* threshold_proportional_und(const gsl_matrix* W, double p);
	
	// Debugging
	void printf(const gsl_vector* v, const std::string& format);
	void printf(const gsl_matrix* m, const std::string& format);
	void printf(const gsl_permutation* p, const std::string& format);
}

namespace matlab {
	
	// ---------------------------------
	// Precision-independent definitions
	// ---------------------------------
	
	// Functions
	std::string dec2bin(int n);
	std::string dec2bin(int n, int len);
	gsl_matrix* inv(const gsl_matrix* m);
	gsl_permutation* randperm(int size);
	
	// Operators
	gsl_matrix* div_left(const gsl_matrix* m1, const gsl_matrix* m2);
	gsl_matrix* div_right(const gsl_matrix* m1, const gsl_matrix* m2);
	
	// Utility
	gsl_rng* get_gsl_rng();
	void seed_rng(const gsl_rng* rng, unsigned long seed);
	
	// -------------------------------
	// Precision-dependent definitions
	// -------------------------------
	
	// Functions
	gsl_vector* abs(const gsl_vector* v);
	gsl_matrix* abs(const gsl_matrix* m);
	int all(const gsl_vector* v);
	gsl_vector* all(const gsl_matrix* m, int dim = 1);
	int any(const gsl_vector* v);
	gsl_vector* any(const gsl_matrix* m, int dim = 1);
	gsl_matrix* diag(const gsl_vector* v, int k = 0);
	gsl_vector* diag(const gsl_matrix* m, int k = 0);
	gsl_matrix* eye_double(int size);
	gsl_matrix* eye_double(int size1, int size2);
	gsl_vector* find(const gsl_vector* v, int n = std::numeric_limits<int>::max(), const std::string& direction = "first");
	gsl_vector* find(const gsl_matrix* m, int n = std::numeric_limits<int>::max(), const std::string& direction = "first");
	gsl_matrix* find_ij(const gsl_matrix* m, int n = std::numeric_limits<int>::max(), const std::string& direction = "first");
	gsl_vector* hist(const gsl_vector* v, int n = 10);
	gsl_vector* hist(const gsl_vector* v, const gsl_vector* centers);
	int length(const gsl_vector* v);
	int length(const gsl_matrix* m);
	double max(double x, double y);
	double max(const gsl_vector* v);
	gsl_vector* max(const gsl_matrix* m, int dim = 1);
	double mean(const gsl_vector* v, const std::string& opt = "a");
	gsl_vector* mean(const gsl_matrix* m, int dim = 1, const std::string& opt = "a");
	double min(double x, double y);
	double min(const gsl_vector* v);
	gsl_vector* min(const gsl_matrix* m, int dim = 1);
	int nnz(const gsl_vector* v);
	int nnz(const gsl_matrix* m);
	gsl_vector* nonzeros(const gsl_matrix* m);
	gsl_vector* normpdf(const gsl_vector* v, double mean, double stdev);
	gsl_matrix* ones_double(int size);
	gsl_matrix* ones_double(int size1, int size2);
	gsl_vector* ones_vector_double(int size);
	double prod(const gsl_vector* v);
	gsl_vector* prod(const gsl_matrix* m, int dim = 1);
	gsl_matrix* rand_double(int size);
	gsl_matrix* rand_double(int size1, int size2);
	gsl_vector* rand_vector_double(int size);
	gsl_vector* reverse(const gsl_vector* v);
	gsl_vector* setxor(const gsl_vector* v1, const gsl_vector* v2);
	gsl_vector* sort(const gsl_vector* v, const std::string& mode, gsl_vector** ind);
	gsl_matrix* sort(const gsl_matrix* m, int dim, const std::string& mode, gsl_matrix** ind);
	gsl_vector* sortrows(const gsl_vector* v, gsl_vector** ind);
	gsl_matrix* sortrows(const gsl_matrix* m, gsl_vector** ind);
	double std(const gsl_vector* v, int opt = 0);
	gsl_vector* std(const gsl_matrix* m, int opt = 0, int dim = 1);
	double sum(const gsl_vector* v);
	gsl_vector* sum(const gsl_matrix* m, int dim = 1);
	gsl_matrix* toeplitz(const gsl_vector* column, const gsl_vector* row = NULL);
	gsl_matrix* tril(const gsl_matrix* m, int k = 0);
	gsl_matrix* triu(const gsl_matrix* m, int k = 0);
	gsl_vector* unique(const gsl_vector* v, const std::string& first_or_last, gsl_vector** i, gsl_vector** j);
	gsl_vector* unique(const gsl_matrix* m, const std::string& first_or_last, gsl_vector** i, gsl_vector** j);
	gsl_matrix* unique_rows(const gsl_matrix* m, const std::string& first_or_last, gsl_vector** i, gsl_vector** j);
	gsl_matrix* zeros_double(int size);
	gsl_matrix* zeros_double(int size1, int size2);
	gsl_vector* zeros_vector_double(int size);
	
	// Operators
	gsl_vector* concatenate(const gsl_vector* v, double x);
	gsl_vector* concatenate(double x, const gsl_vector* v);
	gsl_vector* concatenate(const gsl_vector* v1, const gsl_vector* v2);
	gsl_matrix* concatenate_columns(const gsl_vector* v1, const gsl_vector* v2);
	gsl_matrix* concatenate_columns(const gsl_matrix* m, const gsl_vector* v);
	gsl_matrix* concatenate_columns(const gsl_vector* v, const gsl_matrix* m);
	gsl_matrix* concatenate_columns(const gsl_matrix* m1, const gsl_matrix* m2);
	gsl_matrix* concatenate_rows(const gsl_vector* v1, const gsl_vector* v2);
	gsl_matrix* concatenate_rows(const gsl_matrix* m, const gsl_vector* v);
	gsl_matrix* concatenate_rows(const gsl_vector* v, const gsl_matrix* m);
	gsl_matrix* concatenate_rows(const gsl_matrix* m1, const gsl_matrix* m2);
	gsl_vector* copy(const gsl_vector* v);
	gsl_matrix* copy(const gsl_matrix* m);
	gsl_vector* logical_and(const gsl_vector* v1, const gsl_vector* v2);
	gsl_matrix* logical_and(const gsl_matrix* m1, const gsl_matrix* m2);
	gsl_vector* logical_not(const gsl_vector* v);
	gsl_matrix* logical_not(const gsl_matrix* m);
	gsl_vector* logical_or(const gsl_vector* v1, const gsl_vector* v2);
	gsl_matrix* logical_or(const gsl_matrix* m1, const gsl_matrix* m2);
	gsl_matrix* mul(const gsl_matrix* m1, const gsl_matrix* m2);
	gsl_matrix* pow(const gsl_matrix* m, int power);
	gsl_vector* pow_elements(const gsl_vector* v, double power);
	gsl_vector* pow_elements(const gsl_vector* v, const gsl_vector* powers);
	gsl_matrix* pow_elements(const gsl_matrix* m, double power);
	gsl_matrix* pow_elements(const gsl_matrix* m, const gsl_matrix* powers);
	gsl_vector* sequence_double(int start, int end);
	gsl_vector* sequence_double(int start, int step, int end);
	
	// Floating-point comparison
	extern double epsilon_double;
	int fp_compare(double x, double y);
	typedef bool (*fp_cmp_fn_double)(double, double);
	bool fp_zero(double x);
	bool fp_nonzero(double x);
	bool fp_equal(double x, double y);
	bool fp_not_equal(double x, double y);
	bool fp_less(double x, double y);
	bool fp_less_or_equal(double x, double y);
	bool fp_greater(double x, double y);
	bool fp_greater_or_equal(double x, double y);
	
	// Vector/matrix comparison
	int compare_vectors(const gsl_vector* v1, const gsl_vector* v2);
	bool vector_less(gsl_vector* v1, gsl_vector* v2);
	int compare_matrices(const gsl_matrix* m1, const gsl_matrix* m2);
	bool matrix_less(gsl_matrix* m1, gsl_matrix* m2);
	gsl_vector* compare_elements(const gsl_vector* v, fp_cmp_fn_double compare, double x);
	gsl_vector* compare_elements(const gsl_vector* v1, fp_cmp_fn_double compare, const gsl_vector* v2);
	gsl_matrix* compare_elements(const gsl_matrix* m, fp_cmp_fn_double compare, double x);
	gsl_matrix* compare_elements(const gsl_matrix* m1, fp_cmp_fn_double compare, const gsl_matrix* m2);
	
	// Vector-by-vector indexing
	gsl_vector* ordinal_index(const gsl_vector* v, const gsl_vector* indices);
	void ordinal_index_assign(gsl_vector* v, const gsl_vector* indices, double value);
	void ordinal_index_assign(gsl_vector* v, const gsl_vector* indices, const gsl_vector* values);
	gsl_vector* logical_index(const gsl_vector* v, const gsl_vector* logical_v);
	void logical_index_assign(gsl_vector* v, const gsl_vector* logical_v, double values);
	void logical_index_assign(gsl_vector* v, const gsl_vector* logical_v, const gsl_vector* values);
	
	// Matrix-by-integer indexing
	double ordinal_index(const gsl_matrix* m, int index);
	void ordinal_index_assign(gsl_matrix* m, int index, double value);
	
	// Matrix-by-vector indexing
	gsl_vector* ordinal_index(const gsl_matrix* m, const gsl_vector* indices);
	void ordinal_index_assign(gsl_matrix* m, const gsl_vector* indices, double value);
	void ordinal_index_assign(gsl_matrix* m, const gsl_vector* indices, const gsl_vector* values);
	gsl_vector* logical_index(const gsl_matrix* m, const gsl_vector* logical_v);
	void logical_index_assign(gsl_matrix* m, const gsl_vector* logical_v, double value);
	void logical_index_assign(gsl_matrix* m, const gsl_vector* logical_v, const gsl_vector* values);
	
	// Matrix-by-two-vectors indexing (non-mixed)
	gsl_matrix* ordinal_index(const gsl_matrix* m, const gsl_vector* rows, const gsl_vector* columns);
	void ordinal_index_assign(gsl_matrix* m, const gsl_vector* rows, const gsl_vector* columns, double value);
	void ordinal_index_assign(gsl_matrix* m, const gsl_vector* rows, const gsl_vector* columns, const gsl_matrix* values);
	gsl_matrix* logical_index(const gsl_matrix* m, const gsl_vector* logical_rows, const gsl_vector* logical_columns);
	void logical_index_assign(gsl_matrix* m, const gsl_vector* logical_rows, const gsl_vector* logical_columns, double value);
	void logical_index_assign(gsl_matrix* m, const gsl_vector* logical_rows, const gsl_vector* logical_columns, const gsl_matrix* values);
	
	// Matrix-by-two-vectors indexing (mixed)
	gsl_matrix* ord_log_index(const gsl_matrix* m, const gsl_vector* rows, const gsl_vector* logical_columns);
	void ord_log_index_assign(gsl_matrix* m, const gsl_vector* rows, const gsl_vector* logical_columns, double value);
	void ord_log_index_assign(gsl_matrix* m, const gsl_vector* rows, const gsl_vector* logical_columns, const gsl_matrix* values);
	gsl_matrix* log_ord_index(const gsl_matrix* m, const gsl_vector* logical_rows, const gsl_vector* columns);
	void log_ord_index_assign(gsl_matrix* m, const gsl_vector* logical_rows, const gsl_vector* columns, double value);
	void log_ord_index_assign(gsl_matrix* m, const gsl_vector* logical_rows, const gsl_vector* columns, const gsl_matrix* values);
	
	// Matrix-by-matrix indexing
	gsl_matrix* ordinal_index(const gsl_matrix* m, const gsl_matrix* indices);
	void ordinal_index_assign(gsl_matrix* m, const gsl_matrix* indices, double value);
	void ordinal_index_assign(gsl_matrix* m, const gsl_matrix* indices, const gsl_matrix* values);
	gsl_vector* logical_index(const gsl_matrix* m, const gsl_matrix* logical_m);
	void logical_index_assign(gsl_matrix* m, const gsl_matrix* logical_m, double value);
	void logical_index_assign(gsl_matrix* m, const gsl_matrix* logical_m, const gsl_vector* values);
	
	// Vector/matrix conversion
	void to_array(const gsl_vector* v, double* array);
	bool to_bool(const gsl_vector* v);
	bool to_bool(const gsl_matrix* m);
	gsl_vector_float* to_vector_float(const gsl_vector* v);
	gsl_vector* to_vector_double(const gsl_vector* v);
	gsl_vector_long_double* to_vector_long_double(const gsl_vector* v);
	gsl_vector* to_vector(const gsl_matrix* m);
	gsl_matrix_float* to_matrix_float(const gsl_matrix* m);
	gsl_matrix* to_matrix_double(const gsl_matrix* m);
	gsl_matrix_long_double* to_matrix_long_double(const gsl_matrix* m);
	gsl_matrix* to_column_matrix(const gsl_vector* v);
	gsl_matrix* to_row_matrix(const gsl_vector* v);
	gsl_vector* to_vector_double(const gsl_permutation* p);
	gsl_permutation* to_permutation(const gsl_vector* v);
	
	// Utility
	gsl_matrix* permute_columns(const gsl_permutation* p, const gsl_matrix* m);
	gsl_matrix* permute_rows(const gsl_permutation* p, const gsl_matrix* m);
}

PyObject* from_gsl(const gsl_vector* v);
PyObject* from_gsl(const gsl_matrix* m);
PyObject* from_gsl(const std::vector<gsl_matrix*>& m);
PyObject* from_gsl(const gsl_permutation* p);
gsl_vector* to_gslv(PyObject* list);
gsl_matrix* to_gslm(PyObject* list);
std::vector<gsl_matrix*> to_gsl3dm(PyObject* list);
gsl_permutation* to_gslp(PyObject* list);
