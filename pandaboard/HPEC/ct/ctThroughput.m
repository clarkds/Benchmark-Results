function Throughput = ctThroughput (DataSetNum)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: ctThroughput.m
%
% HPEC Challenge Benchmark Suite
% Corner Turn Throughput Matlab Function 
%
% function throughput = ctThroughput (DataSetNum)
%
%   Calculate throughput of the kernel of a data set.
%
%   Input:
%     DataSetNum - data set number
%
%   Output:
%     Throughput - the throughput 
%
%   Unit:
%     Throughput is in bytes/second.
%
%  Author: Hector Chan
%          MIT Lincoln Laboratory
%

% Build the file name
filename = ['./data/' num2str(DataSetNum) '-ct-timing.dat'];

% Check for file existence
if ~exist(filename,'file')
  disp(['Error: file ' filename ' does not exist. Timing result file is missing.']);
  disp(['You may want to re-generate and re-run data set ' num2str(DataSetNum)]);
  disp('Exiting');
  return;
end

% Read the run time from file
oldPath = path;
addpath('../matlab');
t = readFile(filename, 'float32');
path(oldPath);

% Calculate the workload
w = ctWorkload(DataSetNum);

% Calculate the throughput = workload / latency
Throughput = w / t;
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
