#ifndef BCT_TEST_H
#define BCT_TEST_H

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <octave/oct.h>
#include <vector>

namespace bct_test {
	Matrix from_gsl(const gsl_vector*, int = 0);
	Matrix from_gsl(const gsl_matrix*, int = 0, int = 0);
	NDArray from_gsl(const std::vector<gsl_matrix*>, int = 0);
	gsl_vector* to_gslv(const Matrix, int = 0);
	gsl_matrix* to_gslm(const Matrix, int = 0, int = 0);
	std::vector<gsl_matrix*> to_gsl(const NDArray, int = 0);
}

#include "bct_test.cpp"

#define MATRIX_TO_SCALAR_FUNCTION(function_name) \
	DEFUN_DLD(function_name##_cpp, args, , "Wrapper for C++ function.") { \
		if (args.length() == 0) { \
			return octave_value_list(); \
		} \
		Matrix m = args(0).matrix_value(); \
		if (!error_state) { \
			gsl_matrix* m_gsl = bct_test::to_gslm(m); \
			octave_value ret = octave_value(bct::function_name(m_gsl)); \
			gsl_matrix_free(m_gsl); \
			return ret; \
		} else { \
			return octave_value_list(); \
		} \
	}

#define MATRIX_TO_VECTOR_FUNCTION(function_name) \
	DEFUN_DLD(function_name##_cpp, args, , "Wrapper for C++ function.") { \
		if (args.length() == 0) { \
			return octave_value_list(); \
		} \
		Matrix m = args(0).matrix_value(); \
		if (!error_state) { \
			gsl_matrix* m_gsl = bct_test::to_gslm(m); \
			gsl_vector* ret_gsl = bct::function_name(m_gsl); \
			octave_value ret = bct_test::from_gsl(ret_gsl); \
			gsl_matrix_free(m_gsl); \
			gsl_vector_free(ret_gsl); \
			return ret; \
		} else { \
			return octave_value_list(); \
		} \
	}

#define MATRIX_TO_MATRIX_FUNCTION(function_name) \
	DEFUN_DLD(function_name##_cpp, args, , "Wrapper for C++ function.") { \
		if (args.length() == 0) { \
			return octave_value_list(); \
		} \
		Matrix m = args(0).matrix_value(); \
		if (!error_state) { \
			gsl_matrix* m_gsl = bct_test::to_gslm(m); \
			gsl_matrix* ret_gsl = bct::function_name(m_gsl); \
			octave_value ret = bct_test::from_gsl(ret_gsl); \
			gsl_matrix_free(m_gsl); \
			gsl_matrix_free(ret_gsl); \
			return ret; \
		} else { \
			return octave_value_list(); \
		} \
	}

#endif
