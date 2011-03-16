#ifndef BCT_H
#define BCT_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>
#include "matlab/matlab_double.h"
#include "matlab/matlab_float.h"
#include "matlab/matlab_long_double.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace bct {
	using namespace matlab;
	
	class bct_exception : public std::runtime_error {
	public:
		bct_exception(const std::string& what_arg) : std::runtime_error(what_arg) { }
	};

	// Density, degree, and assortativity
	double assortativity_dir(const gsl_matrix* CIJ);
	double assortativity_und(const gsl_matrix* CIJ);
	gsl_vector* degrees_dir(const gsl_matrix* CIJ, gsl_vector** id = NULL, gsl_vector** od = NULL);
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
	gsl_vector* strengths_dir(const gsl_matrix* CIJ, gsl_vector** is = NULL, gsl_vector** os = NULL);
	gsl_vector* strengths_und(const gsl_matrix* CIJ);

	// Clustering
	gsl_vector* clustering_coef_bd(const gsl_matrix* A);
	gsl_vector* clustering_coef_bu(const gsl_matrix* G);
	gsl_vector* clustering_coef_wd(const gsl_matrix* W);
	gsl_vector* clustering_coef_wu(const gsl_matrix* W);
	gsl_vector* efficiency_local(const gsl_matrix* G);
	gsl_vector_float* efficiency_local(const gsl_matrix_float* G);

	// Paths, distances, and cycles
	gsl_vector* breadth(const gsl_matrix* CIJ, int source, gsl_vector** branch = NULL);
	gsl_matrix* breadthdist(const gsl_matrix* CIJ, gsl_matrix** D = NULL);
	gsl_vector* charpath_ecc(const gsl_matrix* D, double* radius = NULL, double* diameter = NULL);
	double charpath_lambda(const gsl_matrix* D);
	double connectivity_length(const gsl_matrix* D);
	gsl_vector* cycprob_fcyc(const std::vector<gsl_matrix*>& Pq);
	gsl_vector* cycprob_pcyc(const std::vector<gsl_matrix*>& Pq);
	gsl_matrix* distance_bin(const gsl_matrix* G);
	gsl_matrix* distance_wei(const gsl_matrix* G); 
	gsl_matrix* efficiency_global(const gsl_matrix* G);
	gsl_matrix_float* efficiency_global(const gsl_matrix_float* G);

	std::vector<gsl_matrix*> findpaths(const gsl_matrix* CIJ, const gsl_vector* sources, int qmax, gsl_vector** plq = NULL, int* qstop = NULL, gsl_matrix** allpths = NULL, gsl_matrix** util = NULL);
	std::vector<gsl_matrix*> findwalks(const gsl_matrix* CIJ, gsl_vector** wlq = NULL);
	double normalized_path_length(const gsl_matrix* D, double wmax = 1.0);
	gsl_matrix* reachdist(const gsl_matrix* CIJ, gsl_matrix** D = NULL);

	// Centrality
	gsl_vector* betweenness_bin(const gsl_matrix* G);
	gsl_vector* betweenness_wei(const gsl_matrix* G);
	gsl_matrix* edge_betweenness_bin(const gsl_matrix* G, gsl_vector** BC = NULL);
	gsl_matrix* edge_betweenness_wei(const gsl_matrix* G, gsl_vector** BC = NULL);
	gsl_matrix* erange(const gsl_matrix* CIJ, double* eta = NULL, gsl_matrix** Eshort = NULL, double* fs = NULL);
	gsl_vector* eigenvector_centrality(const gsl_matrix* G);
	gsl_vector_float* eigenvector_centrality(const gsl_matrix_float* G);


	// Motifs
	enum motif_mode_enum { MILO, SPORNS };
	extern motif_mode_enum motif_mode;
	motif_mode_enum get_motif_mode();
	void set_motif_mode(motif_mode_enum motif_mode);
	std::vector<gsl_matrix*> find_motif34(int m, int n);
	int find_motif34(const gsl_matrix* m);
	gsl_vector* motif3funct_bin(const gsl_matrix* W, gsl_matrix** F = NULL);
	gsl_matrix* motif3funct_wei(const gsl_matrix* W, gsl_matrix** Q = NULL, gsl_matrix** F = NULL);
	gsl_vector* motif3funct_wei_v(const gsl_matrix* W, gsl_vector** Q = NULL, gsl_vector** F = NULL);
	gsl_matrix* motif3generate(gsl_vector** ID = NULL, gsl_vector** N = NULL);
	gsl_vector* motif3struct_bin(const gsl_matrix* A, gsl_matrix** F = NULL);
	gsl_matrix* motif3struct_wei(const gsl_matrix* W, gsl_matrix** Q = NULL, gsl_matrix** F = NULL);
	gsl_vector* motif3struct_wei_v(const gsl_matrix* W, gsl_vector** Q = NULL, gsl_vector** F = NULL);
	gsl_matrix* motif4generate(gsl_vector** ID = NULL, gsl_vector** N = NULL);
	gsl_vector* motif4funct_bin(const gsl_matrix* W, gsl_matrix** F = NULL);
	gsl_matrix* motif4funct_wei(const gsl_matrix* W, gsl_matrix** Q = NULL, gsl_matrix** F = NULL);
	gsl_vector* motif4funct_wei_v(const gsl_matrix* W, gsl_vector** Q = NULL, gsl_vector** F = NULL);
	gsl_vector* motif4struct_bin(const gsl_matrix* A, gsl_matrix** F = NULL);
	gsl_matrix* motif4struct_wei(const gsl_matrix* W, gsl_matrix** Q = NULL, gsl_matrix** F = NULL);
	gsl_vector* motif4struct_wei_v(const gsl_matrix* W, gsl_vector** Q = NULL, gsl_vector** F = NULL);

	// Modularity and community structure
	double modularity_dir(const gsl_matrix* A, gsl_vector** Ci = NULL);
	double modularity_und(const gsl_matrix* A, gsl_vector** Ci = NULL);
	double modularity_und_louvain(const gsl_matrix* W, gsl_vector** Ci = NULL, int N = 100);
	gsl_vector* module_degree_zscore(const gsl_matrix* A, const gsl_vector* Ci);
	gsl_vector* participation_coef(const gsl_matrix* A, const gsl_vector* Ci);
	
	// Synthetic connection networks
	gsl_matrix* makeevenCIJ(int N, int K, int sz_cl);
	gsl_matrix* makefractalCIJ(int mx_lvl, double E, int sz_cl, int* K = NULL);
	gsl_matrix* makelatticeCIJ(int N, int K);
	gsl_matrix* makerandCIJ_bd(int N, int K);
	gsl_matrix* makerandCIJ_bu(int N, int K);
	gsl_matrix* makerandCIJ_wd(int N, int K, double wmin, double wmax);
	gsl_matrix* makerandCIJ_wd_wp(const gsl_matrix* m);
	gsl_matrix* makerandCIJ_wu(int N, int K, double wmin, double wmax);
	gsl_matrix* makerandCIJ_wu_wp(const gsl_matrix* m);
	gsl_matrix* makerandCIJdegreesfixed(const gsl_vector* in, const gsl_vector* out);
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
	bool check_status(const gsl_matrix_float* m, int flags, const std::string& text);
	bool is_square(const gsl_matrix* m);
	bool is_square(const gsl_matrix_float* m);
	bool is_rectangular(const gsl_matrix* m);
	bool is_rectangular(const gsl_matrix_float* m);
	bool is_undirected(const gsl_matrix* m);
	bool is_undirected(const gsl_matrix_float* m);
	bool is_directed(const gsl_matrix* m);
	bool is_directed(const gsl_matrix_float* m);
	bool is_binary(const gsl_matrix* m);
	bool is_binary(const gsl_matrix_float* m);
	bool is_weighted(const gsl_matrix* m);
	bool is_weighted(const gsl_matrix_float* m);
	bool is_positive(const gsl_matrix* m);
	bool is_positive(const gsl_matrix_float* m);
	bool is_signed(const gsl_matrix* m);
	bool is_signed(const gsl_matrix_float* m);
	bool has_loops(const gsl_matrix* m);
	bool has_loops(const gsl_matrix_float* m);
	bool has_no_loops(const gsl_matrix* m);
	bool has_no_loops(const gsl_matrix_float* m);

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

#endif
