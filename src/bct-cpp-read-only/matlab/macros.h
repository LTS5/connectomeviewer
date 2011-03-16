#undef FP_TYPE
#undef FP_ID
#undef VECTOR_TYPE
#undef VECTOR_ID
#undef MATRIX_TYPE
#undef MATRIX_ID

#ifdef GSL_FLOAT
#define FP_TYPE float
#define FP_ID(id) id##_##float
#define VECTOR_TYPE gsl_vector_float
#define VECTOR_ID(id) gsl_vector_float##_##id
#define MATRIX_TYPE gsl_matrix_float
#define MATRIX_ID(id) gsl_matrix_float##_##id
#endif

#ifdef GSL_DOUBLE
#define FP_TYPE double
#define FP_ID(id) id##_##double
#define VECTOR_TYPE gsl_vector
#define VECTOR_ID(id) gsl_vector##_##id
#define MATRIX_TYPE gsl_matrix
#define MATRIX_ID(id) gsl_matrix##_##id
#endif

#ifdef GSL_LONG_DOUBLE
#define FP_TYPE long double
#define FP_ID(id) id##_##long_double
#define VECTOR_TYPE gsl_vector_long_double
#define VECTOR_ID(id) gsl_vector_long_double##_##id
#define MATRIX_TYPE gsl_matrix_long_double
#define MATRIX_ID(id) gsl_matrix_long_double##_##id
#endif
