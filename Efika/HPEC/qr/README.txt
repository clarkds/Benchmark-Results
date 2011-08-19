Filename:     README.txt
Date:         05/09/2005
Author:       Ryan Haney 
	      MIT Lincoln Laboratory

Description:  The following Readme gives a description into the details of
              the standard C Complex Fast Givens QR Factorization kernel. 
              Topics include:
	      -Kernel Description
	      -File Descriptions
              -Generating Data (matlab required) & Pregenerated Data
	      -Compiling the Application and Verification Tool
	      -Running the Application and Verification Tool
	      -Workload and Throughput Calculations (matlab required)
              -Sample run



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
The QR factorization of an MxN matrix A with M>=N is a pair of matrices,
A = QR, where the unitary matrix Q is of size MxM and the upper-triangular
matrix R is of size MxN.  The implementation chosen to compute the QR is
the Fast Givens method, operating on complex data.  For more information
on the QR algorithm, see Golub and Van Loan's "Matrix Computations," 
sections 5.1.13 and 5.2.5.



Files:
___________________________________________________________________________
	qr.c            - complex Fast Givens QR C code
	qrGenerator.m   - matlab function to generate inputs
        qrLatency.m     - matlab function to obtain the kernel latency
	qrThroughput.m  - matlab function to calculate throughput
	qrVerify.c      - qr verification utility
	qrWorkload.m    - matlab function to calculate workload
	Makefile	- Makefile to compile the qr kernel and verification 
                          utility
	README.txt      - This Readme document



Generating Data (matlab required)
___________________________________________________________________________
To generate data sets, within matlab, run the qrGenerator function:
 
	  qrGenerator(DataSetNum, NumRows, NumCols)

This function takes in the following parameters:
    DataSetNum    - Data set number used for file name construction.
    NumRows       - Number of rows in the input matrix A.
    NumCols       - Number of columns in the input matrix A.

This function will generate a random complex input matrix, A.  This matrix 
will be written out to the file "./data/<DataSetNum>-qr-inmatrix.dat". 

The input matrix, A, will be used within the "qr" program, as well as the 
"qrVerify" program to verify the outputs of the C program.



Pregenerated Data
___________________________________________________________________________
If matlab is not available, or you do not wish to use matlab, 3 data sets 
of pregenerated data have been provided.  They are located in the "data"
directory.  The three data sets contain the following data set sizes:

DataSetNum  |   NumRows  |  NumCols
--------------------------------------
     1      |     500    |    100
     2      |     180    |     60
     3      |     150    |    150



Compiling the Application and Verification Tool
___________________________________________________________________________
To compile the application and verification tool, type:  "make".
To clean the directory of executables type:  "make clean".



Running the Application and Verification Tool
___________________________________________________________________________

Once the application has been compiled, it may be run by typing:
"./qr <DataSetNum>".

For example, if you've used matlab to generate data set 0, to run this 
data set, type "./qr 0".  If you would like to use a pregenerated data set, 
this can be done by replacing 0 with the appropriate data set number.
This is also consistent with running the verification tools.  To verify the 
QR kernel, type "./qrVerify <DataSetNum>".

The outputs from the QR are three data files:
    ./data/#-qr-outmatrix_q.dat  - QR Factorization result matrix, Q.
    ./data/#-qr-outmatrix_r.dat  - QR Factorization result matrix, R.
    ./data/#-qr-timing.dat       - QR execution time in seconds. 

The qrVerify program verifies that the QR Kernel output matrices are calculated
correctly.  This is accomplished by verifying that Q*R equals the input matrix 
A, as well as verifying the orthogonality of Q (Q*Q' equals the identity 
matrix), within a calculated error tolerance.  The error tolerance is 
calculated as a function of the single precision floating point epsilon, and 
the maximum value and size of the input matrix A.  

Upon running the qrVerify program, the user will see one of the following 
outputs:

	Maximum error tolerance: <value>
	Maximum error value from A-Q*R test: <value>
	Maximum error value from I-Q*Q' test: <value>


	QR Verification: Passed

-- OR --

	Maximum error tolerance: <value>

	<Error Information>

	QR Verification: Failed

The qrVerify program relies on verification data generated by the 
qrGenerator function.  Therefore, if a data set is re-generated before
the verification is performed, verification output files will be overwritten.
If qrVerify is re-run without re-running the qr program, verification will
fail.



Workload and Throughput Calculations (matlab required)
___________________________________________________________________________
The workload function is run similar to the qr kernel. To determine the 
workload for a given data set number, "DataSetNum", in matlab, run the 
function qrWorkload(DataSetNum).  This function will return the workload in
flops (floating point operations).

Similarly, the throughput can be determined by providing the data set number
to the qrThroughput function:  qrThroughput(DataSetNum).  This function 
will return the throughput in flops/s.

Both the workload and throughput functions rely on the input data generated
by qrGenerator to get data set parameter information.  Therefore, to get
an accurate throughput calculation, DO NOT overwrite the input data files
for a particular data set run before calculating the throughput.  If a 
data set is overwritten with different parameters, an incorrect throughput
calculation could be performed.



SAMPLE:
___________________________________________________________________________

% matlab

>> qrGenerator(101, 64, 64)
Generating QR input matrix of size 64 x 64.
Input matrix is stored in file ./data/101-qr-inmatrix.dat.

>> exit

% make
gcc -xc -ansi -Wall qr.c -o qr -I../include -lm
gcc -xc -ansi -Wall qrVerify.c -o qrVerify -I../include -lm

% ./qr 101
done.

% ./qrVerify 101
Verification: PASS

% matlab

>> qrWorkload(101)
ans =
   3.4953e+06

>> qrThroughput(101)
ans =
   8.0796e+08
