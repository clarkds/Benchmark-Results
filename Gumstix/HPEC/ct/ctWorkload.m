function w = ctWorkload(DataSetNum)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: ctWorkload.m
%
% HPEC Challenge Benchmark Suite
% Corner Turn Workload Matlab Function 
%
% function w = ctWorkload(NumRows, NumCols, NumBytesPerEl)
%
%   Calculate the workload in bytes for a given matrix size
%
%   Input:
%     DataSetNum - data set number
%
%   Output:
%     w - the worload
%
%   Unit:
%     workload is in bytes.
%
%   Note:
%     We assume the number of bytes per element is 4 bytes.
%     If it happens not to be 4 bytes, you can change NumBytesPerEl.
%
%  Author: Hector Chan
%          MIT Lincoln Laboratory
%

NumBytesPerEl = 4;

% Build the file name
filename = ['./data/' num2str(DataSetNum) '-ct-inmatrix.dat'];

% Check for file existence
if ~exist(filename,'file')
  disp(['Error: file ' filename ' does not exist. Input matrix file is missing.']);
  disp(['You may want to re-generate and re-run data set ' num2str(DataSetNum)]);
  disp('Exiting');
  return;
end

% Reading the input paramter
oldPath = path;
addpath('../matlab');
inmatrix = readFile(filename, 'float32');
path(oldPath);

% Calculating workload
w = 2 * prod(size(inmatrix)) * NumBytesPerEl;
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
