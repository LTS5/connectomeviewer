#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(reachdist_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 1) {
		return octave_value_list();
	}
	Matrix CIJ = args(0).matrix_value();
	if (!error_state) {
		gsl_matrix* CIJ_gsl = bct_test::to_gslm(CIJ);
		gsl_matrix* D;
		gsl_matrix* R = bct::reachdist(CIJ_gsl, &D);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(R));
		ret(1) = octave_value(bct_test::from_gsl(D));
		gsl_matrix_free(CIJ_gsl);
		gsl_matrix_free(D);
		gsl_matrix_free(R);
		return ret;
	} else {
		return octave_value_list();
	}
}
