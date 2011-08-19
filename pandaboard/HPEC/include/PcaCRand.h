/******************************************************************************
** File: PcaCRand.h
**
** HPEC Challenge Benchmark Suite
** Common Header File
**
** Contents:
**  The random number generator used by the PCA C kernels.  The generator 
**  used is the VSIPL random number generator.  For details about its
**  inner workings, please refer to the VSIPL Documentation.
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#ifndef _PCACRAND_H_
#define _PCACRAND_H_

/**************************************************************************
* Structure used to hold the state of the random number generator.
**************************************************************************/
struct
{
  unsigned int r_c1;
  unsigned int r_X;
  unsigned int r_X1;
  unsigned int r_X2;
  unsigned int r_a;
  unsigned int r_c;
  unsigned int r_a1;
} vrstate;


/**************************************************************************
* Initializes the random number generator.
**************************************************************************/
void hpec_srand(int seed) 
{ 
  vrstate.r_X = seed;
  vrstate.r_X1 = 1;
  vrstate.r_X2 = 1;
  vrstate.r_a  = 1664525;
  vrstate.r_c  = 1013904223;
  vrstate.r_a1 = 69069;
  vrstate.r_c1 = 3;
}


/**************************************************************************
* Generates a random number in the open interval (0.0, 1.0).
**************************************************************************/
float hpec_rand()
{ 
  unsigned int itmp;
  vrstate.r_X = vrstate.r_X * vrstate.r_a + vrstate.r_c;
  vrstate.r_X1 = vrstate.r_X1 * vrstate.r_a1 + vrstate.r_c1;
  itmp = ((vrstate.r_X - vrstate.r_X1) >> 8) | 0x00000001;
  if (vrstate.r_X1 == vrstate.r_X2) {
    vrstate.r_X1++;
    vrstate.r_X2++;
  }
  return (float) itmp / (float) 16777216.0;
}


#endif

/* ----------------------------------------------------------------------------
Copyright (C) Hughes Research Laboratories, Inc., 1997

A non-exclusive, non-royalty bearing license is hereby granted to all
persons to copy, modify, distribute and produce derivative works for
any purpose, provided that this copyright notice and following disclaimer
appear on all copies.

THIS LICENSE INCLUDES  NO WARRANTEES, EXPRESSED OR IMPLIED, WHETHER
ORAL OR WRITTEN, WITH RESPECT TO  THE SOFTWARE OR OTHER MATERIAL,
INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OF MERCHANT-
ABILITY, OR FITNESS FOR A PARTICULAR PURPOSE, OR ARISING FROM A COURSE
OF PERFORMANCE, OR DEALING, OR FROM USAGE OR TRADE, OR OF NON-
INFRINGEMENT OF ANY PATENTS OF THIRD PARTIES.

This material is based upon work supported by Ft. Huachuca/DARPA under
Contract No. DABT63-96-C-0060.  Any opinions, findings and conclusions
or recommendations expressed in this material are those of the author(s)
and do not necessarily reflect the views of Ft. Huachuca/DARPA.

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
