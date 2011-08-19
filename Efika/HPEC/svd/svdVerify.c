/******************************************************************************
** File: svdVerify.c
**
** HPEC Challenge Benchmark Suite
** SVD Kernel Benchmark
**
** Contents:
**  The verifier of the ANSI C SVD C-Kernel
**
** Input/Output:
**  The input matrix is stored in file "<dataSetNum>-svd-inmatrix.dat".
**  The output matricies are stored in files "<dataSetNum>-svd-outmatrix_u.dat".
**                                           "<dataSetNum>-svd-outmatrix_v.dat".
**                                           "<dataSetNum>-svd-outvector_s.dat".
**
** Command:
**   svdVerify <data set num> 
**
** Author: Jeanette Baran-Gale
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "PcaCArray.h"

#define sqr(a) ((a)*(a))

float compute_allowable_error(int rows, int cols )
{
  float acceptable_diff = 0;

  acceptable_diff = FLT_EPSILON * 10;

  if(cols > rows) {
    acceptable_diff *= (float)(cols);
  }
  else {
    acceptable_diff *= (float)(rows);
  }

#ifdef VERBOSE
  printf("Tolerance: %g\n", acceptable_diff);
#endif

  return acceptable_diff;
}


int verify(int rows, int cols, float allowable_error, ComplexFloat *A,
	   /* Output matrices from the SVD kernel. */
	   ComplexFloat *U_out, ComplexFloat *V_out, ComplexFloat *S_out,
	   /* Truth singular Values */
	   float *S)
{
  int i, j, k;
  ComplexFloat *ptr1, *ptr2, * ptr3;
  float maxA, maxU, maxV, maxS, inv_s1, my_abs;
  float *sptr;

  ComplexFloat *tmp, *tmp2;

  tmp = pca_malloc(ComplexFloat,rows*cols);
  tmp2 = pca_malloc(ComplexFloat,rows*cols);

  /* RelErrSvd = max(max(abs(a - u_out*s_out*V_out')))/s_out(1) */

  /* tmp = u_out * s_out */
  ptr1 = tmp;
  ptr2 = U_out;
  for( i = 0; i< rows; i++)
  {
     for( j = 0; j< cols; j++)
     {
	ptr1->r = ptr2->r * S_out[j].r;
	ptr1->i = ptr2->i * S_out[j].r;
	ptr1++; ptr2++;
     }
  }

  /* u = tmp * V_out' */
  ptr1 = tmp2;
  ptr2 = tmp;
  for( i = 0; i< rows; i++)
  {
     ptr3 = V_out;
     for( j = 0; j< cols; j++)
     {
	ptr1->r = 0.0f;
	ptr1->i = 0.0f;
	ptr2 = tmp + (i * cols);
	for( k = 0; k< cols; k++)
	{
	   /* (ptr2 * conj(ptr3)) */
	   ptr1->r +=  (ptr2->r * ptr3->r) + (ptr2->i * ptr3->i);
	   ptr1->i += -(ptr2->r * ptr3->i) + (ptr2->i * ptr3->r);

	   ptr3++; ptr2++;
	}
	ptr1++;
     }
  }

  /* Verify the correctness of the results. */
  inv_s1 = 1.0f/S_out->r;
  ptr1 = A;
  ptr2 = tmp2; /* now contains the sum */
  maxA = 0.0;
  for(i = 0; i < cols; i++) {
    for(j = 0; j < cols; j++) {
       tmp->r = ptr1->r - ptr2->r;
       tmp->i = ptr1->i - ptr2->i;
       my_abs = sqrt(tmp->r*tmp->r + tmp->i*tmp->i) * inv_s1;
       if (my_abs > maxA)
       {  maxA = my_abs; }

       ptr1++;
       ptr2++;
    }
  }

  /* verify the orthogonality of U */
  /* Tmp  = U'*U */
  /* max( I - U'*U) is the error in orthogonality*/
  ptr1 = tmp2;
  ptr2 = U_out;
  ptr3 = U_out;
  maxU = 0.0;
  for( i = 0; i< cols; i++)
  {
     for( j = 0; j< cols; j++)
     {
	ptr1->r = 0.0f;
	ptr1->i = 0.0f;
	ptr2 = U_out + (i);
	ptr3 = U_out + (j);
	for( k = 0; k< rows; k++)
	{
	   /* (conj(ptr2) * ptr3) */
	   ptr1->r +=  (ptr2->r * ptr3->r) + (ptr2->i * ptr3->i);
	   ptr1->i +=  (ptr2->r * ptr3->i) - (ptr2->i * ptr3->r);

	   ptr3+=cols; ptr2+=cols;
	}
	if( i==j)
	{
	   my_abs = sqrt(sqr(1.0f-ptr1->r)+sqr(ptr1->i));
	}
	else
	{
	   my_abs = sqrt(sqr(ptr1->r)+sqr(ptr1->i));
	}
	if ( my_abs > maxU)
	{
	   maxU = my_abs;
	}
	ptr1++;
     }
  }


 
  /* verify the orthogonality of V */
  /* Tmp  = V'*V */
  /* max( I - U'*U) is the error in orthogonality*/
  ptr1 = tmp;
  ptr2 = V_out;
  ptr3 = V_out;
  maxV = 0.0;
  for( i = 0; i< cols; i++)
  {
     for( j = 0; j< cols; j++)
     {
	ptr1->r = 0.0f;
	ptr1->i = 0.0f;
	ptr2 = V_out + (i );
	ptr3 = V_out + (j );
	for( k = 0; k< cols; k++)
	{
	   /* (conj(ptr2) * ptr3) */
	   ptr1->r +=  (ptr2->r * ptr3->r) + (ptr2->i * ptr3->i);
	   ptr1->i +=  (ptr2->r * ptr3->i) - (ptr2->i * ptr3->r);

	   ptr3+=cols; ptr2+=cols;
	}
	if( i==j)
	{
	   my_abs = sqrt(sqr(1.0f-ptr1->r)+sqr(ptr1->i));
	}
	else
	{
	   my_abs = sqrt(sqr(ptr1->r)+sqr(ptr1->i));
	}
	if ( my_abs > maxV)
	{
	   maxV = my_abs;
	}
	ptr1++;
     }
  }

  /* find the max Relative Error in S */

  ptr1 = S_out;
  sptr = S;
  maxS = 0.0f;
  for( i = 0; i< cols; i++)
  {
     my_abs = (fabs(ptr1->r - *sptr) / (*sptr));
     if(my_abs > maxS)
     {
	maxS = my_abs;
     }
     ptr1++; sptr++;
  }
  
  /* clean mem */
  free(tmp);
  free(tmp2);
  
  /* print results */
#ifdef VERBOSE
  printf("Max Relative Error in A = %g \n",maxA);
  printf("Max Error in Orthogonality of U = %g \n",maxU);
  printf("Max Error in Orthogonality of V = %g \n",maxV);
  printf("Max Relative Error in Singular Values = %g \n",maxS);
#endif

  if((maxA > allowable_error)||
     (maxU > allowable_error)||
     (maxV > allowable_error)||
     (maxS > allowable_error))
  {
     /* The test failed...  Return 0. */
     return 0;
  }
  else
  {
     /* The test verified...  Return 1. */
     return 1;
  }
}

int main(int argc, char **argv)
{
  PcaCArrayFloat in_matrix, out_matrix_u, out_matrix_v, out_vector_s;
  PcaCArrayFloat verify_s;
  char           in_matrix_file[100];
  char           out_matrix_u_file[100], out_matrix_v_file[100], 
		 out_vector_s_file[100], verify_s_file[100];
  
  float          allowable_error;

  if (argc != 2) {
    printf("Usage: %s <data set num>\n", argv[0]);
    return -1;
  }

  /* Build the input file names.                  */
  sprintf(in_matrix_file, "./data/%s-svd-inmatrix.dat", argv[1]);
  sprintf(out_matrix_u_file, "./data/%s-svd-outmatrix_u.dat", argv[1]);
  sprintf(out_matrix_v_file, "./data/%s-svd-outmatrix_v.dat", argv[1]);
  sprintf(out_vector_s_file, "./data/%s-svd-outvector_s.dat", argv[1]);
  sprintf(verify_s_file, "./data/%s-svd-verification_s.dat", argv[1]);

  /* Read in the input matrix from file.  We'll use this to compute */
  /* an allowable error.                                            */
  readFromFile(float, in_matrix_file, in_matrix);

  /* Read in the output matrices from the SVD kernel. */
  readFromFile(float, out_matrix_u_file, out_matrix_u);
  readFromFile(float, out_matrix_v_file, out_matrix_v);
  readFromFile(float, out_vector_s_file, out_vector_s);
  readFromFile(float, verify_s_file, verify_s);


  /* Compute the allowable error. */
  allowable_error = 
    compute_allowable_error(in_matrix.size[0], in_matrix.size[1]);

  /* Run the verifier */
  if (verify(in_matrix.size[0], in_matrix.size[1], allowable_error,
	     (ComplexFloat *)&in_matrix.data[0], 
	     (ComplexFloat *)&out_matrix_u.data[0], 
	     (ComplexFloat *)&out_matrix_v.data[0], 
	     (ComplexFloat *)&out_vector_s.data[0],
	     (       float *)&verify_s.data[0])) {
    printf("Verification: PASS\n");
  }
  else {
    printf("Verification: FAIL\n");
  }
  clean_mem(float, in_matrix);
  clean_mem(float, out_matrix_u);
  clean_mem(float, out_matrix_v);
  clean_mem(float, out_vector_s);
  clean_mem(float, verify_s);

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
