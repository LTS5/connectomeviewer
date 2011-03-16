#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>
#include <vector>

DEFUN_DLD(findpaths_cpp, args, , "Wrapper for C++ function.") {
	if (args.length() != 3) {
		return octave_value_list();
	}
	Matrix CIJ = args(0).matrix_value();
	Matrix sources = args(1).matrix_value();
	int qmax = args(2).int_value();
	if (!error_state) {
		gsl_matrix* CIJ_gsl = bct_test::to_gslm(CIJ);
		gsl_vector* sources_gsl = bct_test::to_gslv(sources);
		gsl_vector_add_constant(sources_gsl, -1.0);
		gsl_vector* plq;
		int qstop;
		gsl_matrix* allpths;
		gsl_matrix* util;
		std::vector<gsl_matrix*> Pq = bct::findpaths(CIJ_gsl, sources_gsl, qmax, &plq, &qstop, &allpths, &util);
		gsl_matrix_add_constant(allpths, 1.0);
		octave_value_list ret;
		ret(0) = octave_value(bct_test::from_gsl(Pq, 1));
		ret(1) = octave_value(bct_test::from_gsl(plq, 1));
		ret(2) = octave_value(qstop);
		ret(3) = octave_value(bct_test::from_gsl(allpths));
		ret(4) = octave_value(bct_test::from_gsl(util, 0, 1));
		gsl_matrix_free(CIJ_gsl);
		gsl_vector_free(sources_gsl);
		gsl_vector_free(plq);
		gsl_matrix_free(allpths);
		gsl_matrix_free(util);
		bct::gsl_free(Pq);
		return ret;
	} else {
		return octave_value_list();
	}
}
