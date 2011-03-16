#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>

DEFUN_DLD(makerandCIJdegreesfixed_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 2) {
		return octave_value_list();
	}
	Matrix in = args(0).matrix_value();
	Matrix out = args(1).matrix_value();
	if (!error_state) {
		gsl_vector* in_gsl = bct_test::to_gslv(in);
		gsl_vector* out_gsl = bct_test::to_gslv(out);
		gsl_matrix* cij = bct::makerandCIJdegreesfixed(in_gsl, out_gsl);
		octave_value_list ret;
		if (cij == NULL) {
			ret(0) = octave_value(0);
			ret(1) = octave_value(0);
		} else {
			ret(0) = octave_value(bct_test::from_gsl(cij));
			ret(1) = octave_value(1);
			gsl_matrix_free(cij);
		}
		gsl_vector_free(in_gsl);
		gsl_vector_free(out_gsl);
		return ret;
	} else {
		return octave_value_list();
	}
}
