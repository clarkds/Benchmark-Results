%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: qrWorkload.m
%
% HPEC Challenge Benchmark Suite
% QR Workload Matlab Function 
%
% function workload = qrWorkload(DataSetNum)
%
% Purpose: Given the number of rows and columns for an input
%          matrix, A, calculate the workload for the complex Fast
%          Givens QR C-Kernel.  This returns the number of flop
%          (floating point operations) for the QR.
%
% Inputs:
%   DataSetNum: Index number for QR data.
%
% Returns:
%   The workload in flop (floating point operations).
%
% Author: Ryan Haney 
%         MIT Lincoln Laboratory
%
function workload = qrWorkload(DataSetNum)

  % Read the input matrix to get the number of rows and columns for
  % this data set.  We'll use these numbers to compute the workload.
  input_filename = ['./data/' num2str(DataSetNum) '-qr-inmatrix.dat'];

  % Check to make sure that the input file exists.
  if(~exist(input_filename, 'file'))
    error(['The input file does not exist for this data set.  Re-run ' ...
	   'qrGenerator to generate this file.']);
  end
  
  % Add the directory, "../matlab", to the Matlab path to access the
  % readFile function.
  oldPath = path;
  addpath('../matlab');

  % Read in the input matrix to get its dimensions.
  A = readFile(input_filename, 'float');

  NumRows = size(A, 1);
  NumCols = size(A, 2);
  
  % Compute the workload in flop.
  workload = 8*(2*(NumRows^2)*NumCols - (NumCols^3)/3);

  % Reset the Matlab path to the user's original path.
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
