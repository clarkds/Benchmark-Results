/******************************************************************************
** File: ct.c
**
** HPEC Challenge Benchmark Suite
** CornerTurn Kernel Benchmark
**
** Contents:
**  The ANSI C Corner Turn kernel.
**
** Note:
**  This kernel has been stripped out of all optimizations such as
**  AltiVec or SSE.
**
** Input/Output:
**  The input matrix is stored in file 
**                  "./data/<dataSetNum>-ct-inmatrix.dat".
**  The output matrix will be stored in file 
**                  "./data/<dataSetNum>-ct-outmatrix.dat".
**  The total run time will be stored in file 
**                  "./data/<dataSetNum>-ct-timing.dat".
**
** Command:
**   ct <data set num>
**
** Author: Hector Chan
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <stdio.h>

#include "PcaCArray.h"
#include "PcaCTimer.h"

void ct(int numrows, int numcols, float *in, float *out)
{
  unsigned int i, j;

  for (i=0; i<numrows; i++) {
    for (j=0; j<numcols; j++)
      *(out + j * numrows + i) = *(in + i * numcols + j);
  }
}

int main(int argc, char **argv)
{
  PcaCArrayFloat inmatrix, outmatrix, rtime;
  pca_timer_t    timer;
  char           inmatrixfile[100], outmatrixfile[100], timefile[100];

  if (argc != 2) {
    printf("Usage: %s <data set num>\n", argv[0]);
    return -1;
  }

  /* Build the input file names */
  sprintf(inmatrixfile, "./data/%s-ct-inmatrix.dat", argv[1]);
  sprintf(outmatrixfile, "./data/%s-ct-outmatrix.dat", argv[1]);
  sprintf(timefile, "./data/%s-ct-timing.dat", argv[1]);

  /* Read the input matrix from file */
  readFromFile(float, inmatrixfile, inmatrix);

  /* Allocate memory for output matrix and run time */
  pca_create_carray_2d(float, outmatrix, inmatrix.size[1], inmatrix.size[0], PCA_REAL);
  pca_create_carray_1d(float, rtime, 1, PCA_REAL);

  /* Run corner turn */
  timer = startTimer();
  ct(inmatrix.size[0], inmatrix.size[1], inmatrix.data, outmatrix.data);
  rtime.data[0] = stopTimer(timer); /* time is in second  */

#ifdef VERBOSE
  printf("Time: %f sec\n", rtime.data[0]);
#endif
  printf("Done.  Latency: %f s.\n", rtime.data[0]);

  /* Write the run time and output matrix to file */
  writeToFile(float, timefile, rtime);
  writeToFile(float, outmatrixfile, outmatrix);

  /* Clean the memory */
  clean_mem(float, rtime);
  clean_mem(float, outmatrix);
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
