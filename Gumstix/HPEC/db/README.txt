Filename:     README.txt
Date:         05/10/2005
Author:       Edmund L. Wong
	      MIT Lincoln Laboratory
Description:  The following readme gives a description into the details of
              the C version of the database kernel benchmark.
	      Topics include:
	      -Kernel Description
	      -File Descriptions
              -Generating Data (matlab required) & Pregenerated Datasets
	      -Compiling the Application 
	      -Running the Application
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
The context of the database is in a tracking application that stores
track records consisting of an identifier and coordinates.  During any
given cycle, a series of insert, delete, and search operations are
performed on the database by the tracking application.  

This kernel characterizes these operations by simulating them on
a structure typically used in a database.  The main structure used is
a red-black tree which performs the necessary search/insert/delete
operations.  The tree nodes point to database track records that
contain the info that is stored in the database.  This is done by
running and timing a set number of each operation
(insert/delete/search) each cycle.  Timings for each cycle are then
output to a file.  




Files:
___________________________________________________________________________
	db.c                -implementation of the database kernel
	dbVerify.c          -kernel verification utility
	dbmem.h		    -database memory management (relies on 
			     PcaCMemMgr for the actual implementation)
	list.h/list.c       -interface/implementation of a linked list
	rbtree.h/rbtree.c   -interface/implementation of a red-black tree
	dbGenerator.m       -matlab function to generate the necessary
			     inputs for the kernel
	dbWorkload.m        -matlab function to calculate workload
        dbLatency.m         -matlab function to obtain the kernel latency
	dbThroughput.m      -matlab function to calculate throughput
	Makefile	    -Makefile to compile the kernel
	README.txt	    -This file



Generating Data (matlab required)
___________________________________________________________________________
To generate custom datasets, run dbGenerator() in the 
following way within matlab:

         dbGenerator(id, M, N, NumDelete, NumInsert, NumSearch,
		     TotalCycles, TotalRecords, dX, dY)

This function takes in the following parameters:
    id           - number representing which dataset this will be; the file
                   written will be of the form "<id>-db-param.dat".
    M            - number of grid rows
    N            - number of grid columns
    NumDelete    - number of delete ops/cycle
    NumInsert    - number of insert ops/cycle
    NumSearch    - number of search ops/cycle
    TotalCycles  - number of cycles to run
    TotalRecords - total number of records in database
    dX           - grid row search size
    dY           - grid column search size

This function will generate three files.  A parameter file
(<id>-db-param.dat) specifies the parameters for the particular
dataset, and an instruction list (<id>-db-inst.dat) specifies which
instructions to execute.  The last file (<id>-db-soln.dat) stores the
number of results that should be returned by each search operation;
this file is used to verify the database.  These files are written in
the data subdirectory within the database directory.  



Pregenerated Data
___________________________________________________________________________
The database kernel includes the 2 datasets defined in the PCA
Kernel-Level Benchmarks Report.  The datasets have the following
parameters:

Dataset 1 (id = 1):
  M              = 5
  N              = 5
  NumDelete      = 20
  NumInsert      = 20
  NumSearch      = 400
  TotalCycles    = 100
  TotalRecords   = 500
  dX             = 2
  dY             = 2
    
Dataset 2 (id = 2):
  M              = 32
  N              = 32
  NumDelete      = 300
  NumInsert      = 300
  NumSearch      = 100
  TotalCycles    = 100
  TotalRecords   = 102400
  dX             = 2
  dY             = 2



Compiling the Application and Verification Tool
___________________________________________________________________________
To build everything, type:  make
To clean up the directory, type: make clean

The red-black tree, linked list (within the red-black tree), and the
database track records perform a lot of dynamic memory
allocation/deallocation.  Thus, these structures can be set to use
either the standard malloc()/free() or the memory manager which
allocates larger blocks at a time for better efficiency.  This can be
controlled by defining (either in Makefile or a source file) the
symbol PCA_DB_MEM, which turns on the memory manager.  By default,
this is defined in the Makefile under CFLAGS. 



Running the Application and Verification Tool
___________________________________________________________________________

To run the kernel, type:  ./db <id>
To run the verification tool, type: ./dbVerify <id>
For example, to run sample dataset 1, type: ./db 1
To verify dataset 1, type: ./dbVerify 1

The kernel will output <id>-db-timing.dat which contains the time in
seconds to perform each cycle of inserts/deletes/searches and
<id>-db-results.dat which will contain the number of targets found
during each search instruction.  Note that unlike other kernels, the
database outputs a time for EACH cycle, not for all the cycles
together.  Thus, the timing file will contain many times.

Running dbVerify will check the number of targets found for each
search instruction with the number that should be found.  Any
discrepancies are reported, and "PASS" is printed if none were found.



Workload and Throughput Calculations (matlab required)
___________________________________________________________________________
The workload and throughput functions are run in a similar fashion as
the db kernel.  In order to calculate the workload for a generated
dataset in matlab, type: dbWorkload(<id>).  The workload is reported
in transactions. 

In order to calculate the throughput, type: dbThroughput(<id>).  The
throughput is reported in transactions per second.



SAMPLE:
___________________________________________________________________________

% make clean
rm -f list.o rbtree.o db.o ../include/PcaCMemMgr.o *.o
rm -f *~
rm -f db dbVerify

% make
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM   -c -o db.o db.c
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM   -c -o list.o list.c
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM   -c -o rbtree.o rbtree.c
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM   -c -o ../include/PcaCMemMgr.o ../include/PcaCMemMgr.c
gcc   db.o list.o rbtree.o ../include/PcaCMemMgr.o   -o db
gcc -ansi -x c -I../include -Wall -DPCA_DB_MEM    dbVerify.c   -o
dbVerify

% matlab

>> dbGenerator(0, 5, 5, 20, 20, 400, 100, 500, 2, 2)
Wrote database parameters to 0-db-param.dat.
Wrote 44000 instruction(s) to 0-db-inst.dat.
Wrote search results file to 0-db-soln.dat.

>> quit

% ./db 1
done.

% ./dbVerify 1
Verification: PASS

% matlab

>> dbWorkload(1)
ans =
   440

>> dbThroughput(1)
ans =
   9.5879e+04

