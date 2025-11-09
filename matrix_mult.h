/*
Multiplies two NxN matrices A and B of type int and puts the product in matrix C.
Allows transposition of A and/or B before multiplication.

transpose = 0: Use A and B as is
transpose = 1: Transpose A before multiplication
transpose = 2: Transpose B before multiplication
transpose = 3: Transpose A and B before multiplication
*/

#ifndef _matrix_mult
#define _matrix_mult

#include <ac_int.h>

// Matrix size NxN
const int N = 8;

const int MOST_DEMANDING_BOUNDARY = 32; // Reason which boundary value determines this to not cause overflow

const int WIDTH = ac::log2_ceil<MOST_DEMANDING_BOUNDARY>::val+1;

const int RESULT_WIDTH = ac::log2_ceil<N>::val+2*WIDTH;

using data_t = int;
using result_t = int;
using hls_data_t = ac_int<WIDTH, true>;
using hls_result_t = ac_int<RESULT_WIDTH, true>;

// Golden reference implementation
void matrixMult(data_t A[N][N], data_t B[N][N], result_t C[N][N], int transpose);

// HLS implementation
void matrixMultHLS(hls_data_t A[N][N], hls_data_t B[N][N], hls_result_t C[N][N], ac_int<2, false> transpose);

#endif
