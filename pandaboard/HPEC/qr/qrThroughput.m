%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: qrThroughput.m
%
% HPEC Challenge Benchmark Suite
% QR Throughput Matlab Function 
%
% function throughput = qrThroughput(DataSetNum)
%
% Purpose: This function is to be used after running the QR C-Kernel.
%          The function will read in the file,
%          "./data/<DataSetNum>-qr-timing.dat" to acquire the timing 
%          information from the QR C-Kernel, then calculate the throughput 
%          (in flops/s) based on the latency and workload.
%
% Inputs:
%   DataSetNum: The data set number used to run the QR.
%
% Returns:
%   The throughput in flops/s (float point operations per second).
%
% Author: Ryan Haney
%         MIT Lincoln Laboratory
%
function throughput = qrThroughput(DataSetNum)
  
  % Check to see if the timing file exists.  If not, throw an error.
  timing_filename = ['./data/' num2str(DataSetNum) '-qr-timing.dat'];
  if(~exist(timing_filename, 'file'))
    error(['Timing information file ' timing_filename ' does not ' ...
	   'exist.  Run "qr ' num2str(DataSetNum) '" to create the file.']);
  end
  
  % Add the directory, "../matlab", to the Matlab path to access the
  % readFile function.
  oldPath = path;
  addpath('../matlab');
  
  % Read the timing information for the given data set.
  latency = readFile(timing_filename, 'float');
    
  % Compute the workload for this data set.
  workload = qrWorkload(DataSetNum);
  
  % Calculate the throughput in flops/s.
  throughput = workload/latency;
    
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
