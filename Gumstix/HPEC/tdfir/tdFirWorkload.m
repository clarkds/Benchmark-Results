function workload = tdFirWorkload(dataSet)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: tdFirWorkload.m
%
% HPEC Challenge Benchmark Suite
% TDFIR Workload Matlab Function 
%
% function: tdFirWorkload(dataSet)
% params:
%	    dataSet    - Data set for which we want to compute
%			  the throughput.
% calculates the workload for the PCA C TDFIR kernel.
%
% Author: Matthew A. Alexander
%         MIT Lincoln Laboratory                            

oldpath = path;
addpath ../matlab;

fileName     = ['./data/' num2str(dataSet) '-tdFir-input.dat'];
if ~exist(fileName, 'file')
  disp([fileName ' does not exist.  Please run tdFirGenerator']);
  path(oldpath);
  return
end
data         = readFile(fileName, 'float32');
inLength     = length(data);
clear data;
clear fileName;
fileName     = ['./data/' num2str(dataSet) '-tdFir-filter.dat'];
if ~exist(fileName, 'file')
  disp([fileName ' does not exist.  Please run tdFirGenerator']);
  path(oldpath);
  return
end
filter       = readFile(fileName, 'float32');
dims         = size(filter);
filters      = dims(1);
filterLength = dims(2);



workload     = filters * (8 * inLength * filterLength);

path(oldpath);
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
