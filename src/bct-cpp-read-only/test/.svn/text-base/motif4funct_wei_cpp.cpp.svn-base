#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>

DEFUN_DLD(motif4funct_wei_cpp, args, , "Wrapper for C++ function.") {
	bct::set_motif_mode(bct::SPORNS);
	if (args.length() != 1) {
		return octave_value_list();
	}
	Matrix W = args(0).matrix_value();
	if (!error_state) {
		gsl_matrix* W_gsl = bct_test::to_gslm(W);
		gsl_matrix* Q;
		gsl_matrix* F;
		gsl_matrix* I = bct::motif4funct_wei(W_gsl, &Q, &F);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(I));
		ret(1) = octave_value(bct_test::from_gsl(Q));
		ret(2) = octave_value(bct_test::from_gsl(F));
		gsl_matrix_free(W_gsl);
		gsl_matrix_free(Q);
		gsl_matrix_free(F);
		gsl_matrix_free(I);
		return ret;
	} else {
		return octave_value_list();
	}
}
