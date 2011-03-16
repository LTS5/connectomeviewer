#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>

DEFUN_DLD(modularity_und_louvain_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 1) {
		return octave_value_list();
	}
	Matrix W = args(0).matrix_value();
	if (!error_state) {
		gsl_matrix* W_gsl = bct_test::to_gslm(W);
		gsl_vector* Ci;
		double Q = bct::modularity_und_louvain(W_gsl, &Ci);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(Ci));
		ret(1) = octave_value(Q);
		gsl_matrix_free(W_gsl);
		gsl_vector_free(Ci);
		return ret;
	} else {
		return octave_value_list();
	}
}
