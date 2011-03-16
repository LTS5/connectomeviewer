#include "bct.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>

/*
 * Computes degree, in-degree, and out-degree for a directed graph.  Connection
 * weights are ignored.
 */
gsl_vector* bct::degrees_dir(const gsl_matrix* CIJ, gsl_vector** id, gsl_vector** od) {
	if (safe_mode) check_status(CIJ, SQUARE | DIRECTED, "degrees_dir");
	
	// CIJ = double(CIJ~=0);
	// id = sum(CIJ,1);
	gsl_vector* _id = gsl_vector_alloc(CIJ->size2);
	for (int i = 0; i < (int)CIJ->size2; i++) {
		gsl_vector_const_view CIJ_col_i = gsl_matrix_const_column(CIJ, i);
		gsl_vector_set(_id, i, nnz(&CIJ_col_i.vector));
	}
	
	// od = sum(CIJ,2);
	gsl_vector* _od = gsl_vector_alloc(CIJ->size1);
	for (int i = 0; i < (int)CIJ->size1; i++) {
		gsl_vector_const_view CIJ_col_i = gsl_matrix_const_row(CIJ, i);
		gsl_vector_set(_od, i, nnz(&CIJ_col_i.vector));
	}
	
	// deg = id+od;
	gsl_vector* deg = copy(_id);
	gsl_vector_add(deg, _od);
	
	if (id != NULL) *id = _id; else gsl_vector_free(_id);
	if (od != NULL) *od = _od; else gsl_vector_free(_od);
	return deg;
}
