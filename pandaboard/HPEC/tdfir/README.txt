Filename:     README.txt
Date:         05/03/2005
Author:       Matthew A. Alexander 
              MIT Lincoln Laboratory                           
Description:  The following Readme gives a description of the standard C 
              Time-domain FIR Filter Bank kernel benchmark. 
              Topics include:
	      -Kernel Description.
	      -File Descriptions.
              -Generating Data (matlab required) & Pregenerated Datasets
	      -Compiling the Application and Verification Tool
	      -Running the Application and Verification Tool
	      -Workload and Throughput Calculations (matlab required)


Disclaimer:
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
The enclosed kernel benchmark performs a time domain implementation of a
finite impulse response (FIR) filter bank.  Its operations include a sequence 
of multiply and accumulates.  Input sample vectors and a bank of filters
are read into the kernel, in return writing out a set of results.  The
length of each result should be equivalent to: sampleSize + filterSize -1.



Files:
___________________________________________________________________________
	tdFir.c           -time-domain FIR bank implementation code
	tdFirGenerator.m  -matlab function to generate inputs, filters,
			   and results 
	tdFir.h		  -time-domain FIR bank implementation header file
        tdFirLatency.m    -matlab function to obtain the kernel latency
	tdFirThroughput.m -matlab function to calculate throughput
	tdFirVerify.c     -time-domain FIR bank implementation verify utility
	tdFirWorkload.m	  -matlab function to calculate workload
	Makefile	  -Makefile to compile the tdFir kernel as well
			   as the verification utility.
	README.txt        -This Readme document



Generating Data (matlab required)
___________________________________________________________________________
To generate random data within matlab, run the tdFirGenerator 
function: 
	tdFirGenerator(dataSet, inputSize, filterSize, numFilters)

This function takes in the following parameters:
    dataSet    - dataSet number
    inputSize  - number of input samples 
    filterSize - filter length
    numFilters - number of filters in filter bank

The function will generate one input, which will be replicated 'numFilters'
times. This will be written to ./data/<dataSet>-tdFir-input.dat.  The function 
will then generate a filter bank of 'numFilters' filters each of length
'filterSize'.  This will be written to ./data/<dataSet>-tdFir-filter.dat.  
Finally, the function will generate a set of results derived from convoluting 
the inputs and filters.  These results will be written to
./data/<dataSet>-tdFir-answer.dat.  This will later be used to compare 
against the result of the c kernel.



Pregenerated Data
___________________________________________________________________________
If matlab is not available, or you do not wish to use matlab, we have 
provided 2 sets of pregenerated sample data, filter banks, and results.
They are all located in the ./data/ directory, and are named 
#-tdFir-type.dat.  The two datasets are formatted as follows:

DataSet #  |   InputLength  |  FilterLength   |  BankSize
-----------------------------------------------------------
    1      |      4096      |       128       |    64
    2      |      1024      |       12        |    20
-----------|----------------|-----------------|------------



Compiling the Application and Verification Tool
___________________________________________________________________________
To compile the application, type:  make
To clean the directory of executables type:  make clean



Running the Application and Verification Tool
___________________________________________________________________________

Once the application has been compiled, it may be run by typing:
tdFir <dataSet>
For example, if you've used matlab to generate dataset 0, to run 
this instance, type:  tdFir 0
If you want to use a pregenerated data set, this can be done by replacing 
<dataSet> with the appropriate data set number.
This is also consistent with running the verification tools.  Again, 
to verify the tdFir kernel performed correctly on matlab generated data,
type:  tdFirVerify 0
This also can be modified by changing the <dataSet> to the appropriate 
data set number.

The outputs from tdFir are two data files:
    -./data/#-tdFir-time.dat   - time in seconds to perform tdFir routine
    -./data/#-tdFir-output.dat - result from tdFir kernel 

Once tdFirVerify has been run, you will see one of the following
outputs:
        Verification: PASS
or
        Verification: FAIL




Workload and Throughput Calculations (matlab required)
___________________________________________________________________________
The workload function is run similar to the tdFir kernel. To 
determine the workload for a given dataset <dataSet>, in matlab, type:
tdFirWorkload <dataSet>.

Similarly, the throughput can be determined by providing the dataset number
to the tdFirThroughput funtion:  tdFirThroughput <dataSet>



SAMPLE:
___________________________________________________________________________

% make clean
rm -f tdFir tdFirVerify

% make
gcc -xc -ansi -lm tdFir.c -o tdFir -I../include
gcc -xc -ansi -lm tdFirVerify.c -o tdFirVerify -I../include

% matlab

>> tdFirGenerator(0,256,16,4)
Generating input of size 256
Input  stored in file ./data/0-tdFir-input.dat
Generating filter of size 16
Filter stored in file ./data/0-tdFir-filter.dat
Generating Result
Result stored in file ./data/0-tdFir-answer.dat

>> exit  

% ./tdFir 0
done.

% ./tdFirVerify 0
Verification: PASS

% matlab

>> tdFirWorkload(0)
ans =
      131072

>> tdFirThroughput(0)
ans =
  6.0964e+08
