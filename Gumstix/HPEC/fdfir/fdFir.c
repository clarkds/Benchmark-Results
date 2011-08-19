/******************************************************************************
** File: fdFir.c
**
** HPEC Challenge Benchmark Suite
** FDFIR Kernel Benchmark
**
** Contents:
** Desc    : This file provides definitions for various functions in support 
**           of the generic C Frequency-domain FIR filter bank implementation.
**            Inputs: ./data/<dataset>-fdFir-input.dat           
**                    ./data/<dataset>-fdFir-filter.dat          
**            Outputs:./data/<dataset>-fdFir-output.dat          
**                    ./data/<dataset>-fdFir-time.dat            
**
** Author: Matthew A. Alexander
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include <math.h>  /* for sin() and cos() in createTwiddles()*/
#include <stdlib.h>
#include <stdio.h>
#include "fdFir.h"
#include "PcaCTimer.h"

int main(int argc, char **argv)
{
  fdFirVars.arguments = argc;
  fdFirVars.dataSet = argv[1];

  /*
    I need to declare some variables:
      -pointers to data, filter, and twiddle factors.
      -inputLength;
      -butterflies;
      -phases;
    These are all in the fdFirVariables structure in fdFir.h
    Along with the structure definition, I also instantiate an 
    instanced called fdFirVars.  This is the same as doing:
    
    struct fdFirVaribles fdFirVars;
  */
  

  


  /*
    In fdFirSetup, I want to perform tasks that I do NOT want to include
    in my timing.  For example:
      -allocating space for:
         -data
	 -filter
	 -twiddle factors
      -calculating twiddle factors
    The declarations for this function can be found in fdFir.h, while
    the definitions of these functions can be found in fdFir.c.
  */
  fdFirSetup(&fdFirVars);






  /*
    In fdFir, I want to call the individual routines that make-up an
    FDFIR filter:
      -fft()
      -elMul()
      -ifft()
      -elDiv()
    The declaration for fdFir() can be found in fdFir.h, while
    the definition of fdFir() can be found in fdFir.c.
  */
  fdFir(&fdFirVars);







  /*
    In fdFirComplete(), I fdFirst want to output my result to output.dat.  
    I then want to do any required clean up.
    The fdFirComplete() declaration can be found in fdFir.h, while
    the definition of fdFirComplete() can be found in fdFir.c.
  */
  fdFirComplete(&fdFirVars);





  return 0;
}







/*
  In fdFirSetup, I want to perform tasks that I do NOT want to include
  in my timing.  For example:
    -allocating space for:
      -data
      -filter
      -twiddle factors
    -calculating twiddle factors
  The declarations for this function can be found in fdFir.h, while
  the definitions of these functions can be found in fdFir.c.
*/
void fdFirSetup(struct fdFirVariables *fdFirVars)
{

  int inputLength;
  char dataSetString[100];
  char filterSetString[100];

  if(fdFirVars->arguments == 1)
    {
      printf("No dataset provided\n");
      printf("Usage: fdFir <dataset> \n");
      exit(-1); /*return ;*/
    }


  sprintf(  dataSetString,"./data/%s-fdFir-input.dat",fdFirVars->dataSet);
  sprintf(filterSetString,"./data/%s-fdFir-filter.dat",fdFirVars->dataSet);

  /*
    input read from file 'input.dat', and stored at:   fdFirVars->input.data
    fileter read from file 'filter.dat' and stored at: fdFirVars->filter.data
  */
  readFromFile(float, dataSetString, fdFirVars->input);    
  readFromFile(float, filterSetString, fdFirVars->filter);  

  pca_create_carray_1d(float, fdFirVars->time, 1, PCA_REAL);

  inputLength          = fdFirVars->input.size[1];
  fdFirVars->inputLength = fdFirVars->input.size[1];
  fdFirVars->numFilters  = fdFirVars->filter.size[0];
  fdFirVars->filterLength= fdFirVars->filter.size[1];
  fdFirVars->butterflies = fdFirVars->inputLength / RADIX;  /* Radix 4*/
  fdFirVars->stride      = fdFirVars->butterflies * 2;      /* cplx data */
  fdFirVars->phases      = computeNumPhases(inputLength);


  /*  Verify length is a power of 4. */
  if( !verifyLength(fdFirVars->inputLength) )  /* invalid length*/
    {
#ifdef VERBOSE
      printf("Length is not a power of 4. FFT & iFFT do not \n");
      printf("non powers of 4. \n");
#endif
    }


  /*
    allocate some space at twiddlePtr for the twiddle factors
    I'm about to compute.
  */
  fdFirVars->twiddlePtr      = malloc((2*(inputLength)*sizeof(float)));
  fdFirVars->twiddleConjPtr  = malloc((2*(inputLength)*sizeof(float)));


  /*
    Let's create the twiddle factors, and put them at twiddlePtr
         for jj = 0:inputLength-1
            val(jj+1) = (-i*2*pi*jj)/inputLength;
         end
    This will be done in createTwiddles().  The declaration for this
    function can be found in fdFir.h while the definition can be
    found here in fdFir.c
  */
  createTwiddles(fdFirVars->twiddlePtr, fdFirVars->twiddleConjPtr, inputLength);




  /*
    Finally, we need to put the time domain filter into the frequency domain.
  */
  createFreqFilter(fdFirVars);

}







/*
  In fdFir, I want to call the individual routines that make-up an
  FDFIR filter:
    -fft()
    -elMul()
    -ifft()
    -elDiv()
  The declaration for fft(), elMul(), ifft(), and elDiv() can be 
  found in fdFir.h, while the definition can be found as follows:
    -fft()   - fft.c
    -elMul() - elWise.c
    -ifft()  - ifft.c
    -elDiv() - elWise.c
*/
void fdFir(struct fdFirVariables *fdFirVars)
{
  int filter = 0;
  float * filterPtr = fdFirVars->freqFilterPtr;
  float * resultPtrSave = fdFirVars->input.data;
  float * resultPtr = fdFirVars->input.data;

  /*
    I will need a timer to evaluate my functions.  The
    pca_timer_t is located in PcaCTimer.h
  */
  pca_timer_t t;
  t = startTimer();

  for (filter = 0; filter < fdFirVars->numFilters; filter++)
    {
      fdFirVars->currentFilter = filter;
      resultPtr = resultPtrSave + (2 * fdFirVars->inputLength * (filter));
  /*
    fft does a fast fourier transform on the input.  This input of 
    length 'inputLength' is located at fdFirVars->input.data.  We also
    need the twiddle factors to compute the fft of the input.  
    Input Parameters:
    fdFirVars  -  pointer to an instance of the fdFirVars class.  See fdFir.h
                for a definition of the fdFirVars class.
  */
      fft(fdFirVars->currentFilter, fdFirVars->inputLength, fdFirVars->phases,
	  fdFirVars->butterflies, fdFirVars->stride, fdFirVars->input.data,
	  fdFirVars->twiddlePtr);


  /*
    elMul does an element wise multiply of the fft result and the 
    filter. Note that I continue to reuse the space reserved for 
    the input  (fdFirVars->input.data).  Also note that the output from
    the fft is bit reversed; therefore, I multiply the fft result by
    a filter whose indices are bit reversed.
    Input Parameters:
    fdFirVars->input.data  - pointer to one of the vectors
    filterPtr.data       - pointer to the other input.
    fdFirVars->inputLength - integer value representing length in input
                           vectors.
  */

  elMul(resultPtr,  filterPtr, fdFirVars->inputLength); 
  /*We need to now move the filter pointer to the next filter in the bank. */
  filterPtr = filterPtr + (2 * fdFirVars->inputLength);
 

  /*
    ifft does an inverse  fast fourier transform on the input.  This 
    input of length 'inputLength' is located at fdFirVars->input.data.  
    We also need the twiddle factors to compute the fft of the input. 
    Input Parameters:
    fdFirVars  -  pointer to an instance of the fdFirVars class.  See fdFir.h
                for a definition of the fdFirVars class.
  */

  /*  VERY IMPORTANT IF YOU PLAN TO USE YOUR OWN iFFT 
      Normally, we do an divide by inputLength on each
      element in the LAST Phase.  For optimization 
      purposes, I've moved the elDivide into the elDiv
      routine.  
  */
  
  ifft(fdFirVars);
  elDiv(resultPtr,  fdFirVars->inputLength);


  /*
    elDiv does an element wise divide by the inputLength to finish the ifft result.
    Note that I continue to reuse the space reserved for 
    the input  (fdFirVars->input.data)   
    Input Parameters:
    fdFirVars->input.data  - pointer to the vector
    fdFirVars->inputLength - integer value representing length of the input vector
  */

  

    }/*end for filters*/
  fdFirVars->time.data[0] = stopTimer(t);
  
  printf("Done.  Latency: %f s.\n", fdFirVars->time.data[0]);
  

}






/*
  In fdFirComplete, I want to do any required clean up.
    -...
*/
void fdFirComplete(struct fdFirVariables *fdFirVars)
{
  char timeString[100];
  char outputString[100];
  
  sprintf(timeString,"./data/%s-fdFir-time.dat",fdFirVars->dataSet);
  sprintf(outputString,"./data/%s-fdFir-output.dat",fdFirVars->dataSet);
 
  
  writeToFile(float, outputString, fdFirVars->input);
  writeToFile(float, timeString, fdFirVars->time);
  
  clean_mem(float, fdFirVars->input);
  clean_mem(float, fdFirVars->filter);
  clean_mem(float, fdFirVars->time);
}

void createFreqFilter(struct fdFirVariables *fdFirVars)
{

  int cnt,cnt2;
  int butterflies, stride, phases, paddedFilterLength, paddedZeros;
  int numFilters   = fdFirVars->filter.size[0];
  int filterLength = fdFirVars->filter.size[1];
  int inputLength  = fdFirVars->input.size[1];
  float paddedPhases;
  float* paddedFilterPtr;
  float* paddedFilterPtrSave;
  float* inPtr;
  float* outPtr;
  float* filterPtr = fdFirVars->filter.data;

  /* calculate the length of the new freqFilter.  */
  paddedFilterLength = inputLength;
  paddedZeros = paddedFilterLength - filterLength;
  paddedFilterPtr = malloc((2*(numFilters * paddedFilterLength)*sizeof(float)));
  paddedFilterPtrSave = paddedFilterPtr;
  
  /* copy the time filter into the freqFilter buffer and pad. */
  for(cnt = 0; cnt < numFilters; cnt++)
    {
      for(cnt2 = 0; cnt2 < fdFirVars->filterLength; cnt2++)
	{
	  *paddedFilterPtr = *filterPtr;
	  paddedFilterPtr++; filterPtr++;
	  *paddedFilterPtr = *filterPtr;
	  paddedFilterPtr++; filterPtr++;
	}
      for(cnt2 = 0; cnt2 < paddedZeros; cnt2++)
	{
	  *paddedFilterPtr = 0;
	  paddedFilterPtr++;
	  *paddedFilterPtr = 0;
	  paddedFilterPtr++;
	}
    }
  paddedFilterPtr = paddedFilterPtrSave;

  /* assign new length to filterLength. */
  fdFirVars->filterLength = paddedFilterLength;


  /* create twiddle factors for this filter fft. */
  fdFirVars->freqFilterPtr = malloc((2*numFilters*(fdFirVars->filterLength)*sizeof(float)));
  createFftTwiddles(fdFirVars->freqFilterPtr, fdFirVars->filterLength);

  butterflies = fdFirVars->filterLength / RADIX;  /* Radix 4*/
  stride      = butterflies * 2;                /* cplx data */
  phases      = computeNumPhases(fdFirVars->filterLength);


  for(cnt = 0; cnt < numFilters; cnt++)
    {
      fft(cnt, fdFirVars->filterLength, phases,
	  butterflies, stride, paddedFilterPtr,
	  fdFirVars->freqFilterPtr);
    }
  /* The fft results are stored back in paddedFilterPtr.  */

  /* 
     We now have freq domain filters of length <inputLength> in paddedFilterPtr.  
     Remember, the fft's output is base-4 reversed; therefore, a hand base-4 
     reversal is not necessary.  We do need to copy this into fdFirVars however.
  */

  for(cnt = 0; cnt < numFilters; cnt++)
    {
      inPtr  = paddedFilterPtr + (cnt*inputLength*2);
      outPtr = fdFirVars->freqFilterPtr + (cnt*inputLength*2);

      for(cnt2 = 0; cnt2 < fdFirVars->filterLength; cnt2++)
	{
	  outPtr[2*cnt2]     = inPtr[2*cnt2];
	  outPtr[(2*cnt2)+1] = inPtr[2*cnt2+1];
	}
    }


}





  /*
    Here, we create the twiddle factors required by the fft & ifft.  These 
    are depended on the length of the input vector.
         for jj = 0:inputLength-1
            val(jj+1) = (-i*2*pi*jj)/inputLength;
         end
  */
void createTwiddles(float *twiddlePtr, float * twiddleConjPtr, int inputLength)
{

  int index;
  float exponent;
  float * ptrSave = twiddlePtr;
 
  /*  fft twiddle factors */
  for(index = 0; index < inputLength; index++)
    {
      exponent = (2*PI*index)/inputLength; /* w/out the i of course */
      *twiddlePtr = (float)cos((double)exponent);
      twiddlePtr++;
      *twiddlePtr = (float)sin((double)exponent) * -1;
      twiddlePtr++;
    }

  twiddlePtr = ptrSave;
  /*  ifft twiddle factors */
  for(index = 0; index < inputLength; index++)
    {
      /*   real  */
      *twiddleConjPtr = *twiddlePtr;
      twiddleConjPtr++; twiddlePtr++;
      /*   imag  */
      *twiddleConjPtr = -1 * (*twiddlePtr);
      twiddleConjPtr++; twiddlePtr++;
    }


}

void createFftTwiddles(float *twiddlePtr, int inputLength)
{

  int index;
  float exponent;
 
  /*  fft twiddle factors */
  for(index = 0; index < inputLength; index++)
    {
      exponent = (2*PI*index)/inputLength; /* w/out the i of course */
      *twiddlePtr = (float)cos((double)exponent);
      twiddlePtr++;
      *twiddlePtr = (float)sin((double)exponent) * -1;
      twiddlePtr++;
    }

}




/*
  Here, we want to compute the number of phases in the fft and ifft.
  Normally, numPhases = log(base RADIX) N, where N is the length of the input.
  Because the kernel supports only lengths that are powers of four, we are able 
  to do a sequence of shifts to determine the number of phases in the fft and ifft.
*/
int computeNumPhases(int inputLength)
{
  int iL = inputLength;
  int retVal = 0;

  while(iL != 0)
    {
      iL = iL >> 2;
      if(iL != 0)
        { 
          retVal++;
        }
    }
  

  return retVal;
}



/* This function verifies that the input length is a power of 4.*/
int verifyLength(int inputLength)
{
  int iL = inputLength;

  while(1)
    {
      if(iL%4 != 0)
	{
	  return 0;
	}
      else if( iL == 4)
	{
	  return 1;
	}
      else
	{
	  iL = iL / 4;
	}
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
