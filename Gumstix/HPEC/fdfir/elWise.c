/******************************************************************************
** File: elWise.c
**
** HPEC Challenge Benchmark Suite
** FDFIR Kernel Benchmark
**
** Contents:
** Desc    : This file provides definitions for elMul and elDiv functions 
**           in support of the generic C FDFIR filter implementation.
**
** Author: Matthew A. Alexander
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include "fdFir.h"



void elMul(float *dataPtr, float *filterPtr, int inputLength)
{
  int index;
  float data_r, data_i, filt_r, filt_i;


  for(index = 0; index < inputLength; index++)
    {
      data_r = *dataPtr;
      data_i = *(dataPtr+1);
      filt_r = *filterPtr;
      filt_i = *(filterPtr+1);

      *dataPtr     = data_r * filt_r - data_i * filt_i;
      *(dataPtr+1) = data_r * filt_i + data_i * filt_r;

      /*    Go to the next elements    */
      dataPtr   = dataPtr + 2;
      filterPtr = filterPtr + 2; 
    }
}



void elDiv(float *dataPtr, int inputLength)
{
  int index;
  int div = inputLength;

  for (index = 0; index < inputLength; index++)
    {
      /*  real  */
      *dataPtr = (*dataPtr) / div;
      dataPtr++;
      /*  imag  */
      *dataPtr = (*dataPtr) / div;
      dataPtr++;
    }
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
