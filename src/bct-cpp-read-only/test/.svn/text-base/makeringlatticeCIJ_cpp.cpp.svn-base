#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(makeringlatticeCIJ_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 2) {
		return octave_value_list();
	}
	int N = args(0).int_value();
	int K = args(1).int_value();
	if (!error_state) {
		gsl_matrix* CIJ = bct::makeringlatticeCIJ(N, K);
		octave_value ret = bct_test::from_gsl(CIJ);
		gsl_matrix_free(CIJ);
		return ret;
	} else {
		return octave_value_list();
	}
}
