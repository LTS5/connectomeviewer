#include <bct/bct.h>
#include "bct_test.h"
#include <gsl/gsl_matrix.h>
#include <octave/oct.h>
#include <vector>

DEFUN_DLD(find_motif34_cpp, args, , "Wrapper for C++ function.") {
	bct::set_motif_mode(bct::SPORNS);
	if (args.length() == 1) {
		Matrix m = args(0).matrix_value();
		if (!error_state) {
			gsl_matrix* m_gsl = bct_test::to_gslm(m);
			int M = bct::find_motif34(m_gsl);
			octave_value ret = octave_value(M);
			gsl_matrix_free(m_gsl);
			return ret;
		} else {
			return octave_value_list();
		}
	} else if (args.length() == 2) {
		int m = args(0).int_value();
		int n = args(1).int_value();
		if (!error_state) {
			std::vector<gsl_matrix*> M = bct::find_motif34(m, n);
			octave_value ret = octave_value(bct_test::from_gsl(M));
			bct::gsl_free(M);
			return ret;
		} else {
			return octave_value_list();
		}
	} else {
		return octave_value_list();
	}
}
