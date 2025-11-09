#ifndef __MATRIX_CLASS__
#define __MATRIX_CLASS__

#include <ac_int.h>
#include "matrix_mult.h"

// Class is implemented as a template class that takes as its template parameters data type of matrix elements element_t and matrix dimension N
template<typename element_t, int N>

	class Matrix {
		
		private:
		
			// Elements of NxN matrix are of type element_t
			element_t elements[N][N];
			
		public:
		
			// Appropriate bit-accurate data types using helper class to calculate the required bit-width based on N
			static const int W = ac::log2_ceil<N>::val;
			using bit_acc = ac_int<W, false>;
			
			// Provide the public member functions:
			
			// Construct a matrix with all elements of zero (default constructor)
			Matrix() {
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
					  elements[i][j] = 0;
					}
				}
			}
			
			// Construct a matrix with elements copied from an NxN matrix "input_vals"
			Matrix(element_t input_vals[N][N]) {
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
					  elements[i][j] = input_vals[i][j];
					}
				}
			}
			
			// Set the value of element (i, j) with value "val"
			void setElement(bit_acc i, bit_acc j, element_t val) {
				elements[i][j] = val;
			}
			
			// Return the value of element (i, j)
			element_t getElement(bit_acc i, bit_acc j) const {
				return elements[i][j];
			}
			
			// The jth row ith column element of A^T is the ith row jth column element of A
			// ChatGPT 5 was used here to catch indexing error on the j loop
			void transpose() {
				for (int i = 0; i < N; i++)
				{
					for (int j = i+1; j < N; j++)
					{
						element_t tmp = elements[i][j];
						elements[i][j] = elements[j][i];
						elements[j][i] = tmp;
					}
				}
			}
			
			// Return type of the multiplication operator considers increased bit-width
			using mult_result_t = Matrix<hls_result_t, N>;
			
			// Multiplication operator, returns a matrix that contains the result of matrix multiplication with an input parameter matrix
			mult_result_t operator * (const Matrix& param) const {
				
				mult_result_t result_matrix;
				
				for (bit_acc i = 0; i < N; i++)
				{
					for (bit_acc j = 0; j < N; j++)
					{
						hls_result_t acc = 0;
						for (bit_acc k = 0; k < N; k++)
						{
							acc += hls_result_t(getElement(i, k) * param.getElement(k, j));
						}
						result_matrix.setElement(i, j, acc);
					}
				}
				return result_matrix;
			}
	};
	
#endif
