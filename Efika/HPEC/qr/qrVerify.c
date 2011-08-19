/******************************************************************************
** File: qrVerify.c
**
** HPEC Challenge Benchmark Suite
** QR Kernel Benchmark
**
** Contents:
**  The verification program for the ANSI C Complex Fast Givens QR Kernel.
**  The following program will verify that the QR Factorization C Kernel gave
**  correct outputs by verifying that Q*R=A, and Q*Q'=I.  The qrVerify
**  program will output "QR Verification: Passed" upon successful 
**  verification, or  "QR Verification: Failed" if any errors are found.
**
** Input:
**  The input matrix is stored in file "./data/<DataSetNum>-qr-inmatrix.dat".
**
** Command:
**   qrVerify <DataSetNum> 
**
** Author: Ryan Haney
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "PcaCArray.h"

/*
** Function: compute_allowable_error
**
** The following function computes the maximum error that we'll allow in 
** our verification of Q*R=A and Q*Q'=I.  Our allowable error is 
** computed as a function of the maximum value in our input, the single 
** precision floating point epsilon, the largest dimension in our input 
** matrix, and a reasonable factor for single precision calculations.
*/
float compute_allowable_error(int rows, int cols, ComplexFloat * A)
{
  int i, j;
  float temp, max;
  float allowable_error = 0;
  ComplexFloat * ptr = A;

  /* The "reasonable factor" variable is used to offset the accumulation */
  /* of error seen while performing the QR factorization with single     */
  /* precision floating point numbers.                                   */
  const float reasonable_factor = 5.0;

  /* Compute the maximum value in the input matrix. */
  max = 0.0;
  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++) {
      /* Find the absolute value of the current complex element. */
      temp = sqrt(ptr->r*ptr->r + ptr->i*ptr->i);

      /* Compare this value with the max. */
      if(temp > max) {
	max = temp;
      }
      /* Move the pointer on to the next element in the matrix. */
      ptr++;
    }
  }

  /* Our allowable error is computed as a function of the maximum value in */
  /* our input, the single precision floating point epsilon, the largest   */
  /* dimension in our input matrix, and a reasonable factor for single     */
  /* precision calculations.                                               */
  allowable_error = FLT_EPSILON * max * (float)(rows) * reasonable_factor;

#ifdef VERBOSE
  printf("Maximum error tolerance: %f\n", allowable_error);
#endif

  return allowable_error;
}


/* Function: hermitian
**
** Compute the hermitian (complex conjugate transpose) of the mxn matrix X.
*/
void hermitian(int m, int n, ComplexFloat * X_herm, ComplexFloat * X)
{
  /* Loop counters */
  int i, j;
  
  /* Data pointers */
  ComplexFloat * x = X;
  ComplexFloat * x_herm = X_herm;

  /* Loop over the rows of X (columns of X_herm). */
  for(i = 0; i < m; i++) {

    /* Advance to the ith row of X_herm. */
    x_herm = X_herm + i*m;

    /* Loop over the columns of X (rows of X_herm), advancing the X_herm */
    /* pointer across the row.                                           */
    for(j = 0; j < n; j++, x_herm++) {

      /* Set the x pointer to X[j][i]. */
      x = X + j * n + i;
      
      /* Compute and set the current X_herm[i][j] value to the complex */ 
      /* conjugate of the current X[j][i] value.                       */
      x_herm->r = x->r;
      x_herm->i = x->i * -1.0;
    }
  }

  return;
}


/* Function: matrix_multiply
**
** The following function computes the mxp matrix, Z=X*Y, given a mxn matrix 
** X and a nxp matrix Y.
*/
void matrix_multiply(int m, int n, int p, ComplexFloat *Z, ComplexFloat *X,
		     ComplexFloat *Y)
{
  /* Loop counters */
  int i, j, k;
  
  /* Data pointers */
  ComplexFloat * x = X;
  ComplexFloat * y = Y;
  ComplexFloat * z = Z;

  /* Loop over the first dimension: m. */
  for(i = 0; i < m; i++) {

    /* Set the Z pointer to the beginning of the ith row. */
    z = Z + i*p;

    /* Loop over the last dimension: p. */
    for(j = 0; j < p; j++) {
      /* Initialize the current value of Z to zero for accumulation. */
      z->r = 0.0;
      z->i = 0.0;
      
      /* Set the X pointer to the beginning of the ith row, and the Y */
      /* pointer to the top of the jth column.                        */
      x = X + i*n;
      y = Y + j;

      /* Loop over the middle dimension: n */
      for(k = 0; k < n; k++) {

	/* Multiply and accumulate our two current values of X and Y. */
	z->r += (x->r*y->r) - (x->i*y->i);
	z->i += (x->r*y->i) + (x->i*y->r);

	/* Advance across the row of X and down the column of Y. */
	x++;
	y += p;
      }
      
      /* Advance to the next value in the current row of Z. */
      z++;
    }
  }
  
  return;
}


/* Function: initialize_I
**
** Initialize the mxn matrix I to the identity matrix.
*/
void initialize_I(int m, int n, ComplexFloat * I)
{
  int i, j;
  ComplexFloat one, zero;

  /* Data pointer */
  ComplexFloat * I_data = I;

  /* Create a ComplexFloat value equal to 1 and 0. */
  one.r = 1.0;
  one.i = 0.0;
  zero.r = 0.0;
  zero.i = 0.0;
  
  /* Initialize I to the identity matrix. */
  for(i = 0; i < m; i++) {
    for(j = 0; j < n; j++, I_data++) {
      if(i == j) {
	*I_data = one;
      }
      else {
	*I_data = zero;
      }
    }
  }
}


/*
** Function: verify
**
** The following function verifies that the QR factorization C Kernel 
** generated the correct matrices, Q and R, where Q*R=A and Q*Q'=I.
** To test this, we'll look for any values in the resulting matrices of 
** abs(A-Q*R) and abs(I-Q*Q') larger than our calculated allowable error.
**
** This function will return 1 if the test passed and 0 if it failed.
*/
int verify(int rows, int cols, float allowable_error,
	   /* QR kernel input matrix. */
	   struct ComplexFloat * A,
	   /* Output matrices from the QR kernel. */
	   struct ComplexFloat * Q, struct ComplexFloat * R)	   
{
  /* Loop counters, temporaries */
  int i, j;
  float temp;

  /* Flag to tell whether or not the verification passed. */
  int passed = 1;

  /* Variable to hold the maximum error from each test. */
  float max_error = 0.0;

  /* Pointer for the resulting matrix of Q*R. */
  ComplexFloat * Q_x_R;

  /* Pointer for the resulting matrix of Q'. */
  ComplexFloat * Q_herm;
  
  /* Pointer for the resulting matrix of Q*Q'. */
  ComplexFloat * Q_x_Q_herm;

  /* Pointer for the identity matrix, I. */
  ComplexFloat * I;

  /* Data pointers for the input and product matrices. */
  ComplexFloat *ptr1, *ptr2;

  /* Allocate memory for our product matrices, Q_x_R and Q_x_herm_Q. */
  Q_x_R = (struct ComplexFloat *)malloc(sizeof(struct ComplexFloat)*rows*cols);
  Q_herm = (struct ComplexFloat *)malloc(sizeof(struct ComplexFloat)
					 *rows*rows);
  Q_x_Q_herm = (struct ComplexFloat *)malloc(sizeof(struct ComplexFloat)
					     *rows*rows);

  /* Allocate memory for the identity matrix, I. */
  I = (struct ComplexFloat *)malloc(sizeof(struct ComplexFloat)*rows*rows);

  /* Compute the product Q*R. */
  matrix_multiply(rows, rows, cols, Q_x_R, Q, R);

  /* Compute the matrix Q'. */
  hermitian(rows, rows, Q_herm, Q);

  /* Compute the product Q*Q'. */
  matrix_multiply(rows, rows, rows, Q_x_Q_herm, Q, Q_herm);

  /* Set I to the identity matrix. */
  initialize_I(rows, rows, I);

  /* Verify the correctness of A-Q*R = 0. */
  ptr1 = A;
  ptr2 = Q_x_R;
  for(i = 0; i < rows; i++) {
    if(!passed) {
      break;
    }
    for(j = 0; j < cols; j++) {
      /* Check to see that the absolute value of the difference       */
      /* between the two current values of our matrices is within our */
      /* allowable error.                                             */
      temp = sqrt((ptr1->r - ptr2->r)*(ptr1->r - ptr2->r)
		  + (ptr1->i - ptr2->i)*(ptr1->i - ptr2->i));
      if(temp > allowable_error) {
#ifdef VERBOSE
	printf("Q*R=A verification failed: Element %d, %d\n", i, j);
	printf("Error: %f\n", temp);
#endif

	/* The test failed.  Return 0. */
	passed = 0;
	break;
      }
      /* Record the maximum error from this test. */
      else if(temp > max_error) {
	max_error = temp;
      }

      ptr1++;
      ptr2++;
    }
  }

#ifdef VERBOSE
  if(passed) {
    printf("Maximum error value from A-Q*R test: %f\n", max_error);
  }
#endif

  /* Verify the correctness of I-Q*Q' = 0. */
  max_error = 0.0;
  ptr1 = I;
  ptr2 = Q_x_Q_herm;
  for(i = 0; i < rows; i++) {
    if(!passed) {
      break;
    }
    for(j = 0; j < rows; j++) {
      /* Check to see that the absolute value of the difference       */
      /* between the two current values of our matrices is within our */
      /* allowable error.                                             */
      temp = sqrt((ptr1->r - ptr2->r)*(ptr1->r - ptr2->r)
		  + (ptr1->i - ptr2->i)*(ptr1->i - ptr2->i));
      if(temp > allowable_error) {
#ifdef VERBOSE
	printf("Q*Q'=I verification failed: Element %d, %d\n", i, j);
	printf("Error: %f\n", temp);
#endif

	/* The test failed.  Return 0. */
	passed = 0;
	break;
      }
      /* Record the maximum error from this test. */
      else if(temp > max_error) {
	max_error = temp;
      }

      ptr1++;
      ptr2++;
    }
  }

#ifdef VERBOSE
  if(passed) {
    printf("Maximum error value from I-Q*Q' test: %f\n", max_error);
  }
#endif

  /* Free allocated memory */
  free(Q_x_R);
  free(Q_herm);
  free(Q_x_Q_herm);
  free(I);

  return passed;
}


/*
** Function: main
**
** Setup data for and run the QR verification.
*/
int main(int argc, char **argv)
{
  /* Containers for the QR's input and output matrices. */
  PcaCArrayFloat in_matrix, out_matrix_q, out_matrix_r;

  /* Strings for our input and output file names. */
  char           *in_matrix_file, *out_matrix_file_q, *out_matrix_file_r;
  
  /* An allowable error for our computation is computed by the function, */
  /* compute_allowable_error.                                            */
  float          allowable_error;

  /* Build the input file names.  If no arguments are specified, */
  /* show the user the proper usage of the program.              */
  if (argc == 2) {
    /* Allocate memory for the file names. */
    in_matrix_file =    (char*) malloc(strlen("./data/") + strlen(argv[1]) +
				       strlen("-qr-inmatrix.dat") + 1); 
    out_matrix_file_q = (char*) malloc(strlen("./data/") + strlen(argv[1]) +
				       strlen("-qr-outmatrix_q.dat") + 1); 
    out_matrix_file_r = (char*) malloc(strlen("./data/") + strlen(argv[1]) +
				       strlen("-qr-outmatrix_r.dat") + 1); 

    /* Create the file names. */
    sprintf(in_matrix_file, "./data/%s-qr-inmatrix.dat", argv[1]);
    sprintf(out_matrix_file_q, "./data/%s-qr-outmatrix_q.dat", argv[1]);
    sprintf(out_matrix_file_r, "./data/%s-qr-outmatrix_r.dat", argv[1]);
  }
  else {
    printf("No data set specified.\n");
    printf("Usage: qrVerify <DataSetNum>\n");
    exit(-1);
  }

  /* Read in the input matrix from file.  We'll use this to compute     */
  /* an allowable error, as well as to check the results of the kernel. */
  readFromFile(float, in_matrix_file, in_matrix);

  /* Read in the output matrices from the QR kernel. */
  readFromFile(float, out_matrix_file_q, out_matrix_q);
  readFromFile(float, out_matrix_file_r, out_matrix_r);

  /* Check that the dimensions for the input output matrices match       */
  /* in case the user accidentally overwrote the data set with different */
  /* data set sizes.                                                     */
  if(in_matrix.size[0] != out_matrix_r.size[0]
     || in_matrix.size[1] != out_matrix_r.size[1]
     || in_matrix.size[0] != out_matrix_q.size[0]) {
#ifdef VERBOSE
    printf("Error: Input and output matrix dimensions do not match.\n");
    printf("Please re-run qrGenerator and qr.\n");
#endif
    exit(-1);
  }

  /* Compute the allowable error. */
  allowable_error = 
    compute_allowable_error(in_matrix.size[0], in_matrix.size[1],
			    (struct ComplexFloat *)&in_matrix.data[0]);

  /* Run the verification function. */
  printf("Verification: ");
  if (verify(in_matrix.size[0], in_matrix.size[1], allowable_error,
	     (struct ComplexFloat *)&in_matrix.data[0],
	     (struct ComplexFloat *)&out_matrix_q.data[0], 
	     (struct ComplexFloat *)&out_matrix_r.data[0])) {
    printf("PASS\n");
  }
  else {
    printf("FAIL\n");
  }

  /* Clean up allocated memory. */
  free(in_matrix_file);
  free(out_matrix_file_q);
  free(out_matrix_file_r);
  clean_mem(float, in_matrix);
  clean_mem(float, out_matrix_q);
  clean_mem(float, out_matrix_r);

  return 0;
}
/* ----------------------------------------------------------------------------
Copyright (c) 2006, Massachusetts Institute of Technology
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are  
met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the Massachusetts Institute of Technology nor  
       the names of its contributors may be used to endorse or promote 
       products derived from this software without specific prior written 
       permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF  
THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------- */
