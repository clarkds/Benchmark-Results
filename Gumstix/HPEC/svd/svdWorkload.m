%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: svdWorkload.m
%
% HPEC Challenge Benchmark Suite
% SVD Workload Matlab Function 
%
% function workload = svdWorkload(DataSetNum)
%          workload = svdWorkload(DataSetNum, its)
%
% Purpose: Given the number of rows and columns for an input
%          matrix, A, calculate the workload for the complex reduced 
%          SVD C-Kernel.  This returns the number of flop (floating 
%          point operations) for the SVD.  The optional parameter its can
%          be used to calculate the exact workload rather than an approximate.
%
% Inputs:
%   DataSetNum: Index number for SVD data.
%   its       : Actual number of iterations of part of the diagonalize step
%               which ran during execution. If parameter is absent nCols is 
%		used as an approximate.
%
% Returns:
%   The workload in flop.
%
% Author: Jeanette Baran-Gale 
%         MIT Lincoln Laboratory
%
function workload = svdWorkload(DataSetNum,varargin)

  % Read the input matrix to get the number of rows and columns for
  % this data set.  We'll use these numbers to compute the workload.
  input_filename = ['./data/' num2str(DataSetNum) '-svd-inmatrix.dat'];

  % Check for optional arguments.
  NumArgs = nargin -1;


  % Check to make sure that the input file exists.
  if(~exist(input_filename, 'file'))
    error(['The input file does not exist for this data set.  Re-run ' ...
	   'svdGenerator to generate this file.  If the throughput ' ...
	   'is also desired, run the svd program.']);
  end
  oldPath = path;
  addpath('../matlab');
  A = readFile(input_filename, 'float');

  NumRows = size(A, 1);
  NumCols = size(A, 2);

  if(NumArgs == 1)
     n_its = varargin{1};
  else
     n_its = NumCols;
  end;
  
  % Compute the workload in flop.
  if(NumRows > NumCols)
    w_qr = 8 * NumRows * NumCols * NumCols;
  else
    w_qr = 0;
  end

  w_tran_a = 4*(8 * NumCols*NumCols*NumCols/3);
  w_acc_u1 = 4* 4* NumCols * NumCols * NumCols /3;
  w_acc_v1 = 4* 4* NumCols * NumCols * NumCols /3;
  w_bi = w_tran_a + w_acc_u1 + w_acc_v1;
  
  w_mult = 8 * NumRows*NumCols*NumCols;

  w_fixed = w_qr +w_bi + w_mult;


  w_step = 30 * NumCols;
  w_acc_u = 2*6*NumCols*NumCols;
  w_acc_v = 2*6*NumCols*NumCols;
  w_iter = w_step + w_acc_u + w_acc_v;

  workload = w_fixed + n_its * w_iter;
  path(oldPath);

return
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Copyright (c) 2006, Massachusetts Institute of Technology
% All rights reserved.
% 
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are  
% met:
%     * Redistributions of source code must retain the above copyright
%       notice, this list of conditions and the following disclaimer.
%     * Redistributions in binary form must reproduce the above copyright
%       notice, this list of conditions and the following disclaimer in the
%       documentation and/or other materials provided with the distribution.
%     * Neither the name of the Massachusetts Institute of Technology nor  
%       the names of its contributors may be used to endorse or promote 
%       products derived from this software without specific prior written 
%       permission.
%
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
% IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
% LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
% CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
% SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
% INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
% CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
% ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF  
% THE POSSIBILITY OF SUCH DAMAGE.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
