#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes strength, in-strength, and out-strength for a directed graph.
 */
gsl_vector* bct::strengths_dir(const gsl_matrix* CIJ, gsl_vector** is, gsl_vector** os) {
	if (safe_mode) check_status(CIJ, SQUARE | DIRECTED, "strengths_dir");
	
	// is = sum(CIJ,1);
	gsl_vector* _is = sum(CIJ, 1);
	
	// os = sum(CIJ,2);
	gsl_vector* _os = sum(CIJ, 2);
	
	// str = is+os;
	gsl_vector* str = copy(_is);
	gsl_vector_add(str, _os);
	
	if (is != NULL) *is = _is; else gsl_vector_free(_is);
	if (os != NULL) *os = _os; else gsl_vector_free(_os);
	return str;
}
