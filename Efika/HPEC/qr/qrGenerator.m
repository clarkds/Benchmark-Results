%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: qrGenerator.m
%
% HPEC Challenge Benchmark Suite
% QR Dataset Generator Matlab Function 
%
% function qrGenerator(DataSetNum, NumRows, NumCols)
%
% Purpose: Generate input and verification data for the complex QR C-Kernel.
%          This function will generate the following file:
%          "./data/<DataSetNum>-qr-inmatrix.dat", 
%
% Inputs:
%   DataSetNum: Index number for generating input and verification data.
%   NumRows: Number of rows for the input matrix A.
%   NumCols: Number of columns for the input matrix A.
%
% Author: Ryan Haney 
%         MIT Lincoln Laboratory
%
function qrGenerator(DataSetNum, NumRows, NumCols)
  
  % Form input and output verification file names.
  input_file_name = ['./data/' num2str(DataSetNum) '-qr-inmatrix.dat'];

  % Check to see if this data set already exists.  If it does,
  % allow the user to overwrite or exit.
  if(exist(input_file_name, 'file'))
    ret = input(['Files for this data set number exist.  ' ...
		 'Would you like to overwrite them?  (y/n) '], 's');
    if(~strcmpi(ret, 'y'))
      disp('Exiting.');
      return;
    end
  end 
  
  % Check to make sure the number of rows is greater than or equal
  % to the number of columns.
  if(NumCols > NumRows)
    error(['The QR algorithm requires that NumRows >= NumCols.']);
  end
  
  disp(['Generating QR input matrix of size ' num2str(NumRows) ...
	' x ' num2str(NumCols) '.']);
  disp(['Input matrix is stored in file ' input_file_name '.']);

  % Generate a random input matrix A.
  A = rand(NumRows, NumCols) + rand(NumRows, NumCols)*i;

  % Add the directory, "../matlab", to the Matlab path to access the
  % writeFile function.
  oldPath = path;
  addpath('../matlab');
  
  % Write our input and output verification matrices to file.
  writeFile(input_file_name, A, 'float32');

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
