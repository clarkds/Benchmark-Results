function ctGenerator(DataSetNum, NumRows, NumCols)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: ctGenerator.m
%
% HPEC Challenge Benchmark Suite
% Corner Turn Dataset Generator Matlab Function 
%
% function ctGenerator(DataSetNum, NumRows, NumCols)
%   
%   Generates a random matrix for the Corner Turn kernel
%
%   Input(s):
%     NumRows - number of rows of the matrix
%     NumCols - number of columns of the matrix
%
%   Output(s):
%     Files: 
%       ./data/<DataSetNum>-ct-inmatrix.dat    - the randomly generated matrix
%       ./data/<DataSetNum>-ct-truthmatrix.dat - the transpose of the randomly 
%                                                generated matrix
%
%   Author: Hector Chan
%           MIT Lincoln Laboratory
%

% build the file names
InFileName    = ['./data/' num2str(DataSetNum) '-ct-inmatrix.dat'];
TruthFileName = ['./data/' num2str(DataSetNum) '-ct-truthmatrix.dat'];

% Check if the data set already exists
if(exist(InFileName, 'file') | exist(TruthFileName, 'file'))
  ret = input('Files for this data set number exist.  Would you like to overwrite them? (y/n) ', 's');
  if ~strcmpi(ret, 'y')
    disp('Exiting');
    return;
  end 
end 

disp(['Generating test matrix of size ' num2str(NumRows) ' x ' num2str(NumCols)]);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

oldPath = path;
addpath('../matlab');
matrix = rand(NumRows, NumCols);
writeFile(InFileName, matrix, 'float32');

truthmatrix = matrix';
writeFile(TruthFileName, truthmatrix, 'float32');
path(oldPath);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

disp(['Test matrix is stored in file ' InFileName]);
disp(['Transposed truth matrix is stored in file ' TruthFileName]);
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
