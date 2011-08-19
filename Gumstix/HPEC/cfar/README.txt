Filename:     README.txt
Date:         05/10/2005
Author:       Edmund L. Wong 
              MIT Lincoln Laboratory
Description:  The following readme gives a description into the details of
              the C version of the CFAR kernel benchmark.
	      Topics include:
	      -Kernel Description.
	      -File Descriptions.
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
The enclosed constant false-alarm rate (CFAR) kernel finds targets in
an environment of varying background noise.  This environment is
represented by a data cube that contains uniform noise with the
exception of randomly placed targets, which have higher signal power.
The CFAR algorithm loops through this cube, calculating a local noise
estimate consisting of the cells neighboring a particular cell under
test (along the range gates dimension).  If the power of that cell,
normalized to the noise estimate, exceeds a specified threshold, the
cell is considered to contain a target.  Targets that are found in the
cube are outputted to a file, where they are checked against a list of
the actual targets.



Files:
___________________________________________________________________________
	cfar.c            -implementation of CFAR
	cfarVerify.c      -CFAR verification utility
	cfarGenerator.m   -matlab function to generate the necessary
			   inputs for both the kernel and verification
	cfarWorkload.m	  -matlab function to calculate workload
        cfarLatency.m     -matlab function to obtain the kernel latency
	cfarThroughput.m  -matlab function to calculate throughput
	Makefile	  -Makefile to compile the kernel and 
			   verification code
	README.txt	  -This file



Generating Data (matlab required)
___________________________________________________________________________
To generate custom CFAR datasets, run cfarGenerator() in the 
following way within matlab:
	 cfarGenerator(id, Nbm, Ndop, Nrg, G, Ncfar, mu, Ntgts)

This function takes in the following parameters:
    id    - number representing which dataset this will be; files
            written will be of the form "<id>-cfar-param.dat",
            "<id>-cfar-cube.dat" and "<id>-cfar-actual.dat". 
    Nbm   - number of beams (dimension of the data cube)
    Ndop  - number of dopplers (dimension of the data cube)
    Nrg   - number of range gates (dimension of the data cube)
    G     - number of guard cells (cells adjacent to the cell under
            test that are not part of the average background noise 
	    calculation)
    Ncfar - size of one side of the CFAR window
    mu    - background noise threshold; a power level above this is
            considered a target
    Ntgts - number of targets to place in the data cube

Note that Ncfar should be > 0, and Nrg >= 2*(Ncfar + G) + 1 (the size 
of the two windows and guard cells plus cell under test) for the CFAR 
benchmark to work correctly. 

This function will generate three files.  Two of the files are inputs
into the CFAR kernel: a parameter file containing the specified
parameters (<id>-cfar-param.dat) and a data cube containing targets
(<id>-cfar-actual.dat).  The last file is a list of the placed
targets for verification purposes (<id>-cfar-actual.dat).  All these
files are written in the data subdirectory within the CFAR directory. 



Pregenerated Data
___________________________________________________________________________
The CFAR kernel includes the 4 datasets defined in the PCA
Kernel-Level Benchmarks Report.  The datasets have the following
parameters:

Dataset 1 (id = 1):
  Nbm   = 16
  Ndop  = 24
  Nrg   = 64
  G     = 4
  Ncfar = 5
  mu    = 100
  Ntgts = 30
Dataset 2 (id = 2):
  Nbm   = 48
  Ndop  = 128
  Nrg   = 3500
  G     = 8
  Ncfar = 10
  mu    = 100
  Ntgts = 30
Dataset 3 (id = 3):
  Nbm   = 48
  Ndop  = 64
  Nrg   = 1909
  G     = 8
  Ncfar = 10
  mu    = 100
  Ntgts = 30
Dataset 4 (id = 4):
  Nbm   = 16
  Ndop  = 16
  Nrg   = 9900
  G     = 16
  Ncfar = 20
  mu    = 100
  Ntgts = 30



Compiling the Application and Verification Tool
___________________________________________________________________________
To build everything, type:  make
To clean up the directory, type: make clean



Running the Application and Verification Tool
___________________________________________________________________________

To run the kernel, type: ./cfar <id>
To run the verification tool, type: ./cfarVerify <id>
For example, to run sample dataset 1, type: ./cfar 1
To verify dataset 1, type: ./cfarVerify 1

The CFAR kernel will output the following two files: <id>-cfar-timing.dat
which contains the time in seconds to perform the CFAR kernel and
<id>-cfar-detect.dat which contains the targets detected.

Running cfarVerify will check the targets detected vs. the actual
placed targets.  The false-negatives (targets not found by CFAR that
are actual targets) and false-positives (targets found by CFAR that
are not actual targets) are displayed along with the total number of
differences between the detected and actual target lists.  If there 
were no errors, "PASS" is printed to the screen.

Note that the CFAR is relatively sensitive to G, Ncfar, and mu.
Changing these may affect the CFAR's ability to find targets in a
data cube.



Workload and Throughput Calculations (matlab required)
___________________________________________________________________________
The workload and throughput functions are run in a similar fashion as
the CFAR benchmark.  In order to calculate the workload for a
generated dataset in matlab, type: cfarWorkload(<id>).  The workload
is reported in floating point operations (flops).

In order to calculate the throughput, type: cfarThroughput(<id>).  The
throughput is reported in floating point operations per second (flops/sec).



SAMPLE:
___________________________________________________________________________

% make clean
rm -f *.o
rm -f *~
rm -f cfar cfarVerify

% make
gcc -ansi -x c -I../include -Wall     cfar.c   -o cfar
gcc -ansi -x c -I../include -Wall     cfarVerify.c   -o cfarVerify

% matlab

>> cfarGenerator(0, 16, 24, 64, 4, 5, 100, 30)
Placed all 30 Targets
Wrote CFAR parameters to 0-cfar-param.dat.
Wrote CFAR data cube to 0-cfar-cube.dat.
Wrote CFAR placed targets to 0-actual.dat.

>> quit

% ./cfar 0
done.

% ./cfarVerify 0
Verification: PASS

% matlab

>> cfarWorkload(0)
ans =
      172032

>> cfarThroughput(0)
ans =
   6.5913e+08
