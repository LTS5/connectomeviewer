#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>
#include <vector>

DEFUN_DLD(findwalks_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 1) {
		return octave_value_list();
	}
	Matrix CIJ = args(0).matrix_value();
	if (!error_state) {
		gsl_matrix* CIJ_gsl = bct_test::to_gslm(CIJ);
		gsl_vector* wlq;
		std::vector<gsl_matrix*> Wq = bct::findwalks(CIJ_gsl, &wlq);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(Wq, 1));
		ret(1) = octave_value(bct_test::from_gsl(wlq, 1));
		gsl_matrix_free(CIJ_gsl);
		gsl_vector_free(wlq);
		bct::gsl_free(Wq);
		return ret;
	} else {
		return octave_value_list();
	}
}
