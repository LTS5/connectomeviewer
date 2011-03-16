#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(makefractalCIJ_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 3) {
		return octave_value_list();
	}
	int mx_lvl = args(0).int_value();
	double E = args(1).double_value();
	int sz_cl = args(2).int_value();
	if (!error_state) {
		int K;
		gsl_matrix* CIJ = bct::makefractalCIJ(mx_lvl, E, sz_cl, &K);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(CIJ));
		ret(1) = octave_value(K);
		gsl_matrix_free(CIJ);
		return ret;
	} else {
		return octave_value_list();
	}
}
