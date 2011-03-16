#include "bct.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_vector.h>
#include <sstream>
#include <string>
#include <vector>

/*
 * Catches GSL errors and throws BCT exceptions.
 */
void bct::gsl_error_handler(const char* reason, const char* file, int line, int gsl_errno) {
	std::stringstream what;
	what << reason << " in " << file << ", line " << line << ".";
	throw bct_exception(what.str());
}

/*
 * Overloaded convenience function for freeing GSL vectors and matrices.
 */
void bct::gsl_free(gsl_vector* v) { gsl_vector_free(v); }
void bct::gsl_free(gsl_matrix* m) { gsl_matrix_free(m); }
void bct::gsl_free(std::vector<gsl_matrix*>& m) {
	for (int i = 0; i < (int)m.size(); i++) {
		if (m[i] != NULL) {
			gsl_matrix_free(m[i]);
			m[i] = NULL;
		}
	}
}
void bct::gsl_free(gsl_permutation* p) { gsl_permutation_free(p); }

/*
 * Initializes the BCT library for external use.
 */
void bct::init() {
	gsl_set_error_handler(gsl_error_handler);
}

/*
 * Returns the number of edges in a directed graph.
 */
int bct::number_of_edges_dir(const gsl_matrix* m) {
	return nnz(m);
}

/*
 * Returns the number of edges in an undirected graph.
 */
int bct::number_of_edges_und(const gsl_matrix* m) {
	gsl_matrix* triu_m = triu(m);
	int ret = nnz(triu_m);
	gsl_matrix_free(triu_m);
	return ret;
}

/*
 * Returns the number of nodes in a graph.
 */
int bct::number_of_nodes(const gsl_matrix* m) {
	return (int)m->size1;
}
