#include "matrix_mult.h"
#include <ac_assert.h>

void matrixMultHLS(hls_data_t A[N][N], hls_data_t B[N][N], hls_result_t C[N][N], ac_int<2, false> transpose) // Transpose bit width reduced
{
    // Check that transpose has legal value
    assert(transpose >= 0 && transpose < 4);
	
	// Case is selected inside the loops and not outside
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			hls_result_t C_temp = 0;
			for (int k = 0; k < N; k++)
			{
				// Select correct processing options using bit values
				if(transpose == 0) {
					C_temp += A[i][k] * B[k][j];
				}
				else if(transpose == 1) {
					C_temp += A[k][i] * B[k][j];
				}
				else if(transpose == 2) {
					C_temp += A[i][k] * B[j][k];
				}
				else if(transpose == 3) {
					C_temp += A[k][i] * B[j][k];
				}
			}
			C[i][j] = C_temp;
		}
	}            
}
