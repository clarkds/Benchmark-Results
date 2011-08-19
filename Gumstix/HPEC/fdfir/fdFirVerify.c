/******************************************************************************
** File: fdFirVerify.c
**
** HPEC Challenge Benchmark Suite
** FDFIR Kernel Benchmark
**
** Contents: This file delivers a verification untility to assure the 
**           functionality of the frequency domain fir filter bank
**           implementation.                
**            Inputs: ./data/<dataset>-fdFir-result.dat          
**                    ./data/<dataset>-fdFir-answer.dat          
**
** Author: Matthew A. Alexander
**         MIT Lincoln Laboratory
**
******************************************************************************/


#define EPS .00000011921
#include "fdFir.h"
#include <math.h>
#include <stdio.h>
/*
  Need to include fdFir.h to used the fdFirVars structure.
*/

void fdFirVerify(struct fdFirVariables *fdFirVars);
void fdFirComplete(struct fdFirVariables *fdFirVars);


int main(int argc, char **argv)
{
  /*

  */
  char argPtr[2];
  sprintf(argPtr, "0");
  if (argc == 1)
    {
      printf("No dataset provided\n");
      printf("Usage: fdFirVerify <dataset> \n");
      exit(-1); /*return ;*/
    }
  else
    {
      fdFirVars.dataSet = argv[1];
    }
  
  fdFirVerify(&fdFirVars);
  fdFirComplete(&fdFirVars);

  return 0;
}






void fdFirVerify(struct fdFirVariables *fdFirVars)
{
  int inputLength, resultLength, index, filter, filters;
  int failed = 0;

  float t;
  float * expectedPtr_r;
  float * expectedPtr_i;
  float * kernelResPtr_r;
  float * kernelResPtr_i;
  float tmp, bottom;
  char expectedString[100];
  char kernelResString[100];

  sprintf(  expectedString,"./data/%s-fdFir-answer.dat",fdFirVars->dataSet); 
  sprintf(  kernelResString,"./data/%s-fdFir-output.dat",fdFirVars->dataSet);


  readFromFile(float, expectedString, fdFirVars->filter);
  readFromFile(float, kernelResString, fdFirVars->input);
  resultLength   = fdFirVars->filter.size[1];
  inputLength    = fdFirVars->input.size[1];
  filters        = fdFirVars->filter.size[0];

  t = inputLength * 10 * EPS;  /* compute the tolerance */

  expectedPtr_r  = fdFirVars->filter.data;  
  expectedPtr_i  = expectedPtr_r + 1;
  kernelResPtr_r = fdFirVars->input.data;
  kernelResPtr_i = kernelResPtr_r + 1;
  

  if(resultLength != inputLength)
    {
#ifdef VERBOSE
      printf("Kernel output length does not match correct result length\n");
#endif
      return;
    }

  /*
    Verify that both the real and imaginary values are equal to the
    expected result.  If they're not, print out the index and values, then
    set a flag so that we don't print out success at the end.
  */

  for (filter = 0; filter < filters; filter++)
    {
      for (index = 0; index < inputLength; index++)
	{




	  if((*expectedPtr_r) == 0 && (*expectedPtr_i) == 0)
	    {
	      if(fabs(*kernelResPtr_r) > t)
		{
#ifdef VERBOSE
		  printf("%%%%%%  Error %%%%%% \n");
		  printf("filter %d \n",filter);
		  printf("result differs at index %d (real)\n",index);
		  printf("result:%1.9f ,correctResult:%1.9f, \n",*kernelResPtr_r, *expectedPtr_r);
#endif
		  failed = 1;
		}
	      if(fabs(*kernelResPtr_i) > t)
		{
#ifdef VERBOSE
		  printf("%%%%%%  Error %%%%%% \n");
		  printf("filter %d \n",filter);
		  printf("result differs at index %d (imag)\n",index);
		  printf("result:%1.9f ,correctResult:%1.9f, \n",*kernelResPtr_i, *expectedPtr_i);
#endif
		  failed = 1;
		}

	    }
	  else
	    {
	      bottom = sqrt(     (*expectedPtr_r) * (*expectedPtr_r)   +   (*expectedPtr_i) * (*expectedPtr_i)  );
	      tmp = ((*expectedPtr_r) - (*kernelResPtr_r)) / bottom; 
	      if(fabs(tmp) > t)
		{
#ifdef VERBOSE
		  printf("%%%%%%  Error %%%%%% \n");
		  printf("filter %d \n",filter);
		  printf("result differs at index %d (real)\n",index);
		  printf("result:%1.9f ,correctResult:%1.9f, tmp: %1.9f \n",*kernelResPtr_r, *expectedPtr_r, fabs(tmp));
#endif
		  failed = 1;
		}
	      tmp = ((*expectedPtr_i) - (*kernelResPtr_i)) / bottom; 
	      if(fabs(tmp) > t)
		{
#ifdef VERBOSE
		  printf("%%%%%%  Error %%%%%% \n");
		  printf("filter %d \n",filter);
		  printf("result differs at index %d (imag)\n",index);
		  printf("result:%1.9f ,correctResult:%1.9f, tmp: %1.9f \n",*kernelResPtr_i, *expectedPtr_i, fabs(tmp));
#endif
		  failed = 1;
		}
	    }
	  expectedPtr_r  += 2; expectedPtr_i  += 2; 
	  kernelResPtr_r += 2; kernelResPtr_i += 2;
	}
    }

  printf("Verification: ");
  if(failed==1)
    {
      printf("FAIL \n");
    }
  else
    {
      failed = 0;
      printf("PASS \n");
    }


}



void fdFirComplete(struct fdFirVariables *fdFirVars)
{
  clean_mem(float, fdFirVars->input);
  clean_mem(float, fdFirVars->filter);
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
