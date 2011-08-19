Filename:     README.txt

Author:       Jeanette Baran-Gale 
              MIT Lincoln Laboratory

Description:  The following Readme gives a description of the standard 
              C Complex Reduced SVD kernel. 
              Topics include:
	      -Kernel Description
	      -File Descriptions
              -Generating Data (matlab required) & Pregenerated Datasets
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
The reduced singular value decomposition of an MxN matrix A with M>=N is a set 
of matrices, A = USV', where U and V are orthogonal matricies of size [M,N] 
and [N,N] respectively, and S in a diagonal matrix of size [N,N].  For 
simplicity the S matrix is stored as a vector of the singular values. The 
implementation chosen to compute the SVD is based on the Golub-Kahan algorithm, 
operating on complex data.  For more information on this algorithm, see Golub 
and Van Loan's "Matrix Computations," Section 8.3.2



Files:
___________________________________________________________________________
	svd.c           -complex Reduced SVD C code
	svdGenerator.m  -matlab function to generate inputs, and 
			 verification data
        svdThroughput.m -matlab function to obtain the kernel latency
	svdThroughput.m -matlab function to calculate throughput
	svdVerify.c     -svd verification utility
	svdWorkload.m   -matlab function to calculate workload
	Makefile	-Makefile to compile the svd kernel and verification 
                         utility
	README.txt      -This Readme document



Generating Data (matlab required)
___________________________________________________________________________
To generate data sets, within matlab, run the svdGenerator function:
 
	  svdGenerator(DataSetNum, NumRows, NumCols)

This function takes in the following parameters:
    DataSetNum    - Data set number used for file name construction.
    NumRows       - Number of rows in the input matrix A.
    NumCols       - Number of columns in the input matrix A.

This function will generate a random complex input matrix, A, and an array
containing the sinqular values for A.
This matrix will be written out to "./data/<DataSetNum>-svd-inmatrix.dat". 
The singular values will be written out to 
"./data/<DataSetNum>-svd-verification_s.dat".

The input matrix, A, will be used within the "svd" program.  The Vector S will
be used for verification.



Pregenerated Data
___________________________________________________________________________
If matlab is not available, or you do not wish to use matlab, 3 sets of 
pregenerated data have been provided.  They are located in the "data"
directory.  The three datasets contain the following matrix sizes:

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
"svd <DataSetNum>".

For example, if you've used matlab to generate data set 0, to run this 
data set, type "svd 0".  If you would like to use a pregenerated data set, 
this can be done by replacing 0 with the appropriate data set number.
This is also consistant with running the verification tools.  To verify the 
SVD kernel, type "svdVerify <DataSetNum>".

The outputs from the SVD are four data files:
    ./data/#-svd-outmatrix_u.dat  - SVD result matrix, U.
    ./data/#-svd-outmatrix_v.dat  - SVD result matrix, V.
    ./data/#-svd-outvector_s.dat  - SVD result vector, S.
    ./data/#-svd-timing.dat       - SVD execution time in seconds, followed 
				    by the number of iterations that were
			 	    required to run the svd for that dataset. 

Upon running the svdVerify program, you will see one of the following
outputs:
      Tolerance: XX.X
      Max Relative Error in A = XX.X
      Max Error in Orthogonality of U = XX.X
      Max Error in Orthogonality of V = XX.X
      Max Relative Error in Singular Values = XX.X
      SVD Verification: Passed
or
      Tolerance: XX.X
      Max Relative Error in A = XX.X
      Max Error in Orthogonality of U = XX.X
      Max Error in Orthogonality of V = XX.X
      Max Relative Error in Singular Values = XX.X
      SVD Verification: Failed

The svdVerify program relies on output data generated by the svd function, as 
well as the input matrix A, and the array of singular values.  Therefore, if 
a data set is re-generated before the verification is performed, the input file 
will be overwritten.  If svdVerify is re-run without re-running the svd 
program, verification will fail.



Workload and Throughput Calculations (matlab required)
___________________________________________________________________________
The workload function is run similar to the svd kernel. To determine the 
workload for a given data set number, "DataSetNum", in matlab, run the 
function svdWorkload(DataSetNum, opt_num_its).  This function will return the 
workload in flops (floating point operations).  There is an optional parameter 
which is the number of iterations executed.  If this parameter is not present,
the number of iterations is assumed to be the number of columns.

Similarly, the throughput can be determined by providing the dataset number
to the svdThroughput function:  svdThroughput(DataSetNum).  This function 
will return the throughput in flops/s.  This function reads the number of 
iterations executed out of the timing file, and calculates the workload based on
that number.

Both the workload and throughput functions rely on the input data generated
by svdGenerator to get data set parameter information.  Therefore, to get
an accurate throughput calculation, DO NOT overwrite the input data files
for a particular data set run before calculating the throughput.  If a 
data set is overwritten with different parameters, an incorrect throughput
calculation could be performed.



SAMPLE:
___________________________________________________________________________

% make
gcc -xc -ansi -Wall svd.c -o svd -I../include -DVERBOSE -lm
gcc -xc -ansi -Wall svdVerify.c -o svdVerify -I../include -DVERBOSE -lm

% matlab

>> svdGenerator(0,400,200)
Generating SVD input matrix of size 400 x 200.
Input matrix is stored in file ./data/0-svd-inmatrix.dat.
Singular Values are stored in file ./data/0-svd-verification_s.dat.

>> quit

% ./svd 0
done.

% ./svdVerify 0
Verification: PASS

% matlab

>> svdWorkload(0)
ans =
   6.1987e+08

>> svdThroughput(0)
ans =
   4.7181e+08
