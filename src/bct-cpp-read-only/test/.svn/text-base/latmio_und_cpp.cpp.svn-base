#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(latmio_und_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 2) {
		return octave_value_list();
	}
	Matrix R = args(0).matrix_value();
	int ITER = args(1).int_value();
	if (!error_state) {
		gsl_matrix* R_gsl = bct_test::to_gslm(R);
		gsl_matrix* _R = bct::latmio_und(R_gsl, ITER);
		octave_value ret = bct_test::from_gsl(_R);
		gsl_matrix_free(R_gsl);
		gsl_matrix_free(_R);
		return ret;
	} else {
		return octave_value_list();
	}
}
