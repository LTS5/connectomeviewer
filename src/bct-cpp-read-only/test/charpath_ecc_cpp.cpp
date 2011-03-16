#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>

DEFUN_DLD(charpath_ecc_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 1) {
		return octave_value_list();
	}
	Matrix D = args(0).matrix_value();
	if (!error_state) {
		gsl_matrix* D_gsl = bct_test::to_gslm(D);
		double radius;
		double diameter;
		gsl_vector* ecc = bct::charpath_ecc(D_gsl, &radius, &diameter);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(ecc));
		ret(1) = octave_value(radius);
		ret(2) = octave_value(diameter);
		gsl_matrix_free(D_gsl);
		gsl_vector_free(ecc);
		return ret;
	} else {
		return octave_value_list();
	}
}
