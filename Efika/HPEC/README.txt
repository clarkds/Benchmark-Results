Filename: README.txt

Author:   Ryan Haney
          MIT Lincoln Laboratory

Description:
          The following README gives a description of the contents of the 
          HPEC Challenge Kernel Benchmarks.

Contents:
          Introduction
          Upper Level Directory Layout
          Kernel Contents
          Sample Data Files
          Testing Platform and Version Requirements
          Generating Data Sets and Running Benchmarks
          Benchmark Results File


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


Introduction:
-------------------------------------------------------------------------------
The HPEC Challenge Kernel Benchmarks consist of ANSI C implementations of nine
kernel benchmarks.  The benchmarks are: time-domain finite impulse 
response filter bank (tdfir), frequency-domain finite impulse response filter
bank (fdfir), QR factorization (qr), singular value decomposition (svd), 
constant false-alarm rate detection (cfar), pattern matching (pm), 
graph optimization via genetic algorithm (ga), real-time database operations 
(db), and memory re-arrangement or cornerturn of a data matrix (ct).  

A detailed description of the kernel benchmarks can be found in 
"KernelDescription.pdf," located in the "docs" directory.  It is recommended 
that the user reads this document as well as the individual README files 
located in each of the kernel directories prior to attempting to run the 
kernels.

Along with the Kernel Description document, an Excel spreadsheet is available
("docs/KernelWorkload.xls") for calculating workloads for the kernels.
Also, each kernel includes a C verification utility and Matlab scripts for 
generating data, computing workloads, and calculating kernel throughput.  
Please see the individual kernel README files for more information on these 
utilities.



Upper Level Directory Layout:
-------------------------------------------------------------------------------
The following section describes the directories found in the current upper 
level directory:

      README.txt:        This upper level README file.
      
      include:           Directory containing C files defining structures and 
                         functions required by the C kernels.

      matlab:            Directory containing Matlab files used by the kernel 
                         Matlab support functions.

      cfar:              Constant false-alarm rate detection implementation 
                         and utilities.

      ct:                Cornerturn kernel implementation and utilities.

      db:                Real-time database implementation and utilities. 

      fdfir:             Frequency-domain FIR filter bank implementation and
                         utilities.

      ga:                Graph optimization via genetic algorithm 
                         implementation and utilities.
		   
      pm:                Pattern matching implementation and utilities.

      qr:                QR factorization implementation and utilities.

      svd:               Singular value decomposition implementation and 
                         utilities.

      tdfir:             Time-domain FIR filter bank implementation and 
                         utilities.

      gen_datasets.m:    Matlab script for generating kernel data sets defined 
                         in "docs/KernelDescription.pdf"
      
      Makefile:          Makefile for building/cleaning all of the kernel
                         benchmarks and verification utilities.

      exec_kernels.csh:  Shell script for running and verifying each of the 
                         kernels on each of the chosen data sets.

      get_results.m:     Matlab script for obtaining throughput and latency
                         results for each kernel on each chosen data set 
                         after running exec_kernels.csh.  Also generates a
                         results file ("hpecKernelResults-<date>.txt").



Kernel Contents:
-------------------------------------------------------------------------------
Each kernel directory contains the following files/directories:

      README.txt:          README file containing kernel information and 
                           instructions on running the kernel and its support 
                           utilities.

      Makefile:            Makefile for building the kernel code.

      kernel.c:            Main C kernel code.

      kernelVerify.c:      Code for verifying the output of the C kernel.

      kernelGenerator.m:   Matlab function for generating kernel data sets.

      kernelWorkload.m:    Matlab function for determining the workload of the
                           kernel for a given data set.

      kernelLatency.m:     Matlab function for reading the latency of the 
                           kernel for a previously run data set.

      kernelThroughput.m:  Matlab function for calculating the throughput
                           of the kernel for a previously run data set.
      
      data:                Directory for storing data sets.

Some kernels may have other files used in support of the kernel and/or Matlab
code.

Each kernel as well as its verification and Matlab utilities relies on 
relative paths to the directory they are contained in.  Therefore, each 
kernel and its utilities must be run from the individual kernel directory.



Generated Data Sets:
-------------------------------------------------------------------------------
Each kernel directory has a "data" directory used for storing generated data 
sets and kernel results.  For information on these data sets, see the 
"KernelDescription.pdf" document or the individual kernel README file.  
User generated data sets and kernel output files will also be put in the 
"data" directory.  The format of the data files, as well as instructions on 
reading or writing data files is described in the "docs/DataFileFormat.txt."



Testing Platform and Version Requirements
-------------------------------------------------------------------------------
The HPEC Challenge Kernel Benchmarks are written using the ANSI C standard
that should be portable across Unix based platforms.  The kernels have been 
tested using gcc 3.3.5 on Linux (RedHat 9 2.4.20) and Solaris (SunOS 5.8).  
The Matlab utilities will run on versions 6.5 and higher.



Generating Data Sets and Running Benchmarks
-------------------------------------------------------------------------------
Each benchmark may be built, run, and verified from each individual kernel 
sub-directory.  Performance results may also be obtained within each 
sub-directory.  See the individual kernel README files for instructions.

It is also possible to perform these tasks on the chosen kernel data sets from
this upper-level directory.  The following sample illustrates this:

% make
make --directory=cfar
make[1]: Entering directory `./KernelBenchmarks/cfar'
gcc -ansi -x c -I../include -Wall   -c -o cfar.o cfar.c
gcc -ansi -x c -I../include -Wall   -c -o ../include/PcaCMemMgr.o ../include/PcaCMemMgr.c
gcc   cfar.o ../include/PcaCMemMgr.o   -o cfar
gcc -ansi -x c -I../include -Wall    cfarVerify.c   -o cfarVerify
make[1]: Leaving directory `./KernelBenchmarks/cfar'
make --directory=tdfir
make[1]: Entering directory `./KernelBenchmarks/tdfir'
gcc -xc -ansi -lm tdFir.c -o tdFir -I../include
gcc -xc -ansi -lm tdFirVerify.c -o tdFirVerify -I../include
make[1]: Leaving directory `./KernelBenchmarks/tdFir'
make --directory=ct
make[1]: Entering directory `./KernelBenchmarks/ct'
gcc -xc -ansi -o ct -I../include ct.c
gcc -xc -ansi -o ctVerify -I../include ctVerify.c
make[1]: Leaving directory `./KernelBenchmarks/ct'
make --directory=db
make[1]: Entering directory `./KernelBenchmarks/db'
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM   -c -o db.o db.c
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM   -c -o list.o list.c
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM   -c -o rbtree.o rbtree.c
gcc   db.o list.o rbtree.o ../include/PcaCMemMgr.o   -o db
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM    dbVerify.c   -o dbVerify
make[1]: Leaving directory `./KernelBenchmarks/db'
make --directory=fdfir
make[1]: Entering directory `./KernelBenchmarks/fdfir'
gcc -xc -ansi -lm fft.c ifft.c elWise.c fdFir.c -o fdFir -I../include
gcc -xc -ansi -lm fdFirVerify.c -o fdFirVerify -I../include
make[1]: Leaving directory `./KernelBenchmarks/fdFir'
make --directory=ga
make[1]: Entering directory `./KernelBenchmarks/ga'
gcc -ansi -x c -I../include -Wall    genalg.c   -o genalg
gcc -ansi -x c -I../include -Wall    genalgVerify.c   -o genalgVerify
make[1]: Leaving directory `./KernelBenchmarks/ga'
make --directory=pm
make[1]: Entering directory `./KernelBenchmarks/pm'
gcc -xc -ansi -o pm -I../include -lm pm.c
gcc -xc -ansi -o pmVerify -I../include pmVerify.c
make[1]: Leaving directory `./KernelBenchmarks/pm'
make --directory=qr
make[1]: Entering directory `./KernelBenchmarks/qr'
gcc -xc -ansi -Wall qr.c -o qr -I../include -lm
gcc -xc -ansi -Wall qrVerify.c -o qrVerify -I../include -lm
make[1]: Leaving directory `./KernelBenchmarks/qr'
make --directory=svd
make[1]: Entering directory `./KernelBenchmarks/svd'
gcc -xc -ansi -Wall  svd.c -o svd -I../include  -lm -lc 
gcc -xc -ansi -Wall  svdVerify.c -o svdVerify -I../include  -lm -lc
make[1]: Leaving directory `./KernelBenchmarks/svd'


% matlab

>> gen_datasets  
--- CFAR ---
Placed all 30 Targets
Wrote CFAR parameters to 1-cfar-param.dat.
Wrote CFAR data cube to 1-cfar-cube.dat.
Wrote CFAR placed targets to 1-actual.dat.
Placed all 30 Targets
Wrote CFAR parameters to 2-cfar-param.dat.
Wrote CFAR data cube to 2-cfar-cube.dat.
Wrote CFAR placed targets to 2-actual.dat.
Placed all 30 Targets
Wrote CFAR parameters to 3-cfar-param.dat.
Wrote CFAR data cube to 3-cfar-cube.dat.
Wrote CFAR placed targets to 3-actual.dat.
Placed all 30 Targets
Wrote CFAR parameters to 4-cfar-param.dat.
Wrote CFAR data cube to 4-cfar-cube.dat.
Wrote CFAR placed targets to 4-actual.dat.
--- CT ---
Generating test matrix of size 50 x 5000
Test matrix is stored in file ./data/1-ct-inmatrix.dat
Transposed truth matrix is stored in file ./data/1-ct-truthmatrix.dat
Generating test matrix of size 750 x 5000
Test matrix is stored in file ./data/2-ct-inmatrix.dat
Transposed truth matrix is stored in file ./data/2-ct-truthmatrix.dat
--- DB ---
Placed 450 targets, left 50 unplaced.
Wrote database parameters to 1-db-param.dat.
Wrote 44000 instruction(s) to 1-db-inst.dat.
Wrote search results file to 1-db-soln.dat.
Placed 92160 targets, left 10240 unplaced.
Wrote database parameters to 2-db-param.dat.
Wrote 70000 instruction(s) to 2-db-inst.dat.
Wrote search results file to 2-db-soln.dat.
--- FDFIR ---
Generating input of size 4096
Input  stored in file ./data/1-fdFir-input.dat
Generating 64 filters
   of size 128
Filters stored in file ./data/1-fdFir-filter.dat
Generating Result
Result stored in file ./data/1-fdFir-answer.dat
Generating input of size 1024
Input  stored in file ./data/2-fdFir-input.dat
Generating 20 filters
   of size 12
Filters stored in file ./data/2-fdFir-filter.dat
Generating Result
Result stored in file ./data/2-fdFir-answer.dat
--- GA ---
Wrote GA parameters to 1-genalg-param.dat.
Wrote GA scorecare to 1-genalg-score.dat.
Wrote GA solution to 1-solution.dat.
Wrote GA parameters to 2-genalg-param.dat.
Wrote GA scorecare to 2-genalg-score.dat.
Wrote GA solution to 2-solution.dat.
Wrote GA parameters to 3-genalg-param.dat.
Wrote GA scorecare to 3-genalg-score.dat.
Wrote GA solution to 3-solution.dat.
Wrote GA parameters to 4-genalg-param.dat.
Wrote GA scorecare to 4-genalg-score.dat.
Wrote GA solution to 4-solution.dat.
--- PM ---
Pattern Length = 64, Number of Templates = 72
Generating the template library
Distorting pattern number 40 from lib as test pattern
lib data is stored in ./data/1-pm-lib.dat
test pattern data is stored in ./data/1-pm-pattern.dat
original index data is stored in ./data/1-pm-origpatnum.dat
Pattern Length = 128, Number of Templates = 256
Generating the template library
Distorting pattern number 25 from lib as test pattern
lib data is stored in ./data/2-pm-lib.dat
test pattern data is stored in ./data/2-pm-pattern.dat
original index data is stored in ./data/2-pm-origpatnum.dat
--- QR ---
Generating QR input matrix of size 500 x 100.
Input matrix is stored in file ./data/1-qr-inmatrix.dat.
Generating QR input matrix of size 180 x 60.
Input matrix is stored in file ./data/2-qr-inmatrix.dat.
Generating QR input matrix of size 150 x 150.
Input matrix is stored in file ./data/3-qr-inmatrix.dat.
--- SVD ---
Generating SVD input matrix of size 500 x 100.
Input matrix is stored in file ./data/1-svd-inmatrix.dat.
Singular Values are stored in file ./data/1-svd-verification_s.dat.
Generating SVD input matrix of size 180 x 60.
Input matrix is stored in file ./data/2-svd-inmatrix.dat.
Singular Values are stored in file ./data/2-svd-verification_s.dat.
Generating SVD input matrix of size 150 x 150.
Input matrix is stored in file ./data/3-svd-inmatrix.dat.
Singular Values are stored in file ./data/3-svd-verification_s.dat.
--- TDFIR ---
Generating input of size 4096
Input  stored in file ./data/1-tdFir-input.dat
Generating filter of size 128
Filter stored in file ./data/1-tdFir-filter.dat
Generating Result
Result stored in file ./data/1-tdFir-answer.dat
Generating input of size 1024
Input  stored in file ./data/2-tdFir-input.dat
Generating filter of size 12
Filter stored in file ./data/2-tdFir-filter.dat
Generating Result
Result stored in file ./data/2-tdFir-answer.dat

>> exit


% exec_kernels.csh 
cfar   1
Done.  Latency: 0.000429 s.
Verification: PASS
cfar   2
Done.  Latency: 0.364490 s.
Verification: PASS
cfar   3
Done.  Latency: 0.099616 s.
Verification: PASS
cfar   4
Done.  Latency: 0.044181 s.
Verification: PASS
ct   1
Done.  Latency: 0.030004 s.
Verification: PASS
ct   2
Done.  Latency: 0.459776 s.
Verification: PASS
db   1
Done.  Latency: 0.005235 s.
Verification: PASS
db   2
Done.  Latency: 0.219676 s.
Verification: PASS
fdFir   1
Done.  Latency: 0.131758 s.
Verification: PASS 
fdFir   2
Done.  Latency: 0.008743 s.
Verification: PASS 
genalg   1
Done.  Latency: 0.000064 s.
Verification: PASS
genalg   2
Done.  Latency: 0.002310 s.
Verification: PASS
genalg   3
Done.  Latency: 0.000120 s.
Verification: PASS
genalg   4
Done.  Latency: 0.001090 s.
Verification: PASS
pm   1
Done.  Latency: 0.008020 s.
Verification: PASS
pm   2
Done.  Latency: 0.064046 s.
Verification: PASS
qr   1
Done.  Latency: 0.619421 s.
Verification: PASS
qr   2
Done.  Latency: 0.046979 s.
Verification: PASS
qr   3
Done.  Latency: 0.068198 s.
Verification: PASS
svd   1
Done.  Latency: 0.277958 s.
Verification: PASS
svd   2
Done.  Latency: 0.043723 s.
Verification: PASS
svd   3
Done.  Latency: 0.490242 s.
Verification: PASS
tdFir   1
Done.  Latency: 0.420219 s.
Verification: PASS 
tdFir   2
Done.  Latency: 0.003091 s.
Verification: PASS 


% matlab

>> get_results
TDFIR
Dataset 1: 0.42022 s, 638.7989 Mflops/s
Dataset 2: 0.003091 s, 636.066 Mflops/s
FDFIR
Dataset 1: 0.13176 s, 254.6671 Mflops/s
Dataset 2: 0.008743 s, 252.9841 Mflops/s
QR
Dataset 1: 0.61942 s, 641.4592 Mflops/s
Dataset 2: 0.046979 s, 649.8223 Mflops/s
Dataset 3: 0.068198 s, 659.8434 Mflops/s
SVD
Dataset 1: 0.27796 s, 477.3395 Mflops/s
Dataset 2: 0.043723 s, 511.9685 Mflops/s
Dataset 3: 0.49024 s, 445.8308 Mflops/s
CFAR
Dataset 1: 0.000429 s, 401.007 Mflops/s
Dataset 2: 0.36449 s, 412.9825 Mflops/s
Dataset 3: 0.099616 s, 412.0938 Mflops/s
Dataset 4: 0.044181 s, 401.5482 Mflops/s
PM
Dataset 1: 0.00802 s, 149.997 Mflops/s
Dataset 2: 0.064046 s, 212.2634 Mflops/s
GA
Dataset 1: 6.3649e-05 s, 182.6891 Mflops/s
Dataset 2: 0.0023099 s, 218.2658 Mflops/s
Dataset 3: 0.00012 s, 128.9833 Mflops/s
Dataset 4: 0.0010904 s, 104.5297 Mflops/s
CT
Dataset 1: 0.030004 s, 66.6578 MBytes/s
Dataset 2: 0.45978 s, 65.2492 MBytes/s
DB
Dataset 1: 0.0052352 s, 84046.9365 Transactions/s
Dataset 2: 0.21968 s, 3186.5137 Transactions/s



Benchmark Results File
-------------------------------------------------------------------------------
The Matlab function get_results.m generates a results file called 
"hpecKernelResults-<date>.txt".  This file is formatted as follows:

<date and time (yyyymmddTHHMMSS)>
* Kernel1
<Dataset #>, <Dataset latency (s)>
<Dataset #>, <Dataset latency (s)>
  .
  .
  .
* SVD
<Dataset #>, <Dataset latency (s)>, <Dataset throughput (flop/s)>
<Dataset #>, <Dataset latency (s)>, <Dataset throughput (flop/s)>
<Dataset #>, <Dataset latency (s)>, <Dataset throughput (flop/s)>
  .
  .
  .
* KernelN
<Dataset #>, <Dataset latency (s)>
  .
  .
  .


Note that the SVD, unlike the other kernels, requires a latency and throughput
result.  This is because the exact number of iterations required to produce the
SVD is data dependent.  See documentation in KernelDescription.pdf for more
details.
