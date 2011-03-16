#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>

DEFUN_DLD(motif3generate_cpp, args, , "Wrapper for C++ function.") {
	bct::set_motif_mode(bct::SPORNS);
	gsl_vector* ID;
	gsl_vector* N;
	gsl_matrix* M = bct::motif3generate(&ID, &N);
	octave_value_list ret;
	ret(0) = octave_value(bct_test::from_gsl(M));
	ret(1) = octave_value(bct_test::from_gsl(ID));
	ret(2) = octave_value(bct_test::from_gsl(N));
	gsl_vector_free(ID);
	gsl_vector_free(N);
	gsl_matrix_free(M);
	return ret;
}
