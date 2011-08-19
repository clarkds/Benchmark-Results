Filename:     README.txt
Author:       Edmund L. Wong 
              MIT Lincoln Laboratory
Description:  The following readme gives a description into the details of
              the C version of the genetic algorithm kernel benchmark.
	      Topics include:
	      -Kernel Description
	      -File Descriptions
              -Generating Data (matlab required) & Pregenerated Datasets
	      -Compiling the Application and Verification Tool
	      -Running the Application and Verification Tool
	      -Workload and Throughput Calculations (matlab required)


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


Kernel Description:
___________________________________________________________________________
The genetic algorithm is used to solve an optimization problem.  It
achieves this by building a population of chromosomes each of which
represents a possible solution.  A chromosome consists of genes, each
of which can take on a finite set of values or codes.  Each of these
codes are assigned a fitness score.  Note that the gene position of a
particular code in the chromosome matters.  The total score of the
chromosome is calculated by summing the value of the codes in the
various positions within the chromosome and dividing it by the number
of gene positions; this score represents how "good" that solution is.

For each position, one particular code is assigned a value of 1.0.
Thus the chromosome that has a total score of 1.0 is considered to be
the solution.  The problem the genetic algorithm faces then is finding
this chromosome.

The genetic algorithm attempts to achieve this by performing a series
of steps known as generations.  Starting with an initial population of
chromosomes that contain random values, the genetic algorithm, within
each generation, creates a new population by randomly selecting
chromosomes from the current population with a selection probability
based on the fitness score of each chromosome.  The elite chromosome
in a current population is also maintained in the new population.
These newly selected chromosomes are sometimes altered through
mutation (randomly modifying gene values) and crossovers with other
chromosomes (switching gene values with another chromosome).  The
algorithm runs until a certain number of generations pass without
change in elite chromosome (MaxElite) or a certain number of
generations elapse (MaxGen).  



Files:
___________________________________________________________________________
	genalg.c            -implementation of the genetic algorithm kernel
	genalgVerify.c      -kernel verification utility
	genalgGenerator.m   -matlab function to generate the necessary
			     inputs for both the kernel and verification
	genalgWorkload.m    -matlab function to calculate workload
        genalgLatency.m     -matlab function to obtain the kernel latency
	genalgThroughput.m  -matlab function to calculate throughput
	Makefile	    -Makefile to compile the kernel and 
			     verification code
	README.txt	    -This file



Generating Data (matlab required)
___________________________________________________________________________
To generate custom datasets, run genalgGenerator() in the 
following way within matlab:

         genalgGenerator(id, Codes, Genes, MaxElite, MaxGen,
                         PopSize, PrCrossover, PrMutation)

This function takes in the following parameters:
    id          - number representing which dataset this will be; files
                  written will be of the form "<id>-genalg-param.dat",
		  "<id>-genalg-score.dat" and "<id>-genalg-soln.dat". 

    Codes       - number of possible values for each gene.
    Genes       - number of positions in a chromosome.
    MaxElite    - termination criteria; terminate the genetic algorithm 
                  if the number of generations exceed MaxElite without 
		  a change in the elite chromosome.
    MaxGen      - termination criteria; terminate the genetic algorithm 
		  if the number of generations exceed MaxGen.
    PopSize     - number of chromosomes in the population.
    PrCrossover - probability of crossover for a given pair of chromosomes.
    PrMutation  - probability of mutation for any particular gene.

This function will generate three files.  Two of the files are inputs
into the kernel: a parameter file containing the specified
parameters (<id>-genalg-param.dat) and a scorecard mapping a specific
gene position and code to a score (<id>-genalg-score.dat).  The last
file is the solution chromosome for verification purposes
(<id>-genalg-soln.dat).  The files are written in the data
subdirectory within the genetic algorithm directory.  



Pregenerated Data
___________________________________________________________________________
The genetic algorithm kernel includes the 4 datasets defined in the PCA
Kernel-Level Benchmarks Report.  The datasets have the following
parameters:

Dataset 1 (id = 1):
  Codes       = 4;
  Genes       = 8;
  MaxElite    = 50;
  MaxGen      = 500;
  PopSize     = 50;
  PrCrossover = 0.01;
  PrMutation  = 0.60;
    
Dataset 2 (id = 2):
  Codes       = 8;
  Genes       = 96;
  MaxElite    = 150;
  MaxGen      = 2000;
  PopSize     = 200;
  PrCrossover = 0.002;
  PrMutation  = 0.60;
    
Dataset 3 (id = 3):
  Codes       = 100;
  Genes       = 5;
  MaxElite    = 50;
  MaxGen      = 500;
  PopSize     = 100;
  PrCrossover = 0.02;
  PrMutation  = 0.60;
    
Dataset 4 (id = 4):
  Codes       = 1000;
  Genes       = 10;
  MaxElite    = 500;
  MaxGen      = 5000;
  PopSize     = 400;
  PrCrossover = 0.03;
  PrMutation  = 0.30;



Compiling the Application and Verification Tool
___________________________________________________________________________
To build everything, type:  make
To clean up the directory, type: make clean



Running the Application and Verification Tool
___________________________________________________________________________

To run the kernel, type: ./genalg <id>
To run the verification tool, type: ./genalgVerify <id>
For example, to run sample dataset 1, type: ./genalg 1
To verify dataset 1, type: ./genalgVerify 1

The kernel will output the following two files: <id>-genalg-timing.dat
which contains the time in seconds to perform a generation and
<id>-genalg-elite.dat which contains the elite chromosome found.  Note
that unlike other kernels, the genetic algorithm outputs a time for
EACH generation, not for all the generations together.  Thus, the
timing file may contain many times.

Running genalgVerify will check the elite chromosome found vs. the
actual elite chromosome.  The number of gene errors in the chromosome is
reported.

Note that the elite chromosome found often strays from the actual
elite chromosome.  This is due to the random search behavior of the
genetic algorithm as it searches for a solution.  Changing the number
of generations (total and elite), mutation/crossover probabilities, or
even the initial seed used by the random number generator can result
in much different behavior.  Thus, an error reported by the verifier 
does not necessarily indicate a problem with the genetic algorithm.



Workload and Throughput Calculations (matlab required)
___________________________________________________________________________
The workload and throughput functions are run in a similar fashion.  
In order to calculate the workload for a generated dataset in matlab, type:
genalgWorkload(<id>).  The workload is reported in floating point
operations (flops). 

In order to calculate the throughput, type: genalgThroughput(<id>).  The
throughput is reported in floating point operations per second (flops/sec).



SAMPLE:
___________________________________________________________________________

% make clean
rm -f *.o
rm -f *~
rm -f genalg genalgVerify

% make
gcc -ansi -x c -I../include -Wall    genalg.c   -o genalg
gcc -ansi -x c -I../include -Wall    genalgVerify.c   -o genalgVerify

% matlab

>> genalgGenerator(0, 4, 8, 50, 500, 50, 0.01, 0.60)
Wrote GA parameters to 0-genalg-param.dat.
Wrote GA scorecare to 0-genalg-score.dat.
Wrote GA solution to 0-solution.dat.

>> quit

% ./genalg 0
done.

% ./genalgVerify 0
Verification: FAIL *NOTE-See Documentation*

********
Note: Inputs are based on the random generator, so result may vary.
********

% matlab

>> genalgWorkload(0)
ans =
       11628

>> genalgThroughput(0)
ans =
   3.2474e+08

