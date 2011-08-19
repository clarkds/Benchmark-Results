/******************************************************************************
** File: tdFir.h
**
** HPEC Challenge Benchmark Suite
** TDFIR Kernel Benchmark
**
** Contents:
** Desc    : This include file provides declarations for various functions 
**           in support of the generic C TDFIR implementation.
**
** Author: Matthew A. Alexander 
**         MIT Lincoln Laboratory
**
******************************************************************************/

#ifndef TDFIR_H_
#define TDFIR_H_

#include "PcaCArray.h"

struct tdFirVariables{
  PcaCArrayFloat input;
  PcaCArrayFloat filter;
  PcaCArrayFloat result;
  PcaCArrayFloat time;
  int   numFilters;
  int   inputLength;
  int   filterLength;
  int   resultLength;
  int   arguments;
  char  *dataSet;
}tdFirVars; 

void tdFirSetup(struct tdFirVariables *tdFirVars);
void tdFir(struct tdFirVariables *tdFirVars);
void tdFirComplete(struct tdFirVariables *tdFirVars);
void elCplxMul(float *dataPtr, float *filterPtr, 
	       float *resultPtr, int inputLength);
void printVector(float * dataPtr, int inputLength);
void zeroData(float *dataPtr, int length, int filters);




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
