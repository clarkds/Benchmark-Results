Filename:     README.txt
Date:         05/05/2005
Author:       Matthew A. Alexander
              MIT Lincoln Laboratory                        
Description:  The following Readme gives a description into the details of
              the standard C frequency domain FIR filter bank kernel. 
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
The enclosed frequency domain finite impulse response (FIR) filter bank kernel
has been developed as a Radix-4 implementation.  Its operations include a
fast fourier transform (fft), an element wise multiply, and an inverse fft
(ifft).  Input sample vectors and a bank of filters are read into the 
kernel, in return writing out a set of results.  The length of each result
should be equivalent to the input.  Only input lengths of powers of four are
valid.

Note that the fft and ifft are not standard implementations. The indices 
of the fft result are in base-4 reversed order.  For optimization purposes,
the fft result is not reordered.  Instead, the filter which is multiplied
by the fft result is also in base-4 reversed order.  The reversed ifft 
input vector is eventually put back in a normal order after performing 
the ifft.

These modules cannot be replaced by normal fft, elMul, and ifft functions.
The following steps must be taken to replace these modules:
1)Both the fft & ifft must be replaced; otherwise, base-4 reversal errors
  will occur.
2)The filter must not be in base-4 reversed order.  This can be fixed by
  removing the bit_reverse call in fdFirGenerator.m



Files:
___________________________________________________________________________
	fdFir.c           -fdFir implementation code
	fdFirGenerator.m  -matlab function to generate inputs, filters,
			   and results 
        fft.c		  -fft implementation for the FIR filter
        ifft.c		  -ifft implementation for the FIR filter
	fdFir.h	          -fdFir implementation header file
        fdFirLatency.m    -matlab function to obtain the kernel latency
	fdFirThroughput.m -matlab function to calculate throughput
	fdFirVerify.c     -fdFir implementation verify utility
	fdFirWorkload.m	  -matlab function to calculate RADIX 2 workload
	Makefile	  -Makefile to compile the fdFir kernel as well
			   as the verification utility.
	README.txt        -This Readme document



Generating Data (matlab required)
___________________________________________________________________________
To generate random data within matlab, run the fdFirGenerator 
function: 
	fdFirGenerator(dataSet, inputSize, filterSize, numFilters)

This function takes in the following parameters:
    dataSet    - dataSet number
    inputSize  - number of input samples 
    filterSize - filter length
    numFilters - number of filters in filter bank

The function will generate one input, which will be replicated 'numFilters'
times. This will be written to ./data/<dataSet>-fdFir-input.dat.  The function 
will then generate a filter bank of 'numFilters' filters each of length
'filterSize'.  This will be written to ./data/<dataSet>-fdFir-filter.dat.  
Finally, the function will generate a set of results derived from filtering 
the inputs using the provided filters.  These results will be written to
./data/<dataSet>-fir-answer.dat.  This will later be used to compare 
against the result of the c kernel.



Pregenerated Data
___________________________________________________________________________
If matlab is not available, or you do not wish to use matlab, we have
provided 2 sets of pregenerated sample data, filter banks, and results.
They are all located in the ./data/ directory, and are named 
#-fdFir-type.dat.  The two datasets are formatted as follows:

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
fdFir <dataSet>.
For example, if you've used matlab to generate dataset 0, to run 
this instance, type:  fdFir 0
If you want to use a pregenerated data set, this can be done by replacing 
<dataSet> with the appropriate data set number.
This is also consistent with running the verification tools.  Again, 
to verify the fdFir kernel performed correctly on matlab generated data,
type:  fdFirVerify 0
This also can be modified by changing the <dataSet> to the appropriate data 
set number.

The outputs from fdFir are two data files:
    -./data/#-fdFir-time.dat   - time in seconds to perform fdFir routine
    -./data/#-fdFir-output.dat - result from fdFir kernel 

Once fdFirVerify has been run, you will see one of the following
outputs:

        Verification: PASS
or
        Verification: FAIL



Workload and Throughput Calculations (matlab required)
___________________________________________________________________________
The workload function is run similar to the fdFir kernel. To determine the 
radix 2 workload for a given dataset <dataSet>, in matlab, type:
fdFirWorkload <dataSet>.  
Similarly, the throughput can be determined by providing the dataset number
to the fdFirThroughput funtion:  fdFirThroughput <dataSet>



SAMPLE:
___________________________________________________________________________

% make clean
rm -f fdFir fdFirVerify

% make
gcc -xc -ansi -lm fft.c ifft.c elWise.c fdFir.c -o fdFir -I../include
gcc -xc -ansi -lm fdFirVerify.c -o fdFirVerify -I../include

% matlab

>> fdFirGenerator(0,256,16,4)
Generating input of size 256
Input  stored in file ./data/0-fdFir-input.dat
Generating 4 filters of size 16
Filter stored in file ./data/0-fdFir-filter.dat
Generating Result
Result stored in file ./data/0-fdFir-answer.dat

>> exit  

% ./fdFir 0
done.

% ./fdFirVerify 0
Verification: PASS

% matlab

>> fdFirWorkload 0
ans =
      90112

>> fdFirThroughput 0
ans =
  2.7985e+08

