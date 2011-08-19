/******************************************************************************
** File: PcaTimer.h
**
** HPEC Challenge Benchmark Suite
** Common Header File
**
** Contents:
**    A timer used for the C versions of the PCA kernel benchmarks.
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#ifndef PCA_TIMER_H
#define PCA_TIMER_H

#include <stdlib.h>
#include <sys/time.h>

typedef struct timeval pca_timer_t;

/**************************************************************************
* Starts the timer, passes back a value that is used with endTimer
*  to determine the time elapsed.
**************************************************************************/
pca_timer_t startTimer() 
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv;
}

/**************************************************************************
* Stops the timer, returns back the time elapsed in seconds.
**************************************************************************/
float stopTimer(pca_timer_t initial)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  tv.tv_sec -= initial.tv_sec;
  tv.tv_usec -= initial.tv_usec;

  if (tv.tv_usec < 0) {
    tv.tv_sec--;
    tv.tv_usec += 1e6;
  }

  return (float) (tv.tv_sec + tv.tv_usec * 1e-6);
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
