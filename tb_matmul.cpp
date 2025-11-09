// IO library to print into file
#include <fstream> // output file manipulation
#include <cstdlib> // random input value generation
#include <ctime>   // to randomize the seed also

// This library must be included to use the macros below
#include "mc_scverify.h"
#include "matrix_mult.h"

CCS_MAIN(int argc, char *argv[]) // The Catapult testbench should always be invoked with this macro
{
  // Use this variable as a return value from the testbench (0 indicates passing the testbench, 1 failing it)
  int ret = 0; // Change value to 1 if test fails

  // Create variables to be passed to the design
  data_t       A[N][N]      = {0};
  data_t       B[N][N]      = {0};
  hls_data_t   A_hls[N][N]  = {0};
  hls_data_t   B_hls[N][N]  = {0};
  result_t     C[N][N];
  hls_result_t C_hls[N][N];
  int transpose = 0;
  bool error = false;

  std::ofstream file;
  file.open("matrix_mult_output.txt"); // matrix_mult_output.txt opened for writing

  // ========================== ZERO MATRIX TEST ==========================
  // Make BOTH paths identical for the zero test
  for (int u = 0; u < N; ++u) {
    for (int v = 0; v < N; ++v) {
      A[u][v]     = 0;
      B[u][v]     = 0;
      A_hls[u][v] = hls_data_t(0); // explicit init for ac_int
      B_hls[u][v] = hls_data_t(0);
    }
  }

  // First test case uses initial fixed values so all input values are zero. Afterwards test cases overwrite initial values.
  // Results of the tests are printed to matrix_mult_output.txt file.
  // File is made readable and informative by adding some clarificating lines, linebreaks and spaces.

  // Fixed value test case
  file << "=============== TEST CASE: ZERO MATRIX ===================\n";

  file << "A INPUTS\n";
  for (int h = 0; h < N; h++) {
    file << "=============== ROW NUMBER " << h << " ==================\n";
    for (int l = 0; l < N; l++) {
      file << A[h][l] << " <- REFERENCE ";
      file << A_hls[h][l].to_int() << " <- MODIFIED\n";
    }
    file << '\n';
  }

  file << "B INPUTS\n";
  for (int h = 0; h < N; h++) {
    file << "=============== ROW NUMBER " << h << " ==================\n";
    for (int l = 0; l < N; l++) {
      file << B[h][l] << " <- REFERENCE ";
      file << B_hls[h][l].to_int() << " <- MODIFIED\n";
    }
    file << '\n';
  }

  file << "OUTPUTS\n";

  // Branch coverage (each transpose branch 0-3 should be tested)
  for (int i = 0; i < 4; i++) {

    transpose = i;

    // This Catapult macro calls the design and passes the variables to it
    // Do not use this macro for calling the golden reference function

    // DUT modified
    CCS_DESIGN(matrixMultHLS)(A_hls, B_hls, C_hls, transpose);

    // DUT original
    matrixMult(A, B, C, transpose);

    file << "================ TRANSPOSE BRANCH " << transpose << " ==================\n";

    // Determine row to be tested
    for (int j = 0; j < N; j++) {
      file << "=============== ROW NUMBER " << j << " ==================\n";
      // Determine a column from a row to be tested (=values to be compared)
      for (int k = 0; k < N; k++) {
        // Test if values match
        if (C_hls[j][k] != C[j][k]) {
          // If values don't match, output error and set variable error status ready to return error
          file << "Error: values on row " << j << " on column " << k << " don't match. Modified value is "
               << C_hls[j][k].to_int() << " and original value is " << C[j][k] << ' ';
          error = true;
        } else {
          // If values match, just output the correct value from any of the tested files (now original file C used)
          file << C[j][k] << ' ';
        }
      }
      file << '\n';
    }
    file << '\n';
  }

  // ========================== CONTROL TEST (unchanged) ==========================
  for (int u = 0; u < N; u++) {
    for (int v = 0; v < N; v++) {
      A[u][v]     = u + v;
      B[u][v]     = u - v;
      A_hls[u][v] = u + v;
      B_hls[u][v] = u - v;
    }
  }

  file << "=============== TEST CASE: CONTROL ===================\n";

  file << "A INPUTS\n";
  for (int h = 0; h < N; h++) {
    file << "=============== ROW NUMBER " << h << " ==================\n";
    for (int l = 0; l < N; l++) {
      file << A[h][l] << " <- REFERENCE ";
      file << A_hls[h][l].to_int() << " <- MODIFIED\n";
    }
    file << '\n';
  }

  file << "B INPUTS\n";
  for (int h = 0; h < N; h++) {
    file << "=============== ROW NUMBER " << h << " ==================\n";
    for (int l = 0; l < N; l++) {
      file << B[h][l] << " <- REFERENCE ";
      file << B_hls[h][l].to_int() << " <- MODIFIED\n";
    }
    file << '\n';
  }

  file << "OUTPUTS\n";

  for (int i = 0; i < 4; i++) {

    transpose = i;

    CCS_DESIGN(matrixMultHLS)(A_hls, B_hls, C_hls, transpose);

    matrixMult(A, B, C, transpose);

    file << "================ TRANSPOSE BRANCH " << transpose << " ==================\n";

    for (int j = 0; j < N; j++) {
      file << "=============== ROW NUMBER " << j << " ==================\n";
      for (int k = 0; k < N; k++) {
        if (C_hls[j][k] != C[j][k]) {
          file << "Error: values on row " << j << " on column " << k << " don't match. Modified value is "
               << C_hls[j][k].to_int() << " and original value is " << C[j][k] << ' ';
          error = true;
        } else {
          file << C[j][k] << ' ';
        }
      }
      file << '\n';
    }
    file << '\n';
  }

  // ========================== RANDOM TEST (synced inputs) ==========================
  std::srand(std::time(nullptr));
  for (int u = 0; u < N; u++) {
    for (int v = 0; v < N; v++) {
      int a_val = (std::rand() % 17) - 8;
      int b_val = (std::rand() % 17) - 8;
      A[u][v]     = a_val;
      B[u][v]     = b_val;
      A_hls[u][v] = a_val; // same values for HLS path
      B_hls[u][v] = b_val;
    }
  }

  file << "=============== TEST CASE: RANDOM ===================\n";
  file << "A INPUTS\n";
  for (int h = 0; h < N; h++) {
    file << "=============== ROW NUMBER " << h << " ==================\n";
    for (int l = 0; l < N; l++) {
      file << A[h][l] << " <- REFERENCE ";
      file << A_hls[h][l].to_int() << " <- MODIFIED\n";
    }
    file << '\n';
  }

  file << "B INPUTS\n";
  for (int h = 0; h < N; h++) {
    file << "=============== ROW NUMBER " << h << " ==================\n";
    for (int l = 0; l < N; l++) {
      file << B[h][l] << " <- REFERENCE ";
      file << B_hls[h][l].to_int() << " <- MODIFIED\n";
    }
    file << '\n';
  }

  file << "OUTPUTS\n";

  for (int i = 0; i < 4; i++) {

    transpose = i;

    CCS_DESIGN(matrixMultHLS)(A_hls, B_hls, C_hls, transpose);

    matrixMult(A, B, C, transpose);

    file << "================ TRANSPOSE BRANCH " << transpose << " ==================\n";

    for (int j = 0; j < N; j++) {
      file << "=============== ROW NUMBER " << j << " ==================\n";
      for (int k = 0; k < N; k++) {
        if (C_hls[j][k] != C[j][k]) {
          file << "Error: values on row " << j << " on column " << k << " don't match. Modified value is "
               << C_hls[j][k].to_int() << " and original value is " << C[j][k] << ' ';
          error = true;
        } else {
          file << C[j][k] << ' ';
        }
      }
      file << '\n';
    }
    file << '\n';
  }

  // determine return value
  if (error) {
    ret = 1; // test failed
    file << "FAIL";
  } else {
    ret = 0; // test passed
    file << "PASS";
  }

  file.close(); // close file after manipulations are done

  CCS_RETURN(ret); // end with this macro
}
