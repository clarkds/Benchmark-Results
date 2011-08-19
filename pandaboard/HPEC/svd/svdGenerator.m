%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: svdGenerator.m
%
% HPEC Challenge Benchmark Suite
% SVD Dataset Generator Matlab Function 
%
% function svdGenerator(DataSetNum, NumRows, NumCols)
%
% Purpose: Generate input and verification data for the complex SVD C-Kernel.
%
% Inputs:
%   DataSetNum: Index number for generating input and verification data.
%               This number will determine input and verification
%               file names.  For example 0-svd-inmatrix.dat.
%   NumRows: Number of rows for the input matrix A.
%   NumCols: Number of cols for the input matrix A.
%
% Author: Jeanette Baran-Gale
%         MIT Lincoln Laboratory
%
function svdGenerator(DataSetNum, NumRows, NumCols)

  % Form input and output verification file names.
  input_file_name = ['./data/' num2str(DataSetNum) '-svd-inmatrix.dat'];
  sv_file_name = ['./data/' num2str(DataSetNum) '-svd-verification_s.dat'];
  
  if (exist(input_file_name, 'file'))
     ret = input('Files for this data set number exist.  Would you like to overwrite them? (y/n) ', 's');
     if ~strcmpi(ret, 'y')
	disp('Exiting');
	return;
     end
  end

  oldPath = path;
  addpath('../matlab');

  % Check to make sure the number of rows is greater than or equal
  % to the number of columns.
  if(NumCols > NumRows)
    error(['The QR algorithm requires that NumRows >= NumCols.']);
  end

  disp(['Generating SVD input matrix of size ' num2str(NumRows) ...
	' x ' num2str(NumCols) '.']);
  disp(['Input matrix is stored in file ' input_file_name '.']);
  disp(['Singular Values are stored in file ' sv_file_name '.']);

  % Generate a random input matrix A.
  A = rand(NumRows, NumCols) + rand(NumRows, NumCols)*i;
  
  % Use the function svd to calculate singular values for verification
  S = svd(A);

  % Write our input matrix to file.
  writeFile(input_file_name, A, 'float32');
  writeFile(sv_file_name, S, 'float32');
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
