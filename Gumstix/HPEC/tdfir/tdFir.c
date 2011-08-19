/******************************************************************************
** File: tdFir.c
**
** HPEC Challenge Benchmark Suite
** TDFIR Kernel Benchmark
**
** Contents: This file provides definitions for various functions in support 
**           of the generic C TDFIR implementation.                           
**            Inputs: ./data/<dataset>-tdFir-input.dat          
**                    ./data/<dataset>-tdFir-filter.dat         
**            Outputs:./data/<dataset>-tdFir-output.dat         
**                    ./data/<dataset>-tdFir-time.dat           
**
** Author: Matthew A. Alexander 
**         MIT Lincoln Laboratory
**
******************************************************************************/

#include "./tdFir.h"
#include <stdlib.h>
#include <stdio.h>
#include "../include/PcaCTimer.h"



int main(int argc, char **argv)
{
  tdFirVars.arguments = argc;
  tdFirVars.dataSet = argv[1];




  /*
    I need to declare some variables:
      -pointers to data, filter, and result
      -inputLength;
    These are all in the tdFirVariables structure in tdFir.h
    Along with the structure definition, I also instantiate an 
    instanced called tdFirVars.  This is the same as doing:
    
    struct tdFirVaribles tdFirVars;
  */
  

  


  /*
    In tdFirSetup, I want to perform tasks that I do NOT want to include
    in my timing.  For example:
      -allocating space for:
         -data
	 -filter
	 -result
    The declarations for this function can be found in tdFir.h, while
    the definitions of these functions can be found in tdFir.c.
  */
  tdFirSetup(&tdFirVars);






  /*
    In tdFir, I want to perform tdFirolution.
    The declaration for tdFir() can be found in tdFir.h, while
    the definition of tdFir() can be found in tdFir.c.
  */
  tdFir(&tdFirVars);







  /*
    In tdFirComplete(), I first want to output my result to output.dat.  
    I then want to do any required clean up.
    The tdFirComplete() declaration can be found in tdFir.h, while
    the definition of tdFirComplete() can be found in tdFir.c.
  */
  tdFirComplete(&tdFirVars);





  return 0;
}


/*
  In tdFirSetup, I want to perform tasks that I do NOT want to include
  in my timing.  For example:
    -allocating space for:
      -data
      -filter
      -result
    -read in inputs and initalize the result space to 0.
  The declarations for this function can be found in tdFir.h, while
  the definitions of these functions can be found in tdFir.c.
*/
void tdFirSetup(struct tdFirVariables *tdFirVars)
{


  int inputLength, filterLength, resultLength;
  char dataSetString[100];
  char filterSetString[100];

  if(tdFirVars->arguments == 1)
    {
      printf("No dataset provided\n");
      printf("Usage: tdFir <dataset> \n");
      exit(-1); /*return ;*/
    }


  sprintf(  dataSetString,"./data/%s-tdFir-input.dat" ,tdFirVars->dataSet);
  sprintf(filterSetString,"./data/%s-tdFir-filter.dat",tdFirVars->dataSet);

  /*
    input read from file 'input.dat', and stored at:   tdFirVars->input.data
    fileter read from file 'filter.dat' and stored at: tdFirVars->filter.data
  */
  readFromFile(float, dataSetString, tdFirVars->input);    
  readFromFile(float, filterSetString, tdFirVars->filter);  

  pca_create_carray_1d(float, tdFirVars->time, 1, PCA_REAL);

  inputLength            = tdFirVars->input.size[1];
  filterLength           = tdFirVars->filter.size[1];
  resultLength           = inputLength + filterLength - 1;
  tdFirVars->numFilters   = tdFirVars->filter.size[0];
  tdFirVars->inputLength  = tdFirVars->input.size[1];
  tdFirVars->filterLength = tdFirVars->filter.size[1];
  tdFirVars->resultLength = resultLength;
  
  pca_create_carray_2d(float, tdFirVars->result, tdFirVars->numFilters, resultLength, PCA_COMPLEX);
  /*
    Make sure that the result starts out as 0.
  */
  zeroData(tdFirVars->result.data, resultLength, tdFirVars->numFilters);

}







void tdFir(struct tdFirVariables *tdFirVars)
{

  /*
    I will need a timer to evaluate my functions performace.  The
    pca_timer_t is located in PcaCTimer.h
  */
  int index;
  int filter;
  float * inputPtr  = tdFirVars->input.data;
  float * filterPtr = tdFirVars->filter.data;
  float * resultPtr = tdFirVars->result.data;
  float * inputPtrSave  = tdFirVars->input.data;
  float * filterPtrSave = tdFirVars->filter.data;
  float * resultPtrSave = tdFirVars->result.data;
  int  filterLength = tdFirVars->filterLength;
  int  inputLength  = tdFirVars->inputLength;  
  int  resultLength = filterLength + inputLength - 1;
  pca_timer_t t;
  t = startTimer();







  for(filter = 0; filter < tdFirVars->numFilters; filter++)
    {

      inputPtr  = inputPtrSave  + (filter * (2*inputLength)); 
      filterPtr = filterPtrSave + (filter * (2*filterLength)); 
      resultPtr = resultPtrSave + (filter * (2*resultLength)); 



      /*
	elCplxMul does an element wise multiply of the current filter element by
	the entire input vector.
	Input Parameters:
	tdFirVars->input.data  - pointer to input
	tdFirVars->filter.data - pointer to filter
	tdFirVars->result.data - pointer to result space
	tdFirVars->inputLength - integer value representing length of input
      */
      
      for(index = 0; index < filterLength; index++)
	{
	  elCplxMul(inputPtr, filterPtr, resultPtr, tdFirVars->inputLength);
	  resultPtr+=2;
	  filterPtr+=2;
	}/* end for filterLength*/
      


    }/* end for each filter */






  /*
    Stop the timer.  Print out the
    total time in Seconds it took to do the TDFIR.
  */

  tdFirVars->time.data[0] = stopTimer(t);

  printf("Done.  Latency: %f s.\n", tdFirVars->time.data[0]);

}





void elCplxMul(float *dataPtr, float *filterPtr, float *resultPtr, int inputLength)
{
  int index;
  float filterReal = *filterPtr; 
  float filterImag = *(filterPtr+1);

  for(index = 0; index < inputLength; index++)
    {
      /*      COMPLEX MULTIPLY   */
      /* real  */
      *resultPtr += (*dataPtr) * filterReal - (*(dataPtr+1)) * filterImag;
      resultPtr++;
      /* imag  */
      *resultPtr += (*dataPtr) * filterImag + (*(dataPtr+1)) * filterReal;
      resultPtr++;
      dataPtr+=2;
    }

}



/*
  In tdFirComplete, I want to do any required clean up.
    -...
*/
void tdFirComplete(struct tdFirVariables *tdFirVars)
{
  char timeString[100];
  char outputString[100];
  sprintf(timeString,"./data/%s-tdFir-time.dat",tdFirVars->dataSet);
  sprintf(outputString,"./data/%s-tdFir-output.dat",tdFirVars->dataSet);
  
  
  writeToFile(float, outputString, tdFirVars->result);
  writeToFile(float, timeString, tdFirVars->time);
  
  clean_mem(float, tdFirVars->input);
  clean_mem(float, tdFirVars->filter);
  clean_mem(float, tdFirVars->result);
  clean_mem(float, tdFirVars->time);
}


void printVector(float * dataPtr, int inputLength)
{

  int index;
  printf("Start of Vector: \n");
  for(index = 0; index < inputLength; index++)
    {
      printf("(%f,%fi) \n",*dataPtr, *(dataPtr+1));
      dataPtr = dataPtr + 2;
    }
  printf("End of Vector. \n");

}



void zeroData(float *dataPtr, int length, int filters)
{
  int index, filter;

  for(filter = 0; filter < filters; filter++)
    {
      for(index = 0; index < length; index++)
	{
	  *dataPtr = 0;
	  dataPtr++;
	  *dataPtr = 0;
	  dataPtr++;
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
