/******************************************************************************
** File: genalgVerify.c
**
** HPEC Challenge Benchmark Suite
** Genetic Algorithm Kernel Benchmark
**
** Contents:
**  The verifier for the genetic algorithm kernel benchmark.  Reports the 
**  number of errors in the selected elite chromosome versus the actual
**  solution chromosome (which has a score of 1).  
**
**  Note that the elite chromosome found often strays from the actual
**  elite chromosome.  This is due to the random search behavior of the
**  genetic algorithm as it searches for a solution.  Changing the number
**  of generations (total and elite), mutation/crossover probabilities, or
**  even the initial seed used by the random number generator can result
**  in much different behavior.  Thus, an error reported by the verifier 
**  does not necessarily indicate a problem with the genetic algorithm.
**
** Input files:
**  data/<id>-genalg-soln.dat  - the actual elite chromosome (generated 
**                               by genalgGenerator()).
**  data/<id>-genalg-elite.dat - the elite chromosome found (generated
**                               by genalg).
**
** Author: Edmund L. Wong
**         MIT Lincoln Laboratory
**
******************************************************************************/
#include <PcaCArray.h>

typedef unsigned int uint;

int main(int argc, char** argv)
{
  /* open the solution and elite genes */
  struct PcaCArrayInt elite, solution;
  char *m_elitefile, *m_solnfile;
  uint i;
  uint num_errors = 0;

  /* check arguments */
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [dataset id]\n", argv[0]);
    exit(-1);
  }

#ifdef VERBOSE
  /* print informative message */
  printf("[Verifying dataset %s]\n", argv[1]);
#endif

  /* load up files */
  m_solnfile = (char*) malloc(strlen("data/") + strlen(argv[1]) + 
			      strlen("-genalg-soln.dat") + 1);
  m_elitefile = (char*) malloc(strlen("data/") + strlen(argv[1]) + 
			       strlen("-genalg-elite.dat") + 1);
  sprintf(m_solnfile, "data/%s-genalg-soln.dat", argv[1]);
  sprintf(m_elitefile, "data/%s-genalg-elite.dat", argv[1]);
  readFromFile(int, m_solnfile, solution);
  readFromFile(int, m_elitefile, elite);
  free(m_solnfile);
  free(m_elitefile);

  /* now check solution against the elite chromosome found */
  for (i=0; i < solution.size[1]; i++)
    if (solution.data[i] != elite.data[i]) {
#ifdef VERBOSE
      printf("Error in position %d: gene value should be %d, is %d.\n",
	     i, solution.data[i], elite.data[i]);
#endif
      num_errors++;
    }
#ifdef VERBOSE
  printf("Number of differences = %d\n", num_errors);
#endif
  if(num_errors == 0)
  {
     printf("Verification: PASS\n");
  }
  else
  {
     printf("Verification: FAIL *NOTE-See Documentation*\n");
  }
  
  /* free memory */
  clean_mem(int, elite);
  clean_mem(int, solution);

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
