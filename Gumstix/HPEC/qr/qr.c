/******************************************************************************
** File: qr.c
**
** HPEC Challenge Benchmark Suite
** QR Kernel Benchmark
**
** Contents:
**  The ANSI C Complex Fast Givens QR kernel.  For more information on the 
**  Fast Givens QR algorithm, see Golub and Van Loan's "Matrix Computations," 
**  sections 5.1.13 and 5.2.5.
**
** Input/Output:
**  The input matrix is stored in file "./data/<DataSetNum>-qr-inmatrix.dat".
**  The output matrices Q and R will be stored in the files 
**  "./data/<DataSetNum>-qr-outmatrix_q.dat" and 
**  "./data/<DataSetNum>-qr-outmatrix_r.dat"
**  respectively.  The total run time will be stored in file 
**  "./data/<DataSetNum>-qr-timing.dat".
**
** Command:
**   qr <DataSetNum>
**
** Author: Ryan Haney
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <stdio.h>
#include <math.h>

#include "PcaCArray.h"
#include "PcaCTimer.h"

/*
** Function: initialize_matrices
**
** Initialize the matrix D (because D is a diagonal matrix we store it as a
** one dimensional vector) to the identity matrix.  Also initialize the matrix 
** M to the identity matrix.  After we apply Fast Givens transformations to M, 
** and eventually multiply by the scaling matrix D, we'll form the output 
** matrix Q from M.  M will be stored in column-wise order.
**
** This function is considered to be part of the kernel setup and will 
** therefore not be included in the timing.
*/
void initialize_matrices(struct PcaCArrayFloat * M, struct PcaCArrayFloat * D)
{
  int i, j;
  struct ComplexFloat one, zero;
  struct ComplexFloat * M_data;
  float * D_data;

  /* Create a ComplexFloat value equal to 1 and zero. */
  one.r = 1.0;
  one.i = 0.0;
  zero.r = 0.0;
  zero.i = 0.0;
  
  /* Set pointers to the first values of M and d. */
  M_data = (struct ComplexFloat *)&M->data[0];
  D_data = (float *)&D->data[0];
  
  /* Initialize M to the identity matrix. */
  for(i = 0; i < M->size[0]; i++) {
    for(j = 0; j < M->size[1]; j++, M_data++) {
      if(i == j) {
	*M_data = one;
      }
      else {
	*M_data = zero;
      }
    }
  }
  /* Initialize D to the identity matrix (stored as a vector of ones). */
  for(i = 0; i < D->size[0]; i++, D_data++) {
    *D_data = 1.0;
  }
}

/*
** Function: qr
**
** Compute the Complex Fast Givens QR factorization of the input matrix A.
** The computation will be performed in-place.  Therefore, as we apply 
** transformations and scaling to the matrix A, we will compute the resulting
** matrix R.  Similarly, applying transformations and scaling to the matrix M,
** will produce the matrix Q.  Computations on the matrix M will be done 
** in-place.  But, the application of the scaling matrix, D, to the matrix M
** will be assigned to a new matrix Q.  This is done so we can store the matrix
** M in a column-wise fashion, and have our result, Q, in row-wise order.
**
** In summary, our data is stored as follows:
**    A: Row major matrix,
**    M: Column major matrix,
**    D: Diagonal matrix stored as a vector, and
**    Q: Row major matrix.
*/
void qr(int rows, int cols, ComplexFloat *A, ComplexFloat *M, 
	float * D, ComplexFloat * Q) 
{
  /* Loop counters */
  int i, j, k;

  /* Data pointers */
  struct ComplexFloat *a1, *a2, *a3, *a4;
  struct ComplexFloat *m1, *m2;
  float *d1, *d2;  
  struct ComplexFloat *q1;

  /* Fast givens transformation values and temporaries */
  ComplexFloat alpha, beta, tau;
  float gamma, temp;
  
  /* Transform type (1 or 2) */
  int type;

  /* Loop over columns of A. */
  for(j = 0; j < cols; j++) {

    /* Setup the data pointers.  a1 should point to A[m-2][j], while   */
    /* a2 should point to A[m-1][j].  d1 should point to D[m-2], while */
    /* d1 points to D[m-1].                                            */
    a1 = A+((rows-2)*cols+j);
    a2 = A+((rows-1)*cols+j);
    d1 = D+(rows-2);
    d2 = D+(rows-1);

    /* Loop from the last to the jth row of A (Up to the diagonal). */
    for(i = rows-1; i > j; i--) {

      /* ============================================================= */
      /* Compute the Fast Givens transformation matrix: Values alpha,  */
      /* beta, and what type of transform we're doing (1 or 2).        */
      /* ============================================================= */

      /* If the current value is zero, we have nothing to cancel out. */
      /* Therefore, we set alpha and beta to zero and use a type 2    */
      /* transformation...  Essentially we multiply by a 2x2          */
      /* identity matrix                                              */
      if(a2->r == 0 && a2->i == 0) {
	type = 2;
	alpha.r = 0.0;
	alpha.i = 0.0;
	beta = alpha;
      }
      /* The current value is not zero.  Therefore compute transform   */
      /* values alpha and beta that will zero out the current element. */
      else {
	/* Compute alpha = -a1/a2 */
	temp = a2->r*a2->r + a2->i*a2->i;
	alpha.r = -(a1->r*a2->r + a1->i*a2->i)/temp;
	alpha.i = -(a1->i*a2->r - a1->r*a2->i)/temp;
 
	/* Compute beta = -conj(alpha)*d2/d1 */
	temp = (*d2)/(*d1);
	beta.r = -alpha.r * temp;
	beta.i = alpha.i * temp;

	/* Compute gamma = -alpha*beta...  We'll simplify this to        */
	/* -alpha*-conj(alpha)*(d2/d1) = (alpha.r^2 + alpha.i^2)*(d2/d1) */
	gamma = (alpha.r*alpha.r + alpha.i*alpha.i)*temp;

	/* Check the gamma value.  If it's over 1 we'll take the     */
	/* reciprocal of our alpha, beta, and gamma values to retain */
	/* numerical stability.                                      */
	if(gamma <= 1.0) {
	  type = 1;
	  
	  /* Update the D values */
	  temp = *d1;
	  *d1 = (1 + gamma)*(*d2);
	  *d2 = (1 + gamma)*temp;
	}
	else {
	  type = 2;

	  /* Compute alpha = 1/alpha */
	  temp = alpha.r*alpha.r + alpha.i*alpha.i;
	  alpha.r = alpha.r/temp;
	  alpha.i = -alpha.i/temp;
	  
	  /* Compute beta = 1/beta */
	  temp = beta.r*beta.r + beta.i*beta.i;
	  beta.r = beta.r/temp;
	  beta.i = -beta.i/temp;
	  
	  /* Compute 1/gamma */
	  gamma = 1/gamma;

	  /* Update the D values */
	  *d1 = (1+gamma)*(*d1);
	  *d2 = (1+gamma)*(*d2);
	}	
      } /* if(*a2 == 0) */

      /* ================================================================== */
      /* Perform update of matrices A and M (pre and post multiplications). */
      /* In matlab notation:                                                */
      /*                                                                    */
      /*                     |beta   1  |                                   */
      /*    A([i-1 i],j:n) = | 1   alpha| * A([i-1 i],j:n),                 */
      /*                                                                    */
      /* and...                                                             */
      /*                     |beta   1  |'                                  */
      /*    M(:,[i-1 i]) = [ | 1   alpha| * M(:,[i-1 i])' ]',               */
      /*                                                                    */
      /* for a type 1 transform.                                            */
      /* For a type 2 transform use [1 beta; alpha 1] for an A update,      */
      /* and [1 alpha; beta 1]' for the update to M.                        */
      /* ================================================================== */ 
      
      /* Setup the data pointers.  a3 is set to a1 or A[i-1][j].  a4 is set */
      /* to a2 or A[i][j].  m1 is set to M[0][j], and m2 to M[0][i].        */
      a3 = a1;
      a4 = a2;
      m1 = M + (i-1)*rows;
      m2 = M + i*rows;

      /* ------------- TYPE 1 TRANSFORMATION ---------------- */
      if(type == 1) {
	/* Pre-multiplication (A update) */
	for(k = j; k < cols; k++) {
	  /* Temporarily store a3. */
	  tau = (*a3);
	  
	  /* Update the rows of A. */
	  /* a3 = beta * a3 + a4 */
	  temp = a3->r;
	  a3->r = a3->r*beta.r - a3->i*beta.i + a4->r;
	  a3->i = temp*beta.i + a3->i*beta.r + a4->i;

	  /* a4 = tau + alpha*a4 */
	  temp = a4->r;
	  a4->r = tau.r + a4->r*alpha.r - a4->i*alpha.i;
	  a4->i = tau.i + temp*alpha.i + a4->i*alpha.r;

	  /* Increment pointers. */
	  a3++;
	  a4++;
	}
	/* Post-multiplication (M update) */
	for(k = 0; k < rows; k++) {
	  
	  /* Temporarily store m1 */
	  tau = (*m1);
	  
	  /* Update the columns of M. */
	  /* m1 = beta'*m1 + m2 */
	  temp = m1->r;
	  m1->r = beta.r*m1->r + beta.i*m1->i + m2->r;
	  m1->i = beta.r*m1->i - beta.i*temp + m2->i;

	  /* m2 = tau + alpha'*m2 */
	  temp = m2->r;
	  m2->r = tau.r + alpha.r*m2->r + alpha.i*m2->i;
	  m2->i = tau.i + alpha.r*m2->i - alpha.i*temp;
	  
	  /* Increment pointers. */
	  m1++;
	  m2++;
	}
      }
      /* ---------- TYPE 2 TRANSFORMATION ---------- */
      else {
		
	/* Pre-multiplication (A update) */
	for(k = j; k < cols; k++) {
	  /* Temporarily store a3. */
	  tau = (*a3);
	  
	  /* Update the rows of A. */
	  /* a3 = a3 + beta*a4 */
	  a3->r = a3->r + beta.r*a4->r - beta.i*a4->i;
	  a3->i = a3->i + beta.r*a4->i + beta.i*a4->r;

	  /* a4 = alpha*tau + a4 */
	  a4->r = alpha.r*tau.r - alpha.i*tau.i + a4->r;
	  a4->i = alpha.r*tau.i + alpha.i*tau.r + a4->i;

	  /* Increment pointers. */
	  a3++;
	  a4++;
	}
	/* Post-multiplication (M update) */
	for(k = 0; k < rows; k++) {
	  
	  /* Temporarily store m1 */
	  tau = (*m1);
	  
	  /* Update the columns of M. */
	  /* m1 = m1 + beta'*m2 */
	  m1->r = m1->r + beta.r*m2->r + beta.i*m2->i;
	  m1->i = m1->i + beta.r*m2->i - beta.i*m2->r;
	  
	  /* m2 = alpha'*tau + m2 */
	  m2->r = alpha.r*tau.r + alpha.i*tau.i + m2->r;
	  m2->i = alpha.r*tau.i - alpha.i*tau.r + m2->i;
	  
	  /* Increment pointers. */
	  m1++;
	  m2++;
	}
      }  /* if(type == 1) */
      
      /* Set the data pointers for the next iteration.  The A values shift  */
      /* up a row.  Therefore a2 points to where a1 was in the previous     */
      /* iteration, and a1 shifts up one row.  The D values each shift one  */
      /* element to the left in the vector (or up the diagonal if you think */
      /* of D as a diagonal matrix).                                        */
      a2 = a1;
      a1 -= cols;
      d2 = d1;
      d1--;

    } /* for(j = 0; j < cols; j++) */

    /* Compute 1/square root of D[j][j].  Subsequent iterations will  */
    /* not access this element.  We'll need the 1/square root of each */
    /* diagonal element to compute the final Q and R values.          */
    *d2 = (float)(1/sqrt((double)*d2));

  } /* for(i = rows-1; i > j; i--) */

  /* Compute 1/square root of the remaining rows-cols D values. */
  d1 = D+cols;
  for(i = 0; i < rows-cols; i++) {
    *d1 = (float)(1/sqrt((double)*d1));
    d1++;
  }

  /* Compute Q = M * D^(-1/2) */
  /* This will assign into the new matrix Q.  Recall that the matrix */
  /* M is stored in column major order, while the matrix Q is stored */
  /* in row major order.                                             */
  q1 = Q;
  for(i = 0; i < rows; i++) {
    d1 = D;
    m1 = M+i;
    for(j = 0; j < rows; j++) {
      q1->r = m1->r * (*d1);
      q1->i = m1->i * (*d1);
      m1 += rows;
      q1++;
      d1++;
    }
  }
  /* Compute R = D^(-1/2) * T (where T is the upper triangularized     */
  /* version of A).  This will still assign in-place into the original */
  /* input matrix A.                                                   */
  a1 = A;
  d1 = D;
  for(i = 0; i < rows; i++) {
    for(j = 0; j < cols; j++) {
      a1->r = a1->r * (*d1);
      a1->i = a1->i * (*d1);
      a1++;
    }
    d1++;
  }

  return;
}

/*
** Function: main
**
** Setup data for the QR computation.  Time the QR and output verification
** data.
*/
int main(int argc, char **argv)
{
  /* Input matrix A.  This matrix will be overwritten by one of the */
  /* resulting output matrices, R, throughout the computation.      */
  PcaCArrayFloat inmatrix;

  /* Resulting output matrix Q. */
  PcaCArrayFloat outmatrix_q;
  
  /* Variable used to store the latency of the kernel in seconds. */
  PcaCArrayFloat rtime;
  
  /* Diagonal scaling matrix D.  Because it is a diagonal matrix, */
  /* it will be stored as a one dimensional vector.               */
  PcaCArrayFloat D;

  /* Temporary matrix used for Q computation.  This matrix will be */
  /* stored column-wise.                                           */     
  PcaCArrayFloat M;
  
  /* Temporary timing variable used to store the initial start time. */
  pca_timer_t    timer;

  /* String variables used to store the input and output file names. */
  char           *inmatrixfile, *outmatrixfile_q; 
  char           *outmatrixfile_r, *timefile;
  
  /* Number of rows and columns in the input matrix A. */
  int rows, cols;

  /* Build the input and output file names.  If no arguments are specified, */
  /* show the user the proper usage of the program.                         */
  if (argc == 2) {
    /* Allocate memory for the file names. */
    inmatrixfile =    (char*) malloc(strlen("./data/") + strlen(argv[1]) +
				     strlen("-qr-inmatrix.dat") + 1); 
    outmatrixfile_q = (char*) malloc(strlen("./data/") + strlen(argv[1]) +
				     strlen("-qr-outmatrix_q.dat") + 1); 
    outmatrixfile_r = (char*) malloc(strlen("./data/") + strlen(argv[1]) +
				     strlen("-qr-outmatrix_r.dat") + 1); 
    timefile =        (char*) malloc(strlen("./data/") + strlen(argv[1]) +
				     strlen("-qr-timing.dat") + 1); 

    /* Create the file names. */
    sprintf(inmatrixfile, "./data/%s-qr-inmatrix.dat", argv[1]);
    sprintf(outmatrixfile_q, "./data/%s-qr-outmatrix_q.dat", argv[1]);
    sprintf(outmatrixfile_r, "./data/%s-qr-outmatrix_r.dat", argv[1]);
    sprintf(timefile, "./data/%s-qr-timing.dat", argv[1]);
  }
  else {
    printf("No data set specified.\n");
    printf("Usage: qr <DataSetNum>\n");
    exit(-1);
  }

  /* Read the input matrix from file, and get the number of rows and cols. */
  readFromFile(float, inmatrixfile, inmatrix);
  rows = inmatrix.size[0];
  cols = inmatrix.size[1];

  /* Check to see that rows >= cols. */
  if(rows < cols) {
#ifdef VERBOSE
    printf("Error: The number of rows must be greater than or equal to the number of columns in the input matrix.\n");
#endif
    exit(-1);
  }

#ifdef VERBOSE
  printf("Performing the QR on a matrix of size %d x %d.\n", 
	 inmatrix.size[0], inmatrix.size[1]);
  printf("Output will be stored in files %s, and %s.\n", 
	 outmatrixfile_q, outmatrixfile_r);
  printf("Timing will be stored in file %s.\n", timefile);
#endif

  /* Allocate memory for output matrix, run time, and internal data. */
  pca_create_carray_2d(float, outmatrix_q, rows, 
		       rows, PCA_COMPLEX);
  pca_create_carray_2d(float, M, rows,
		       rows, PCA_COMPLEX);
  pca_create_carray_1d(float, rtime, 1, PCA_REAL);
  pca_create_carray_1d(float, D, rows, PCA_REAL);

  /* Initialize the matrix D and M (which will eventually form to Q) to I */
  initialize_matrices(&M, &D);

  /* Run and time the Complex Fast Givens QR. */
  timer = startTimer();
  
  qr(rows, cols, 
     (struct ComplexFloat *)&inmatrix.data[0], 
     (struct ComplexFloat *)&M.data[0], 
     &D.data[0], (struct ComplexFloat *)&outmatrix_q.data[0]);

  rtime.data[0] = stopTimer(timer); 

  /* Output the calculated time in seconds. */
  printf("Done.  Latency: %f s.\n", rtime.data[0]);

  /* Write the run time and output matrices to file. */
  writeToFile(float, timefile, rtime);
  writeToFile(float, outmatrixfile_q, outmatrix_q);
  writeToFile(float, outmatrixfile_r, inmatrix);

  /* Free the allocated memory. */
  free(inmatrixfile);
  free(outmatrixfile_q);
  free(outmatrixfile_r);
  free(timefile);
  clean_mem(float, D);
  clean_mem(float, rtime);
  clean_mem(float, outmatrix_q);
  clean_mem(float, M);
  clean_mem(float, inmatrix);

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
