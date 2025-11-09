#include "matrix_mult.h"
#include "matrix_class.h"
#include <ac_assert.h>

void matrixMultHLS(hls_data_t A[N][N], hls_data_t B[N][N], hls_result_t C[N][N], ac_int<2, false> transpose) // Transpose bit width reduced
{
    // Check that transpose has legal value
    assert(transpose >= 0 && transpose < 4);
	
	// Create matrix objects from the input matrix data
	Matrix<hls_data_t, N> input_A(A);
	Matrix<hls_data_t, N> input_B(B);
	
	// Check transpose option and proceed accordingly. With transpose = 0, no transpose operation is done
	if(transpose == 1) {
		input_A.transpose();
	}
	else if(transpose == 2) {
		input_B.transpose();
	}
	else if(transpose == 3) {
		input_A.transpose();
		input_B.transpose();
	}
	
	// Matrix multiplication
	Matrix<hls_result_t, N> output_C = input_A*input_B;
	
	// Data from the multiplication result matrix is copied to the output array
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			C[i][j] = output_C.getElement(i, j);
		}
	}
}
