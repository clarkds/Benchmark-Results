/******************************************************************************
** File: pmVerify.c
**
** HPEC Challenge Benchmark Suite
** Pattern Match Kernel Benchmark
**
** Contents:
**  The verifier of the ANSI C Pattern Match kernel.
**
** Input/Output:
**  The matched pattern index is stored in file 
**           "./data/<dataSetNum>-pm-patnum.dat".
**  The original pattern index is stored in file 
**           "./data/<dataSetNum>-pm-origpatnum.dat".
**
** Command:
**   pmVerify <data set num> 
**
** Author: Hector Chan
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <stdio.h>

#include "PcaCArray.h"

int verify(PcaCArrayInt *origpatnum, PcaCArrayInt* patnum)
{
#ifdef VERBOSE
  printf("Original pattern is %d in one based array indexing.\n", origpatnum->data[0]);
  printf("Pattern matches in the kernel is %d in one based array indexing.\n", patnum->data[0]+1);
#endif

  /* add 1 to the pattern number found from C kernel to 
   * offset matlab's 1 based array indexing */
  if (origpatnum->data[0] == (patnum->data[0]+1))
    return 1;
  else
    return 0;
}

int main(int argc, char **argv)
{
  PcaCArrayInt origpatnum;
  PcaCArrayInt patnum;
  char         origpatnumfile[100], patnumfile[100];

  if (argc != 2) {
    printf("Usage: %s <data set num>\n", argv[0]);
    return -1;
  }

  /* Build the input file names */
  sprintf(origpatnumfile, "./data/%s-pm-origpatnum.dat", argv[1]);
  sprintf(patnumfile, "./data/%s-pm-patnum.dat", argv[1]);

  /* Read the matched pattern index from files */
  readFromFile(int, origpatnumfile, origpatnum);
  readFromFile(int, patnumfile, patnum);

  printf("Verification: ");
  if (verify(&origpatnum, &patnum) == 1)
    printf("PASS\n");
  else {
    printf("FAIL *Note see Documentation*\n");
#ifdef VERBOSE
    printf("Note: Pattern match functional performance degrades ");
    printf("if pattern length is too long (approximately when L > 256). ");
    printf("Verification failure may be a result of this effect.\n");
#endif
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
