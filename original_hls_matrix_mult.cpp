#include "matrix_mult.h"
#include <ac_assert.h>

void matrixMultHLS(hls_data_t A[N][N], hls_data_t B[N][N], hls_result_t C[N][N], int transpose)
{
    // Check that transpose has legal value
    assert(transpose >= 0 && transpose < 4);
    
    // Transpose both A and B before multiplication
    if (transpose == 3)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[k][i] * B[j][k];
                }
                C[i][j] = C_temp;
            }
        }            
    }
    // Transpose B before multiplication
    else if (transpose == 2)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[i][k] * B[j][k];
                }
                C[i][j] = C_temp;
            }
        }            
    }
    // Transpose A before multiplication
    else if (transpose == 1)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[k][i] * B[k][j];
                }
                C[i][j] = C_temp;
            }
        }            
    }
    // No transposition 
    else if (transpose == 0)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                hls_result_t C_temp = 0;
                for (int k = 0; k < N; k++)
                {
                    C_temp += A[i][k] * B[k][j];
                }
                C[i][j] = C_temp;
            }
        }            
    }
}
