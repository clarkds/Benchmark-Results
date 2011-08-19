function w = pmWorkload(DataSetNum)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: pmWorkload.m
%
% HPEC Challenge Benchmark Suite
% Pattern Match Workload Matlab Function 
%
% function w = PmWorkload(DataSetNum)
%
%   Compute workload for a given data set for the Pattern Match kernel
% 
%   Input(s):
%     DataSetNum - data set number
%
%   Output(s):
%     w - the workload value for the given data set.
%
%   Unit:
%     workload is in flop.
%
%  Author: Hector Chan
%          MIT Lincoln Laboratory
%

% Build the file name
filename = ['./data/' num2str(DataSetNum) '-pm-lib.dat'];

% Check for file existence
if ~exist(filename, 'file')
  disp(['Error: file ' filename ' does not exist. Input parameter file is missing.']);
  disp(['You may want to re-generate and re-run data set ' num2str(DataSetNum)]);
  disp('Exiting');
  return;
end

% Read the input parameter for the given data set
oldPath = path;
addpath('../matlab');
lib = readFile(filename, 'float32');
path(oldPath);

patternLength = size(lib,2);
libPatterns = size(lib,1);

SHIFT_RATIO = 3; % Defined on line 48 of PatternMatch.m
MAG_DB = [-5:0.5:5]; % Defined on line 300 of PatternMatch.m

% nShift is the number of 
nShift = round(patternLength / SHIFT_RATIO);
nScale = length(MAG_DB);

shiftLoop = (2*patternLength+1) + (patternLength+1) + (4*patternLength);
shiftLoopTotal = nShift .* shiftLoop;

scaleLoop = (patternLength+1) + 4*patternLength;
scaleLoopTotal = nScale .* scaleLoop;

templateLoop = patternLength+1 + (2+3*patternLength) + ...
    (2*patternLength + 1) + (2*patternLength+1);
w = libPatterns .* (templateLoop + scaleLoopTotal + shiftLoopTotal);
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
