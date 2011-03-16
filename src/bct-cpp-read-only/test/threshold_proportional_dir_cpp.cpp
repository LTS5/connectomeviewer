#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(threshold_proportional_dir_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 2) {
		return octave_value_list();
	}
	Matrix W = args(0).matrix_value();
	double p = args(1).double_value();
	if (!error_state) {
		gsl_matrix* W_gsl = bct_test::to_gslm(W);
		gsl_matrix* W_thr = bct::threshold_proportional_dir(W_gsl, p);
		octave_value ret = bct_test::from_gsl(W_thr);
		gsl_matrix_free(W_gsl);
		gsl_matrix_free(W_thr);
		return ret;
	} else {
		return octave_value_list();
	}
}
