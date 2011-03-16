#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(makeevenCIJ_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 3) {
		return octave_value_list();
	}
	int N = args(0).int_value();
	int K = args(1).int_value();
	int sz_cl = args(2).int_value();
	if (!error_state) {
		gsl_matrix* CIJ = bct::makeevenCIJ(N, K, sz_cl);
		octave_value ret = bct_test::from_gsl(CIJ);
		gsl_matrix_free(CIJ);
		return ret;
	} else {
		return octave_value_list();
	}
}
