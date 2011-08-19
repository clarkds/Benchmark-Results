/******************************************************************************
** File: fdFir.h
**
** HPEC Challenge Benchmark Suite
** FDFIR Kernel Benchmark
**
** Contents: This include file provides declarations for various functions in 
**           support of the generic C Frequency-domain FIR filter bank 
**           implementation.
**
** Author: Matthew A. Alexander
**         MIT Lincoln Laboratory
**
******************************************************************************/

#ifndef FDFIR_FREQ_H_
#define FDFIR_FREQ_H_

#include "PcaCArray.h"

/*
  This implementation has only been tested as a RADIX 4.
  Other RADIX may not work.
*/
#define RADIX 4
#define PI 3.1415926535897932384

struct fdFirVariables{
  PcaCArrayFloat input;
  PcaCArrayFloat filter;
  PcaCArrayFloat time;
  float *freqFilterPtr;
  float *twiddlePtr;
  float *twiddleConjPtr;
  int   inputLength;
  int   numFilters;
  int   filterLength;
  int   currentFilter;
  int   butterflies;
  int   phases;
  int   stride;
  int   arguments;
  char  *dataSet;
}fdFirVars; 

void fdFirSetup(struct fdFirVariables *fdFirVars);
void fdFir(struct fdFirVariables *fdFirVars);
void fdFirComplete(struct fdFirVariables *fdFirVars);
void fft(int filter, int inputLength, int phases,
	 int butterflies, int stride, float * inputData,
	 float * twiddlePtr);
void elMul(float *dataPtr, float *filterPtr, int inputLength);
void ifft(struct fdFirVariables *fdFirVars);
void elDiv(float *dataPtr, int inputLength);
void createTwiddles(float * twiddlePtr, float * twiddleConjPtr, int inputLength);
void createFftTwiddles(float *twiddlePtr, int inputLength);
void createFreqFilter(struct fdFirVariables *fdFirVars);
int  computeNumPhases(int inputLength);
int verifyLength(int inputLength);


#define CPLX_MUL(ar,ai,br,bi) { \
float temp_r = ar;              \
ar = ar     * br - ai * bi;     \
ai = temp_r * bi + ai * br;     \
}



#endif
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
