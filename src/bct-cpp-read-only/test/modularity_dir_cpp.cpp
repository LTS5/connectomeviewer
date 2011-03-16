#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>

DEFUN_DLD(modularity_dir_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 1) {
		return octave_value_list();
	}
	Matrix A = args(0).matrix_value();
	if (!error_state) {
		gsl_matrix* A_gsl = bct_test::to_gslm(A);
		gsl_vector* Ci;
		double Q = bct::modularity_dir(A_gsl, &Ci);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(Ci));
		ret(1) = octave_value(Q);
		gsl_matrix_free(A_gsl);
		gsl_vector_free(Ci);
		return ret;
	} else {
		return octave_value_list();
	}
}
