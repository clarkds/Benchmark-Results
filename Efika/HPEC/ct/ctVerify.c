/******************************************************************************
** File: ctVerify.c
**
** HPEC Challenge Benchmark Suite
** CornerTurn Kernel Benchmark
**
** Contents:
**  The verifier of the ANSI C Corner Turn kernel.
**
** Input/Output:
**  The input matrix is stored in file 
**              "./data/<dataSetNum>-ct-truthmatrix.dat".
**  The output matrix is stored in file 
**              "./<dataSetNum>-ct-outmatrix.dat".
**
** Command:
**   ctVerify <data set num> 
**
** Author: Hector Chan
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <stdio.h>
#include "PcaCArray.h"

int verify(PcaCArrayFloat *truth, PcaCArrayFloat *out)
{
  unsigned int i, j;
  unsigned int numrows = truth->size[0];
  unsigned int numcols = truth->size[1];

  /* Check if the dimension of the matrices are correct */
  if (truth->ndims != 2 || out->ndims != 2) {
#ifdef VERBOSE
    printf("Error: truth or output matrix dimension is not equal to 2");
#endif
    return 0;
  }

#ifdef VERBOSE
  printf("Input:\n");
  for (i=0; i<numrows; i++) {
    for (j=0; j<numcols; j++)
      printf("%f ", ((float**)(truth->datav))[i][j]);
    printf("\n");
  }
  printf("\n");

  printf("Output:\n");
  for (i=0; i<numcols; i++) {
    for (j=0; j<numrows; j++)
      printf("%f ", ((float**)(out->datav))[i][j]);
    printf("\n");
  }
  printf("\n");
#endif

  for (i=0; i<numrows; i++) {
    for (j=0; j<numcols; j++) {
      if ( ((float**)(truth->datav))[i][j] != ((float**)(out->datav))[i][j] )
	return 0;
    }
  }

  return 1;
}

int main(int argc, char **argv)
{
  PcaCArrayFloat truthmatrix, outmatrix;
  char           truthmatrixfile[100], outmatrixfile[100];

  if (argc != 2) {
    printf("Usage: %s <data set num>\n", argv[0]);
    return -1;
  }

  /* Build the input file names, assume data set 0 if no argument given */
  sprintf(truthmatrixfile, "./data/%s-ct-truthmatrix.dat", argv[1]);
  sprintf(outmatrixfile, "./data/%s-ct-outmatrix.dat", argv[1]);

  /* Read the input and output matrices from file */
  readFromFile(float, truthmatrixfile, truthmatrix);
  readFromFile(float, outmatrixfile, outmatrix);

  /* Run the verifier */
  printf("Verification: ");
  if (verify(&truthmatrix, &outmatrix)) {
    printf("PASS\n");
  }
  else {
    printf("FAIL\n");
  }

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
