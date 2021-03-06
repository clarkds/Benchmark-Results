%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: dbWorkload.m
%
% HPEC Challenge Benchmark Suite
% Database Workload Matlab Function 
%
% function w = dbWorkload(id)
%
%  Calculates the workload given the identifier for a database
%  datafile.  The workload is expressed in number of transactions,
%  a transaction being one database operation (search, insert, or
%  delete).
%
%  Parameter:
%   id - number representing which dataset to use; the file read
%        will be of the same form as generated by DbGenerator,
%        i.e. "<id>-db-param.dat".
%  
% Input file:
%   data/<id>-db-param.dat - the parameters for dataset <id>
%                            (generated by dbGenerator()).
%
%  Author: Edmund L. Wong 
%          MIT Lincoln Laboratory
%

function w = dbWorkload(id)

% Check the identifier
if isnumeric(id)
  id = num2str(id);
end

% Check to make sure the file exists
dbPath = '.';
filename = [dbPath filesep 'data' filesep id '-db-param.dat'];
if ~exist(filename, 'file')
  error(['Parameters for dataset ' id ' do not exist.  Run ' ...
         'dbGenerator() in matlab to generate parameters ' ...
         'for this dataset.']);
end

% Add path to needed matlab functions
oldPath = path;
addpath([dbPath filesep '..' filesep 'matlab']); 

% Read in the datafile and calculate workload
DbParam = readFile(filename, 'int32');
NumDelete    = DbParam(4);
NumInsert    = DbParam(5);
NumSearch    = DbParam(6);
w = NumDelete + NumInsert + NumSearch;

% Restore path
path(oldPath);
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
