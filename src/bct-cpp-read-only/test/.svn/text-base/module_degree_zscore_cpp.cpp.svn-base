#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>

DEFUN_DLD(module_degree_zscore_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 2) {
		return octave_value_list();
	}
	Matrix A = args(0).matrix_value();
	Matrix Ci = args(1).matrix_value();
	if (!error_state) {
		gsl_matrix* A_gsl = bct_test::to_gslm(A);
		gsl_vector* Ci_gsl = bct_test::to_gslv(Ci);
		gsl_vector* Z = bct::module_degree_zscore(A_gsl, Ci_gsl);
		octave_value ret = bct_test::from_gsl(Z);
		gsl_matrix_free(A_gsl);
		gsl_vector_free(Ci_gsl);
		gsl_vector_free(Z);
		return ret;
	} else {
		return octave_value_list();
	}
}
