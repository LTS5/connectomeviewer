#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(erange_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 1) {
		return octave_value_list();
	}
	Matrix CIJ = args(0).matrix_value();
	if (!error_state) {
		gsl_matrix* CIJ_gsl = bct_test::to_gslm(CIJ);
		double eta;
		gsl_matrix* Eshort;
		double fs;
		gsl_matrix* Erange = bct::erange(CIJ_gsl, &eta, &Eshort, &fs);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(Erange));
		ret(1) = octave_value(eta);
		ret(2) = octave_value(bct_test::from_gsl(Eshort));
		ret(3) = octave_value(fs);
		gsl_matrix_free(CIJ_gsl);
		gsl_matrix_free(Eshort);
		gsl_matrix_free(Erange);
		return ret;
	} else {
		return octave_value_list();
	}
}
