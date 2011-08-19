/******************************************************************************
** File: svd.c
**
** HPEC Challenge Benchmark Suite
** SVD Kernel Benchmark
**
** Contents:
**  The ANSI C Complex SVD kernel.  
**
** Input/Output:
**  The input matrix is stored in file "<dataSetNum>-svd-inmatrix.dat".
**  The output matrices U and V will be stored in the files 
**  "<dataSetNum>-svd-outmatrix_u.dat" and "<dataSetNum>-svd-outmatrix_v.dat"
**  respectively.  The output vector sigma will be stored in the file 
**  "<dataSetNum>-svd-outvector_s.dat".  The total run time will be stored in 
**  file "<dataSetNum>-svd-timing.dat".
**
** Command:
**   svd <data set num>
**
** Author: Jeanette Baran-Gale
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <stdio.h>
#include <math.h>
#include <float.h>

#include "PcaCArray.h"
#include "PcaCTimer.h"

/* ************************************************************************** */
#define true 1
#define false 0
#define STR_SZ 80
typedef int bool;

/* The following struct contains pointers to matrices/vectors required    */
/* inside the SVD calculation.  The function setupMem is used to allocate */
/* data for the structure and cleanMem is used to free the memory.        */
typedef struct
{
   ComplexFloat *m_tmpW;
   ComplexFloat *m_tmpP;
   ComplexFloat *m_tmpBW;
   ComplexFloat *m_tmpBV;
   ComplexFloat *m_tmpV;
   float        *m_urs;
   float        *m_vrs;
   float        *m_diag;
   float        *m_sdiag;
   float        *m_ur;
   float        *m_vr;
   ComplexFloat *m_uPreQr;
   ComplexFloat *m_q;
   ComplexFloat *m_r;
   ComplexFloat *m_ut;
   ComplexFloat *m_vt;
}tMemory;



/* ************************************************************************** *
 * Function: qr
 *
 * Compute the Complex Modified Gram-Schmidt QR factorization of the input 
 * matrix A.  For more information on the MGS QR algorithm, see Golub and 
 * Van Loan's Matrix Computations, pages 231, 232 (for 3rd edition).
 *
 * From the input matrix A, calculate Q and R such that A = Q*R.  The
 * Modified Gram-Schmidt QR is a reduced factorization.  Therefore, for
 * an MxN input matrix A, Q will be of size MxN, while R will be of size
 * NxN.
 *
 * This implementation requires that the matrix R be initialized to zero.
 * Author: Ryan Haney (haney@ll.mit.edu)
 * ************************************************************************** */
void qr(int rows, int cols, ComplexFloat *A, 
	ComplexFloat *Q, ComplexFloat * R)
{
  /* A data access pointers */
  ComplexFloat  *aCol;

  /* Q data access pointers */
  ComplexFloat  *qCol;

  /* R data access pointers */
  ComplexFloat  *rRow = R;
  ComplexFloat  *rDiag = R;

  /* Stride values for advancing along a column of Q and A. */
  int           aRowStride = cols;
  int           qRowStride = cols;

  /* Stride value for advancing along the diagonal of R. */
  int           rDiagStride = cols + 1;

  /* Temporaries and loop variables. */
  double         theNorm;
  int           i, j, k;

  /* Main loop:  Loop over the columns of the input matrix, A. */
  for (i = 0; i < cols; i++) {

    /* Calculate R(i,i) = norm(A(:,i))                             */
    /* The result is a real number, i.e. the imaginary part is 0.  */
    aCol = A + i;
    theNorm = 0.0;
    for(k = 0; k < rows; k++) {
      theNorm += ((double)aCol->r*(double)aCol->r) + 
	         ((double)aCol->i*(double)aCol->i);
      aCol += aRowStride;
    }
    /* We've computed the inner product, A(:,i)'*A(:,i).  Now take    */
    /* the square root of this value to calculate the norm of A(:,i). */
    theNorm = sqrt(theNorm);
    rDiag->r = (float)theNorm;
    
    /* Point to the top of A and Q's column */
    aCol = A + i;
    qCol = Q + i;
    
    /* Calculate the update of column i of Q.                  */
    /* matlab: Q(:,i) = A(:,i) / R (i,i)                       */
    /* Loop over rows (or down the current column of Q and A). */
    for (j = 0; j < rows; j++) {
      /* Set the Q(j,i) value to A(j,i)/R(i,i). */
      qCol->r = aCol->r / theNorm;
      qCol->i = aCol->i / theNorm;
      
      /* Advance the Q and A pointers to the next value in their */
      /* respective columns.                                     */
      qCol += qRowStride;
      aCol += aRowStride;
    }
    
    /* The following is performed only if this is NOT the last column. */
    if (i < (cols-1)) { 

      /* Calculate R(i,i+1:n) = Q(:,i)' * A(:,i+1:n)          */
      /* Set the R value to one element next to the diagonal. */
      rRow = rDiag + 1;
      /* Loop on columns of A, from column i+1 to the last column. */
      for (j = i+1; j < cols; j++, rRow++) {
	/* Set A's pointer to the top of the jth column. */
	aCol = A + j;
	/* Reset Q's pointer to the top of the ith column. */
	qCol = Q + i;
	/* Initialize the current element of R for accumulation. */
	rRow->r = 0.0;
	rRow->i = 0.0;
	/* Calculate R value equal to Q(:,i)' * A(:,j). */
	for(k = 0; k < rows; k++) {
	  rRow->r += (aCol->r * qCol->r) + (aCol->i * qCol->i);
	  rRow->i += (aCol->i * qCol->r) - (aCol->r * qCol->i);

	  /* Advance the Q and A pointers to the next value in their */
	  /* respective columns.                                     */
	  aCol += aRowStride;
	  qCol += qRowStride;	  
	}
      }
      
      /* Calculate A(:,i+1:n) = A(:,i+1:n) - Q(:,i) * R(i,i+1:n) */
      qCol = Q + i;
      for (j = 0; j < rows; j++) {
	aCol = A + (j*aRowStride) + i + 1;
	rRow = rDiag + 1;
	for (k = i+1; k < cols; k++) {
	  aCol->r -= (qCol->r * rRow->r) - (qCol->i * rRow->i);
	  aCol->i -= (qCol->r * rRow->i) + (qCol->i * rRow->r);
	  aCol++;
	  rRow++;
	} 
	
	qCol += qRowStride;
      } 
    } /* endIf Not last column */

    /* Move the rDiag pointer onto the next diagonal element of R. */
    rDiag += rDiagStride;

  } /* if (i < (cols-1)) */
  
  return;
} /* End Function qr */

/* ************************************************************************** *
 * Function:     houseCplx()
 *
 *  Description: Compute Complex Householder vector, v, associated with a
 *               vector of complex values x, such that if
 *               y = ((I - v*v')/beta)*x, <Matlab notation> 
 *               y[0] is real and y[1] ... y[n-1] are all zero.
 *               Computation is based in part on Golub and Van Loan
 *               algorithm 5.1.1, but does not follow the definition of
 *               complex Householder given in Section 5.2.10. Instead, it
 *               computes a Householder vector with a real first element as
 *               is done by Lapack. 
 *
 *  Inputs:
 *       1 : input+output : 
 *           On input, pointer to array of Complex values x.
 *           On output, the Householder vector v described above.
 *       2 : input : integer size of the array
 *       3 : output : reference to complex beta value (see above)
 *  Outputs:
 *       return type: void
 *       v and beta (arguments 1 and 3) are changed
 * ************************************************************************** */
void houseCplx(ComplexFloat *v, int size, ComplexFloat *beta)
{
   float               sigma;
   float               mu;
   struct ComplexFloat ct;
   struct ComplexFloat ck;
   float               dDenominator;

   ComplexFloat *cv, *endp;
   cv = v+1;
   endp = cv+size-1;
   sigma =0.0;
   while(cv!=endp)
   {
      sigma += (cv->r * cv->r) + (cv->i * cv->i);
      ++cv;
   }


   mu = (v->r * v->r) + (v->i * v->i) + sigma;
   mu = sqrt(mu);

   if (sigma == 0.0)
   {
     /* -------------------------------------------------------------*/
     /* Special case: everything below the first element is zero     */
     /* If the first element has a zero imaginary part, beta is zero */
     /* Otherwise, beta must make the imaginary part of the first    */
     /* element equal to zero                                        */
     /* -------------------------------------------------------------*/
      if (fabs(v->i) < (FLT_EPSILON * fabs(v->r)))
      {
	 beta->r = beta->i = 0.0;/*  all is zero */
      }
      else
      {
	 v->r = v->r - mu;
	 /* imaginary part doesn't change */

	 ct.r = mu * v->r;
	 ct.i = mu * v->i;

	 dDenominator = (ct.r * ct.r) + (ct.i * ct.i);
	 beta->r = -1.0 * (ct.r / dDenominator);
	 beta->i = ct.i / dDenominator;
      }
   }
   else
   {
     /* -------------------------------------------------------------*/
     /* General case (arbitrary values below the first element)      */
     /* -------------------------------------------------------------*/

      if (v->r <= 0.0f)
      {
	 v->r = v->r - mu;
	 /*   imaginary part doesn't change */

	 ct.r = mu * v->r;
	 ct.i = mu * v->i;

	 dDenominator = (ct.r * ct.r) + (ct.i * ct.i);
	 beta->r = -1.0 * (ct.r / dDenominator);
	 beta->i = ct.i / dDenominator;
      }
      else
      {
	 float dnomr; 
	 float dnomi;
	 ct.r = v->r + mu;
	 ct.i = - v->i;

	 ck.r = sigma;
	 ck.i = mu * v->i * (-2.0);

	 dDenominator = (ct.r * ct.r) + (ct.i * ct.i);
	 v->r = -1.0 * ((ck.r * ct.r) + (ck.i * ct.i)) / dDenominator;
	 v->i = ((ck.r * ct.i) - (ck.i * ct.r)) / dDenominator;

	 /*  beta = ct / (mu * ck); */
	 dnomr = mu*ck.r;
	 dnomi = mu*ck.i;
	 dDenominator = (dnomr * dnomr) + (dnomi * dnomi);
	 beta->r = ((ct.r * dnomr) + (ct.i * dnomi)) / dDenominator;
	 beta->i = ((ct.i * dnomr) - (ct.r * dnomi)) / dDenominator;
      }
   }/* endIf sigma is nearly zero */

   return;
}/* end houseCplx() */

/* ************************************************************************** *
 *  Function:        applyHouseCols()
 *
 *  Description: Apply a Householder transform to a matrix from the right.
 *                The matrix being transformed is itself altered.
 *
 *  Inputs: 
 *         1: Matrix A, a subMatrix of which is transformed
 *         2: ptr to array of ComplexFloat, this is the Householder vector
 *         3: integer length of the Householder vector
 *         4: integer row of matrix we are applying Householder transform to
 *         5: stride of A
 *         6: rows of A
 *         7: cols of A
 *         8: ComplexFloat beta value associated with Householder transform
 *
 *  Outputs: the return value is void
 *           the input matrix is transformed.
 * ************************************************************************** */
void applyHouseCols(ComplexFloat *a, ComplexFloat *v, int vLen, int theRow,
		    int	aStride, int m_m, int m_n, ComplexFloat beta, 
		    tMemory *MM)
{
   int         wsize = m_m - theRow;
   ComplexFloat *w;
   ComplexFloat *wEnd;
   ComplexFloat *av = a + (theRow * aStride) + m_n - vLen;
   ComplexFloat *ap = av;
   ComplexFloat *tp;
   ComplexFloat  tmp;
   int         r, c;
   ComplexFloat *v1,*v2, *vend, *result; 

   w = MM->m_tmpW;
   wEnd = MM->m_tmpW +wsize;


   /*   --------------------------------------------------------------- */
   /*   matlab:                                                         */
   /*       W = beta * (A * V') -- inner product of A, V'               */
   /*        we apply beta here for optimization purposes               */
   /*   --------------------------------------------------------------- */
   while (w != wEnd) {
      tmp.r = 0.0f;
      tmp.i = 0.0f;
      v1=ap; v2 = v; vend = ap +vLen;
      while (v1 != vend) {
	 tmp.r += (v1->r * v2->r) + (v1->i * v2->i);
	 tmp.i += (v1->i * v2->r) - (v1->r * v2->i);
	 ++v1;
	 ++v2;
      }


      w->r = (beta.r * tmp.r) - (beta.i * tmp.i);
      w->i = (beta.r * tmp.i) + (beta.i * tmp.r);

      w++;
      ap += aStride;
   }/* endWhile filling W */


   /*   ---------------------------------- */
   /*   matlab:                            */
   /*       A = A - W * V                  */
   /*   First, calculate outer product W*V */
   /*   ---------------------------------- */

   result = MM->m_tmpP;
   v1 = MM->m_tmpW; 
   for (r=0; r<wsize; r++) {
      v2 = v;
      for (c=0; c<vLen; c++) {
	 result->r = (v1->r * v2->r) - (v1->i * v2->i);
	 result->i = (v1->r * v2->i) + (v1->i * v2->r);

	 ++result;
	 ++v2;
      }/* endFor each col */

      v1++;
   }/* endFor each row */

   /*   ----------------------------------------------------------- */
   /*   Subtract out matrix built by outer product loop to Update A */
   /*   ----------------------------------------------------------- */
   tp = MM->m_tmpP;

   for (r=0; r<wsize; r++) {
      ap = av + (aStride * r);
      for (c=0; c<vLen; c++) {
	 ap->r -= tp->r;
	 ap->i -= tp->i;
	 ap++; tp++;
      }/* endFor each col */
   }/* endFor each row */


   return;
}/* end applyHouseCols()   */


/* ************************************************************************** *
 * Function: partitionBiDiag()
 *  Description:
 * Partition a bi-diagonal matrix as described in Golub and Van Loan
 * Algorith 8.6.2. The lower-right diagonal matrix size (q) is as large as
 * possible, and the matrix b22 is guaranteed to be bi-diagonal with no
 * zeros on its diagonal or superdiagonal as required by the Golub-Kahan
 * SVD step. 
 * In Matlab notation, 
 *  a = [ b11  0   0 ;
 *        0  b22  0 ;
 *        0   0  b33 ],
 * If a is size n by n, then
 *  b11 is size [n-(p+q) x n-(p+q)] and diagonal
 *  b22 is size [p x p] and bi-diagonal with no zeros on the super-diagonal
 *  b33 is size [q x q] and diagonal with q as large as possible.
 *
 *  Inputs: 
 *          1: ptr to array containing the super-diagonal
 *          2: ref to int which will be set to B11 size output (n-(p+q))
 *          3: ref to int which will be set to B22 size output (p) 
 *          4: ref to int which will be set to B33 size output (q) 
 *          5: diagonal length, n (superdiagonal has length n-1)
 *
 * ************************************************************************** */
void partitionBiDiag(float *sd, int *b11Size,
		     int *b22Size, int *b33Size, int m_diagLen)
{
   int   sdLen = m_diagLen-1;
   int   q = 0;
   int   k = 0;
   float *sd_p = sd+sdLen-1;/*  init ptr to end of super-diag */

   /*    Starting at the end */
   /*    count back while super-diag has 0's     */
   /*   ---------------------------------------* */
   while (q < sdLen) {
      if (*sd_p-- == 0)
	 ++q;
      else
	 break;
   }/* endWhile */

   *b33Size = q;

   if (q == sdLen)
   {
     /* ------------------------------------------------------------- */
     /* The bidiagonal component cannot be just one element: if there */
     /* n-1 diagonal elements the diagonal size is really n           */
     /* ------------------------------------------------------------- */
      *b33Size += 1;
      *b11Size  = 0;
      *b22Size  = 0;
   }
   else
   {
      /* ------------------------------------------------------ */
      /* Find extent of largest superdiagonal in remainder of b */ 
      /*       Continuing where we left off                     */
      /*        count back until we hit a 0                     */
      /* ------------------------------------------------------ */
      ++q;/*  so we don't re-check the last one we looked at    */
      k = 1;/*   we've already found the 1st */
      while (q++ < sdLen) {
	 if (*sd_p-- != 0)
	    ++k;
	 else
	    break;
      }/* endWhile */

      *b22Size = k + 1;

      *b11Size = (sdLen + 1) - (*b22Size + *b33Size);
   }/* endIf whole super-diag has 0's */

   return;
}/* end partitionBiDiag() */

/* ************************************************************************** *
 * Function: applyHouseRows
 *
 *  Description: Apply a Householder transform to a matrix from the left.
 *                The matrix being transformed is itself altered.
 *
 *  Inputs: 
 *         1: Matrix A, a subMatrix of which is transformed
 *         2: ptr to array of ComplexFloat, this is the Householder vector
 *         3: integer length of the Householder vector
 *         4: integer column of matrix we are applying Householder transform to
 *         5: rows of A
 *         6: cols of A
 *         7: ComplexFloat beta value associated with Householder transform
 *
 *  Outputs: the return value is void
 *           the input matrix is transformed.
 * ************************************************************************** */
void applyHouseRows(ComplexFloat *a, ComplexFloat *v, int vsize, int theCol, 
                    int m_m, int m_n, ComplexFloat beta, tMemory* MM)
{
   int         wsize = m_n - theCol;/*  wsize is (# Cols - ColNumber) */
   int         aStride = m_n;
   ComplexFloat *w;
   ComplexFloat *wEnd; 
   ComplexFloat *av = a + ((m_m-vsize)*aStride) + theCol;
   ComplexFloat *ap = av;
   ComplexFloat *tp;
   /* ComplexFloat  topLeft;*/
   int         r, c;
   ComplexFloat *matEl;
   ComplexFloat *v1El ;
   ComplexFloat *v2El;
   ComplexFloat *v1p ;
   ComplexFloat *v2p ;
   ComplexFloat *endp;

   w = MM->m_tmpW;
   wEnd = MM->m_tmpW +wsize;

   /*   --------------------------------------------------------------- */
   /*   matlab:                                                         */
   /*       W = A' * v -- inner product of A', v                        */
   /*   --------------------------------------------------------------- */

   while (w != wEnd) {
      w->r=0.0f;
      w->i=0.0f;
      v1p = v;
      v2p = ap;
      endp = v+vsize;
      while (v1p != endp) {
	 w->r += (v1p->r * v2p->r) + (v1p->i * v2p->i);
	 w->i += (v1p->i * v2p->r) - (v1p->r * v2p->i);
	 v1p++;
	 v2p += aStride;
      }

      w++;
      ap++;
   }/* endWhile filling W */


   /*   --------------------------------------------------- */
   /*    Apply Beta to V prior to outer product - saves ops */
   /*   --------------------------------------------------- */
   for (ap=MM->m_tmpBV, tp=v, c=0; c<vsize; c++) {
      ap->r = (beta.r * tp->r) - (beta.i * tp->i);
      ap->i = (beta.r * tp->i) + (beta.i * tp->r);
      ++ap;
      ++tp;
   }

   /*   ----------------------------------  */
   /*   matlab:                             */
   /*       A = A - beta* V * W'            */
   /*   First, calculate outer product V*W' */
   /*   ----------------------------------  */
   matEl = MM->m_tmpP;
   v1El  = MM->m_tmpBV;

   for (r=0; r<vsize; r++) {
      v2El = MM->m_tmpW;
      for (c=0; c<wsize; c++) {
	 matEl->r = (v1El->r * v2El->r) + (v1El->i * v2El->i);
	 matEl->i = (v1El->i * v2El->r) - (v1El->r * v2El->i);

	 ++matEl;
	 ++v2El;
      }/* endFor each col */

      v1El++;
   }/* endFor each row */

   /*   ----------------------------------------------------------- */
   /*   Subtract out matrix built by outer product loop to Update A */
   /*   ----------------------------------------------------------- */
   tp = MM->m_tmpP;

   for (r=0; r<vsize; r++) {
      ap = av + (aStride * r);
      for (c=0; c<wsize; c++) {
	 ap->r -= tp->r;
	 ap->i -= tp->i;
	 ap++;
	 tp++;
      }/* endFor each col */
   }/* endFor each row */

   return;
}/* end applyHouseRows() */

/* ************************************************************************** *
 * Function: givens()
 * 
 * Description: Calculate a givens rotation such that (in Matlab notation)
 *              [z 0 ] = [ c -s; s c] * [a b];
 *              See Golub and Van Loan section 5.1.8.
 *
 * Inputs:
 *       1: First element, to be set to sqrt(a*a+b*b) by rotation
 *       2: Second element, to be zeroed by rotation
 *       3: Ptr to cosine value for rotation, c
 *       4: Ptr to sine value for rotation, s
 * Outputs:
 *    Returns float, sqrt(a*b+b*b)
 *    On output, arguments referred to by 3-4 are set to c and s
 *
 * ************************************************************************** */
float givens(double a, double b, double *c, double *s)
{
   double theNormSq = (a * a) + (b * b);
   double theNorm = sqrt(theNormSq);

   if (b == 0.0f)
   {
      *c = 1;
      *s=0;
   }
   else
   {
      *c = a / theNorm;
      *s = -(b / theNorm);
   }/* endIf b is zero */


   return((float)theNorm);
}/* end givens() */


/* ************************************************************************** *
 * Function: gkSvdStep()
 *
 *  Description:
 *     One step on the path to diagonalize a bi-diagonal matrix: the
 *     Golub-Kahan SvdStep Algorithm from Golub and Van Loan Algorithm
 *     8.6.1. The matrix must have no zeros on its diagonal or
 *     superdiagonal. 
 *
 *  Inputs:
 *    1: Pointer to vector, diagonal of matrix to be reduced
 *    2: Pointer to vector, super-diagonal of matrix to be reduced
 *    3: Index of first element on diagonal to be considered in this step
 *    4: Length of diagonal to be considered in this step
 *    5: Pointer to left orthogonal matrix u
 *    6: Pointer to right orthogonal matrix v
 *    7: Stride of matrices u and v
 *    8: Overall diagonal length
 *
 *  Outputs:
 *    Return type void
 *    1, 2, 5, 6 (diagonal, superdiagonal, matrix u, matrix v) are updated
 *  
 *
 * ************************************************************************** */
void gkSvdStep(float *diag, float *super_diag,
      int dOffset, int biDiagSize, float *ur, 
      float *vr, int stride, int m_diagLen)
{
   double     c, s;
   double     tmp1, tmp2, extra;
   double     v1, v2;
   double     D0sq, F0sq;
   double     F1sq, cc, dd;
   double     lambda;
   double     sign;
   float         *d = diag + dOffset;
   float         *sd = super_diag + dOffset;
   float         *uData, *vData;
   float         *p1, *p2;
   int        uStride, vStride;
   int        j, k;
   int        n = m_diagLen;
   int        zChaseEnd = biDiagSize - 1;
   bool       biDiagSizeEq_2 = false;

   uData = ur + dOffset;
   uStride = stride;

   vData = vr + dOffset;
   vStride = stride;

   /* Compute the Wilkinson QR shift, lambda */
   D0sq  = d[biDiagSize-2];
   D0sq *= D0sq;

   if (biDiagSize > 2)
   {
      F0sq  = sd[biDiagSize-3];
      F0sq *= F0sq;
   }
   else
   {
      F0sq = 0;
      biDiagSizeEq_2 = true;
   }

   F1sq = *(sd + biDiagSize - 2);
   F1sq *= F1sq;

   cc = *(d + biDiagSize - 1);
   cc *= cc;
   cc += F1sq;
   dd = (D0sq+F0sq-cc) * 0.5;
   if (dd >=0.0)
      sign =  1.0;
   else 
      sign = -1.0;
   lambda = cc + dd - (sign * sqrt((dd*dd) + (D0sq*F1sq)));

   /*  Compute a givens rotation designed to zero an element */
   /*    in the shifted product matrix */
   /*  ---------------------------------------------------------* */
   v1 = (*d * *d) - lambda;
   v2 = *d * *sd;

   /*   matlab: [gMat, yMat] = planerot(v1, v2); */
   /*  ---------------------------------------------------------* */

   givens(v1, v2, &c, &s);

   /*   matlab: s(1:2, 1:2) = s(1:2, 1:2) * gMat' */
   /*  ---------------------------------------------------------* */
   tmp1 = (*d * c) - (*sd * s);
   *sd = (*d * s) + (*sd * c);
   extra   = - *(d+1) * s;
   *(d+1) *= c;
   *d      = tmp1;


   /*   adjust V */
   /*   matlab: v2(1:2, :) = gMat * v2(1:2, :) */
   /*     Note: this V2 is transposed, so we are really going */
   /*           down the 1st 2 columns of the V2 matrix prior to */
   /*           transpose. We do not actually transpose the V2 */
   /*           matrix, we leave it in place inside the V matrix. */
   /*  -----------------------------------------------------------* */
   p1 = vData;  /*   1st Row 1st Col of V2 un-transposed */
   p2 = vData+1;/*   1st Row 2nd Col of V2 un-transposed */

   for (k=0; k<n; k++) {
      tmp1= (c * *p1) - (s * *p2);
      *p2 = (s * *p1) + (c * *p2);
      *p1 = tmp1;
      p1 += vStride;
      p2 += vStride;
   }/* endFor k */


   /*  Zero the lower off-diagonal element created by the */
   /*     above rotation. */
   /*   matlab: [gMat, magVal] = planerot([s(1,1);s(2,1)]); */
   /*           s(1:2, 1:3) = gMat * s(1:2, 1:3) */
   /*           s(1,1) = magVal(1) */
   /*  ---------------------------------------------------------* */
   *d   = givens(*d, extra, &c, &s);
   tmp1 = (*sd * s) + (*(d+1) * c);
   *sd  = (*sd * c) - (s * *(d+1));

   *(d+1)   = tmp1;


   if (!biDiagSizeEq_2)
   {
      extra    = -(*(sd+1) * s);
      *(sd+1) *= c;
   }/* endIf there is No sd[1] */



   /*   adjust U */
   /*   matlab: u(:, 1:2) = u(:, 1:2) * gMat' */
   /*  ---------------------------------------------------------* */
   p1 = uData;  /*   1st Column of U */
   p2 = p1 + 1; /*   2nd Column of U */
   for (k=0; k<n; k++) {
      tmp1 = (*p1 * c) - (*p2 * s);
      *p2   = (*p1 * s) + (*p2 * c);
      *p1   = tmp1;
      p1  += uStride;
      p2   = p1 + 1;
   }


   /*                     Zero Chasing */
   /*  -----------------------------------------------------------* */
   for (j=1; j<zChaseEnd; j++) {

      /*     First: rotate columns to eliminate the super-diag elt */
      /*     matlab: [gMat, magVal] = planerot(s(j-1,j), s(j-1, j+1)) */
      /*                 at this time extra => s(j-1, j+1) */
      /*                 after extra represents the sub-diag elt */
      /*    ---------------------------------------------------------* */
      tmp1 = givens(sd[j-1], extra, &c, &s);
      tmp2    = (d[j] * c) - (sd[j] * s);
      sd[j]   = (d[j] * s) + (sd[j] * c);
      d[j]    = tmp2;
      sd[j-1] = tmp1;
      extra   = - s * d[j+1];
      d[j+1] *= c;


      /*  update V: */
      /*    matlab: v(jj:(jj+1),:) = gMat * v(jj:(jj+1),:) */
      /*     Note: this V2 is transposed, so we are really going */
      /*           down the J and J+1 columns of the V2 matrix prior */
      /*           to transpose. We do not actually transpose the V2 */
      /*           matrix, we leave it in place inside the V matrix. */
      /* ------------------------------------------------------------* */
      p1 = vData+j; /*   1st Row jth Col of V2 un-transposed */
      p2 = p1+1;    /*   1st Row j+1th Col of V2 un-transposed */

      for (k=0; k<n; k++) {
	 tmp1= (c * *p1) - (s * *p2);
	 *p2 = (s * *p1) + (c * *p2);
	 *p1 = tmp1;
	 p1 += vStride;
	 p2 += vStride;
      }/* endFor k */


      /*   Second: rotate rows to eliminate the new sub-diag elt */
      /*   matlab: [gMat, magVal] = planerot(s(j,j:j+2), s(j+1,j:j+2)) */
      /*         extra now represents sub-diag elt */
      /*         after extra represents new super-diag elt */
      /* -------------------------------------------------------------* */
      d[j] = givens(d[j], extra, &c, &s);

      tmp1 = (c * sd[j]) - (s * d[j+1]);

      d[j+1] = (s * sd[j]) + (c * d[j+1]);
      sd[j] = tmp1;

      if (j < (zChaseEnd-1))
      {
	 extra    = -s * sd[j+1];
	 sd[j+1] *= c;
      }


      /*  update U: */
      /*    matlab: u(:, jj:(jj+1)) = u(:, jj:(jj+1)) * gMat' */
      /* ------------------------------------------------------* */
      p1 = uData + j;  /*   jth   Column of U */
      p2 = p1 + 1;     /*   jth+1 Column of U */
      for (k=0; k<n; k++) {
	 tmp1 = (*p1 * c) - (*p2 * s);
	 *p2  = (*p1 * s) + (*p2 * c);
	 *p1  = tmp1;
	 p1  += uStride;
	 p2   = p1 + 1;
      }

   }/* endFor zero chasing */

   return;
}/* end gkSvdStep() */

/* ************************************************************************** *
 * Function: bidiagonalize
 *
 * Description: Reduces a matrix to bi-diagonal form via a series of
 * Householder transformations
 * 
 * Inputs:
 *   1: rows of the matrix a to be reduced
 *   2: columns of the matrix a to be reduced
 *   3: Reference to the matrix a of size rows by columns
 *   4: Reference to the matrix u of size rows by rows
 *   5: Reference to the matrix v of size columns by columns
 *   6: Pointer to workspace memory
 *
 * Outputs: 
 *   The return value is void
 *   The matrices a, u, and v are transformed
 * ************************************************************************** */
void bidiagonalize(int rows, int cols, 
      ComplexFloat *a, ComplexFloat *u, ComplexFloat *v, tMemory* MM)
{ 
   ComplexFloat   beta;
   ComplexFloat  *aStart=a;
   ComplexFloat  *aEnd=a + (rows*cols);
   int          theStride=cols;
   int          icol, vLen;
   int          numColsToItOver=cols;
   float        epsSq = FLT_EPSILON * FLT_EPSILON;

   /* vector copy pointers */
   ComplexFloat *src, *dst, *dstEnd;

   ComplexFloat phase;
   ComplexFloat tmpSrc;

   for (icol=0; icol<numColsToItOver; icol++) {
      vLen = rows - icol;

      /*                                       %%%  iCOL  %%% */
      /*   Copy the column to be reduced                     */
      /* ................................................... */
      src = aStart + (icol * theStride) + icol;
      dst = MM->m_tmpV;
      dstEnd = dst +vLen;
      while(dst < dstEnd) {
	 *dst++ = *src;
	 src   += theStride;
      }/* endWhile copy */

      /*   Compute Householder vector to reduce the column   */
      /* ................................................... */
      houseCplx(MM->m_tmpV, vLen, &beta);


      /*    Apply Householder transform to a from the left */
      /* ..................................................*/
      applyHouseRows(a, MM->m_tmpV, vLen, icol, rows, cols, beta, MM);

      /* Fill in zeros in the transformed column */
      dst = aStart + (icol+1)*theStride + icol;
      dstEnd = dst+(vLen-1)*theStride;
      while (dst < dstEnd) {
	dst->r = 0.0f;
	dst->i = 0.0f;
	dst += theStride;
	}

      /* Apply the conjugate of the Householder transform */
      /* to u from the right                              */
      /* ................................................ */

      beta.i *= -1.0f;/*     Conjugate Beta */
      /* Conjugate the householder vector */
      dst = MM->m_tmpV;
      dstEnd = dst+vLen;
      while (dst < dstEnd) {
	dst->i *= (-1.0f);
	dst++;
      }
      applyHouseCols(u, MM->m_tmpV, vLen, 0, cols, cols, cols, beta, MM);
      
      if (icol < (cols - 1))
      {
	 if (icol < (cols - 2))
	 {/*                                    %%%  ROW  %%% */
	    vLen = cols - (icol + 1);
	    /*  Copy the row to be transformed                */
	    /* .............................................. */
	    src = aStart + (icol * theStride) + icol + 1;
	    memcpy(MM->m_tmpV,src,vLen*sizeof(ComplexFloat));


	    /* Compute Householder transform to zero the row       */
	    /* ................................................... */
	    houseCplx(MM->m_tmpV, vLen, &beta);

	    /* Apply the transform to a from the right             */
	    /* ................................................... */
	    applyHouseCols(a, MM->m_tmpV, vLen, icol, theStride, 
		           rows, cols, beta, MM);
	    /* Fill in zeros in the transformed row */
	    dst=src+1;
	    dstEnd = dst+vLen;
	    while (dst < dstEnd)
	      {
		dst->r = 0.0f;		
		dst->i = 0.0f;
		dst++;
	      }

	    /* Apply the transform to v from the right             */
	    /* ................................................... */
	    applyHouseCols(v, MM->m_tmpV, vLen, 0, cols, cols, cols, beta, MM);
	 }
	 else
	 {  /*           2nd to last Row,  modify last icol */
	    /*  phase = conj(a[icol, icol+1]) / abs(a[icol, icol+1]) */
	    float tmp;
	    src = aStart + (icol * theStride) + icol + 1;
	    tmp = sqrt((src->r * src->r) + (src->i * src->i));

	    if (tmp > epsSq)
	    {
	       tmp = 1.0f / tmp;
	       phase.r = src->r * tmp;
	       phase.i = - src->i * tmp;

	       while (src < aEnd) {
		  tmpSrc = *src;
		  src->r = tmpSrc.r * phase.r - tmpSrc.i * phase.i;
		  src->i = tmpSrc.r * phase.i + tmpSrc.i * phase.r;
		  src += theStride;
	       }

	       {
		  int vStride=cols;
		  ComplexFloat *vEnd = v + (cols * cols);
		  src = v + vStride + icol + 1;

		  while (src < vEnd) {
		     tmpSrc = *src;
		     src->r = tmpSrc.r * phase.r - tmpSrc.i * phase.i;
		     src->i = tmpSrc.r * phase.i + tmpSrc.i * phase.r;
		     src += vStride;
		  }/* end adjusting V */
	       }
	    }/* endIf non-zero */
	 }/* endIf prior to last 2 rows */
      }/* endIf NOT Last iCol */
   }/* endFor each Column */
   return;
} /* End Function bidiagonalize */

/* ************************************************************************** *
 * Function: extractBidiagonalMatrix
 *
 *  Description: 
 *    Extract 2 vectors from the A Matrix.
 *      1: D, the diagonal
 *      2: S, the super-diagonal 
 * ************************************************************************** */
void extractBidiagonalMatrix(ComplexFloat *a, float *d, float *s, 
                           int m_n, int stride) 
{
   ComplexFloat  *aEnd;
   float         *dest1, *dest2;

   /*                         SuperDiagonal */
   aEnd = a + ((m_n-1) * stride);
   dest1 = d; /*  Diagonal */
   dest2 = s; /*  superDiagonal */

   while (a < aEnd) {
      *dest1++ = (a++)->r;
      *dest2++ = a->r;
      a += stride;
   }/* endWhile */

   *dest1 = a->r;

   return;
} /* End Function extractBidiagonalMatrix */


/* ************************************************************************** *
 * Function: diagonalize
 *
 * Description:
 *   SVD's diagonalization step.
 *   note that at this point we have a bidiagonal matrix all of
 *   whose values are real, i.e. there are no imaginary parts.
 *   based on GVL Algorithm 8.6.2 (3rd Ed p.456)
 *
 *  Inputs: 
 *   1: ptr to array containing the diagonal
 *   2: ptr to array containing the super-diagonal
 *   3: ptr to real matrix for left side accumulation
 *   4: ptr to real matrix for right side accumulation
 *   5: Length of diag array
 *
 *  Outputs:
 *   Return type int: the number of interations performed.
 *   Modifies diagonal, superdiagonal, left and right matrices (1, 2, 3, 4)
 * ************************************************************************** */
int diagonalize(float *d, float*sd, float *ur, float *vr, int m_diagLen)
{
   int  m_itCount = 0; 
   float         *tmpD, *end_p;
   float          offMag;
   float          epsilon, nearZero;
   int            b11Size, b22Size, b33Size;
   int            n = m_diagLen;
   int            maxIterations = n * n * n;
   int            i, num;

   float              matNormEst = 0;
   float             *tmpSD = sd;
   float             tmpNormEst;

   epsilon = n * FLT_EPSILON;
   b33Size = 0;

   end_p = sd + n - 1;/*  1 past last element of Superdiag */
   tmpD  = d;
   while (tmpSD < end_p) {
      tmpNormEst = fabs(*tmpD++) + fabs(*tmpSD++);
      if (tmpNormEst > matNormEst)
	 matNormEst = tmpNormEst;
   }

   /*  tmpD now points to the last diag elt - we are past last */
   /*     super-diag elt. */
   if (fabs(*tmpD) > matNormEst)
      matNormEst = fabs(*tmpD);

   nearZero = FLT_EPSILON * matNormEst;

   while (b33Size != n) {

      /*    Check the super-diag for very-small quantities. */
      /*       Set any we find to zero. */
      /*    ------------------------------------------------------* */
      for (num=n-b33Size-1, i=0; i<num; i++) {
	 offMag  = epsilon *(fabs(d[i]) + fabs(d[i+1]));

	 if (fabs(sd[i]) < offMag)
	 {
	    sd[i] = 0;
	 }/* endIf near zero */
      }/* endFor each super-diag to look at */
      partitionBiDiag(sd, &b11Size, &b22Size, &b33Size, m_diagLen); 

      if (b33Size < n)
      {
	 /* 	  Check for zeros along diagonal */
	 /* 	 ----------------------------------------* */
	 tmpD  = d + b11Size;   /*  pt to 1st B22 diag elt */
	 end_p = tmpD + b22Size;/*  pt just past last B22 diag */

	 while (tmpD < end_p) {

	    if (fabs(*tmpD++) < nearZero)
	    {
#ifdef VERBOSE
	       printf("SVD: diag values near ZERO-need to implement! %s, %d\n",
		     __FILE__, __LINE__);
#endif
	    }
	 }/* endWhile */

	 if (b22Size > 1)
	 {
	    gkSvdStep(d, sd, b11Size, b22Size, ur, vr, n, m_diagLen); 
	 }

      }/* EndIf B33 not whole matrix */

      if (++m_itCount > maxIterations)
      {
#ifdef VERBOSE
	 printf("SVD: did NOT Converge!!!! %s, %d\n", __FILE__, __LINE__);
#endif
      }

   }/* endWhile b33Size != n */

   return(m_itCount);
} /* End Function diagonalize */


/* ************************************************************************** *
 * Function: makePositive
 *
 * Description: Make the singular values positive
 * ************************************************************************** */
void makePositive(float *sVals, int len, float *vr, float *vrEnd, int vrStride)
{
   float          *d, *vEnd, *vData;
   int         i;

   /*        Make the Singular Values all Positive */
   /*    Note: for any negative values */
   /*           1.) make it positive */
   /*           2.) negate the corresponding column in V */
   /*   ==========================================================* */
   d = sVals;
   vEnd = d + len;
   i = 0;

   while (d < vEnd) {
      if (*d < 0.0)
      {
	 *d *= -1.0;
	 /*   If we are computing V */
	 /*     negate the correnponding col of V-real */
	 /* --------------------------------------------* */
	 vData = vr+ i;
	 while (vData < vrEnd) {
	    *vData *= -1.0;
	    vData += vrStride;
	 }/* endWhile negating V column */
      }/* endIf found a neg value */
      ++d;
      ++i;
   }/* endWhile */
   return;
} /* End Function makePositive */


/* ************************************************************************** *
 * Function: sortSingulars
 *
 * Description: Sort the Singular values into Descending Order
 * Note:
 *   The columns of U must correspond to their Singular Values. 
 *   i.e. if we move a singular value, we must then move the 
 *   corresponding col in U.
 *
 * ************************************************************************** */
void sortSingulars(float *startLook, int len, ComplexFloat *s,
                   float          *ur, float *urs,
		   float          *vr, float *vrs,
		   int urStride, int vrStride)
{
   float          *d;
   float          *vEnd = startLook + len;
   ComplexFloat *sData = s;
   float           maxVal;
   int         maxIndex = 0;
   int         index = 0;
   int         i;
   float *src, *dst, *dstEnd;

   for (i=0; i<len; i++) {
      d = startLook;
      index = 0;
      maxVal = 0.0f;

      while (d < vEnd) {
	 if (*d > maxVal)
	 {
	    maxVal = *d;
	    maxIndex = index;
	 }

	 ++d;
	 ++index;
      }/* endWhile getting next biggest */

      /*      Got the next biggest Value */
      /*      Put it into the singular value vector S */
      /*      Copy the corresponding Row from U to A */
      /*      mark value as used with a -1, so we don't pick it again */
      /*      ==========================================================* */
      *(startLook + maxIndex) = -1.0f;/*  replace with negative */
      sData->r = maxVal;
      sData->i = 0.0f;
      ++sData;

      /*              Copy Column of U */
      /*   Note that our temp Ureal & Ureal+sorted */
      /*       are square NxN matrices */
      /* -------------------------------------------------* */
      src = ur+maxIndex;
      dst = urs+i;
      dstEnd =  dst +(len*urStride);
      while(dst < dstEnd)
      {
	 *dst = *src;
	 src += urStride;
	 dst += urStride;
      }
      

      /*              Copy Col of V */
      src = vr+maxIndex;
      dst = vrs+i;
      dstEnd = dst + (len*vrStride);
      while(dst < dstEnd)
      {
	 *dst = *src;
	 src += vrStride;
	 dst += vrStride;
      }
      
   }/* endFor each Col */


   return;
} /* End Function sortSingulars */


/* ************************************************************************** *
 * Function: mat_mult_cx_by_cx
 * Description: Multiply one complex matrix by another
 *  Matlab: rslt = m0 * m1;
 * ************************************************************************** */
void mat_mult_cx_by_cx(int rows, int cols, ComplexFloat *rslt, ComplexFloat *m0, ComplexFloat *m1)
{
   ComplexFloat* cM = m0;
   ComplexFloat* xM = m1;
   int  middleDim = cols; 

   ComplexFloat   tmpC;
   ComplexFloat   *cData;
   ComplexFloat   *xData;
   ComplexFloat   *rsltData;
   ComplexFloat   *rsltStart = rslt;
   int           c, r, i;
   int           stride  = cols; 

   for (r=0; r<rows; r++) {
      rsltData = rsltStart + (r*stride);

      for (c=0; c<cols; c++) {
	 tmpC.r = 0.0f;
	 tmpC.i = 0.0f;
	 cData = cM + (r*middleDim);
	 xData = xM + c;

	 for (i=0; i<middleDim; i++) {
	    tmpC.r += (xData->r * cData->r) - (xData->i * cData->i);
	    tmpC.i += (xData->r * cData->i) + (xData->i * cData->r);

	    xData += cols;
	    ++cData;
	 }/* end InnerProduct */

	 *rsltData++ = tmpC;
      }/* endFor each column */
   }/* endFor each Row */
   return;
} /* End Function mat_mult_cx_by_cx */


/* ************************************************************************** *
 * Function: mat_mult_cx_by_real
 * Description: Multiply a complex matrix by a real matrix
 * Matlab: rslt = m0*m1;
 *
 * ************************************************************************** */
void mat_mult_cx_by_real(int rows, int cols, ComplexFloat *rslt, ComplexFloat *m0, float *m1)
{
   ComplexFloat* cM = m0;
   float*          rM = m1;
   int  middleDim = cols; 

   ComplexFloat   tmpC;
   ComplexFloat   *cData;
   float          *rData;
   ComplexFloat   *rsltData;
   ComplexFloat   *rsltStart = rslt;
   int           c, r, i;
   int           stride  = cols; 

   for (r=0; r<rows; r++) {
      rsltData = rsltStart + (r*stride);

      for (c=0; c<cols; c++) {
	 tmpC.r = 0.0f;
	 tmpC.i = 0.0f;
	 cData = cM + (r*middleDim); 
	 rData = rM + c;

	 for (i=0; i<middleDim; i++) {
	    tmpC.r += *rData * cData->r;
	    tmpC.i += *rData * cData->i;

	    rData += cols;
	    ++cData;
	 }/* end InnerProduct */

	 *rsltData++ = tmpC;
      }/* endFor each column */
   }/* endFor each Row */
   return;
} /* End Function mat_mult_cx_by_real */


/* ************************************************************************** *
 * Function: svd
 *  An implementation of the Golub-Kahan SVD algorithm as described in
 *   Golub and Van Loan, Algorithm 8.6.2. Produces a reduced (in Matlab
 *   terms, 'economy sized') decomposition. Produces the output matrices
 *   such that a = u * s * v'
 *
 * ************************************************************************** */
int svd(int m, int n,
         ComplexFloat *a,  
         ComplexFloat *u,  
	 ComplexFloat *s,       
	 ComplexFloat *v,
	 tMemory      *MM)
{

   int vrStride = n;
   int urStride = n;
   int it_count = 0;
   float  *vrEnd;
    
   ComplexFloat *svdTarget;
   ComplexFloat *u_ptr;

   vrEnd = MM->m_vr + (n*n-1);

   if (m>n)
   {
      /* if #rows > #cols do Qr first */
      qr(m, n, a, MM->m_q, MM->m_r);
      svdTarget = MM->m_r;
      u_ptr = MM->m_uPreQr;
   }
   else /* a is a square matrix */
   {
      svdTarget = a;
      u_ptr = u;
   }

   /* ************** straightSvd ********************* */
   bidiagonalize(n,n,svdTarget, MM->m_ut, MM->m_vt, MM);

   extractBidiagonalMatrix(svdTarget, MM->m_diag, MM->m_sdiag, n, n);

   it_count = diagonalize(MM->m_diag, MM->m_sdiag, MM->m_ur, MM->m_vr, n);

   makePositive(MM->m_diag, n, MM->m_vr, vrEnd, vrStride);

   /*      Sort the Singular values into Descending Order */
   /*    ====================================================* */


   sortSingulars(MM->m_diag, n, s, MM->m_ur, MM->m_urs, MM->m_vr, 
	         MM->m_vrs, urStride, vrStride);


   /*     Update U and V  */
   /*    ====================================================* */
   /*   U = Ut * Urs */
   /* -------------------------------------------------------* */
   mat_mult_cx_by_real(n, n, u_ptr, MM->m_ut , MM->m_urs);


   /*   V = Vt * Vrs */
   /* -------------------------------------------------------* */
   mat_mult_cx_by_real(n, n, v, MM->m_vt , MM->m_vrs);
   /* ************** end straightSvd ***************** */


   if(m > n)
   {
      mat_mult_cx_by_cx(m, n, u, MM->m_q, u_ptr);
   }



   return(it_count);
} /* End Function svd */

/* ************************************************************************** *
 * Function: setupMem
 *
 * Setup mem for the SVD computation.  Allocate arrays
 * ************************************************************************** */
void setupMem(int m, int n, tMemory *M)
{
   M->m_tmpW   = pca_malloc( ComplexFloat, m);
   M->m_tmpP   = pca_malloc( ComplexFloat, m*m);
   M->m_tmpBW  = pca_malloc( ComplexFloat, m);
   M->m_tmpBV  = pca_malloc( ComplexFloat, m);
   M->m_tmpV   = pca_malloc( ComplexFloat, m);
   M->m_urs    = pca_malloc( float       , n*n);
   M->m_vrs    = pca_malloc( float       , n*n);
   M->m_diag   = pca_malloc( float       , n);
   M->m_sdiag  = pca_malloc( float       , n);
   M->m_ur     = pca_malloc( float       , n*n);
   M->m_vr     = pca_malloc( float       , n*n);
   M->m_uPreQr = pca_malloc( ComplexFloat, n*n);
   M->m_q      = pca_malloc( ComplexFloat, m*n);
   M->m_r      = pca_malloc( ComplexFloat, n*n);
   M->m_ut     = pca_malloc( ComplexFloat, n*n);
   M->m_vt     = pca_malloc( ComplexFloat, n*n);
}


/* ************************************************************************** *
 * Function: cleanMem
 *
 * Clean mem for the SVD computation.  Free arrays
 * ************************************************************************** */
void cleanMem( tMemory *M)
{
   free( M->m_tmpW);
   free( M->m_tmpP);
   free( M->m_tmpBW);
   free( M->m_tmpBV);
   free( M->m_tmpV);
   free( M->m_urs);
   free( M->m_vrs);
   free( M->m_diag);
   free( M->m_sdiag);
   free( M->m_ur);
   free( M->m_vr);
   free( M->m_uPreQr);
   free( M->m_q);
   free( M->m_r);
   free( M->m_ut);
   free( M->m_vt);
}



/* ************************************************************************** *
 * Function: main
 *
 * Setup data for the SVD computation.  Time the SVD and output verification
 * data.
 * ************************************************************************** */

int main(int argc, char **argv)
{

   int m, n;
   int i , numEls;
   /* arrays */
   PcaCArrayFloat matrix_a, matrix_u, vector_s, matrix_v;
   float *p_ut, *p_ur, *p_vt, *p_vr;
   int d_stride_t, d_stride_r;
   int itCount;
    
   /* Temp Mem structure */
   tMemory MM;

   /* timer variables */
   pca_timer_t svd_time;
   PcaCArrayFloat run_time;

   /* file names */
   char infile[STR_SZ], out_u[STR_SZ], out_s[STR_SZ], out_v[STR_SZ], timefile[STR_SZ];
  
   if (argc != 2) 
   {
      printf("Usage: %s <data set num>\n", argv[0]);
      return -1;
   }
 
   /* Build the input and output file names.  Output a usage message */
   /* if no arguments are given.                                     */
   sprintf(infile,  "./data/%s-svd-inmatrix.dat", argv[1]);
   sprintf(out_u,   "./data/%s-svd-outmatrix_u.dat", argv[1]);
   sprintf(out_v,   "./data/%s-svd-outmatrix_v.dat", argv[1]);
   sprintf(out_s,   "./data/%s-svd-outvector_s.dat", argv[1]);
   sprintf(timefile,  "./data/%s-svd-timing.dat", argv[1]);


   readFromFile(float,infile,matrix_a);
   m = matrix_a.size[0];
   n = matrix_a.size[1];
   itCount = n;

   setupMem(m,n,&MM);
   if (n>m)
   {
#ifdef VERBOSE
      printf("SVD valid for # of Rows >= # of Cols only!\n");
#endif
      return -1;
   }


   pca_create_carray_1d(float, vector_s, n, PCA_COMPLEX); 
   pca_create_carray_2d(float, matrix_u, m, n, PCA_COMPLEX);
   pca_create_carray_2d(float, matrix_v, n, n, PCA_COMPLEX);
   pca_create_carray_1d(float, run_time, 2, PCA_REAL);
   numEls = n*n;

   memset(MM.m_r,0,numEls*sizeof(ComplexFloat));
   memset(MM.m_ut,0,numEls*sizeof(ComplexFloat));
   memset(MM.m_vt,0,numEls*sizeof(ComplexFloat));
   memset(MM.m_ur,0,numEls*sizeof(float));
   memset(MM.m_vr,0,numEls*sizeof(float));

   d_stride_t = 2*(n+1);
   d_stride_r = (n+1);
   p_ut = (float *)MM.m_ut; p_vt = (float*)MM.m_vt;
   p_ur = MM.m_ur;          p_vr = MM.m_vr;
   /* initialize temporary matricies to identity */
   for (i = 0; i < n; i++)
   {
     /* matrix_u.data[i] = 1.0f;*/
      *p_ut = 1.0f;
      *p_vt = 1.0f;
      *p_ur = 1.0f;
      *p_vr = 1.0f;
      p_ut += d_stride_t;
      p_vt += d_stride_t;
      p_ur += d_stride_r;
      p_vr += d_stride_r;
   }

   svd_time = startTimer();

   itCount = svd(m,n,
      (ComplexFloat *)&matrix_a.data[0], 
      (ComplexFloat *)&matrix_u.data[0], 
      (ComplexFloat *)&vector_s.data[0], 
      (ComplexFloat *)&matrix_v.data[0], &MM);

   run_time.data[0] = stopTimer(svd_time);
   run_time.data[1] = (float)itCount;
#ifdef VERBOSE
   printf("Iterations: %d\n",itCount);
#endif
   printf("Done.  Latency: %f s.\n", run_time.data[0]);

   writeToFile(float,out_u,matrix_u);
   writeToFile(float,out_s,vector_s);
   writeToFile(float,out_v,matrix_v);
   writeToFile(float,timefile,run_time);

   /* clean up */
   cleanMem(&MM);
   clean_mem(float, vector_s); 
   clean_mem(float, matrix_u);
   clean_mem(float, matrix_v);
   clean_mem(float, matrix_a);
   clean_mem(float, run_time);
   return 0;
} /* End Function main */
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
