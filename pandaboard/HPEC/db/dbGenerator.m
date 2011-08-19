%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: dbGenerator.m
%
% HPEC Challenge Benchmark Suite
% Database Dataset Generator Matlab Function 
%
% function dbGenerator(id, M, N, ...
%                      NumDelete, NumInsert, NumSearch, ...
%                      TotalCycles, TotalRecords, dX, dY)
%
%   Generates the needed database datasets.  The generation process
%   uses single precision floating point numbers because otherwise,
%   the C kernel may find/miss incorrect targets on searches
%   because of floating point error.
%
%   id - number representing which dataset this will be; files
%        written will be of the form "<id>-db-param.dat",
%        "<id>-db-inst.dat", and "<id>-db-soln.dat".
% 
%  M            - number of grid rows
%  N            - number of grid columns
%  NumDelete    - number of delete ops/cycle
%  NumInsert    - number of insert ops/cycle
%  NumSearch    - number of search ops/cycle
%  TotalCycles  - number of cycles to run
%  TotalRecords - total number of records in database
%  dX           - grid row search size
%  dY           - grid column search size
%
%  Author: Edmund L. Wong 
%          MIT Lincoln Laboratory
%

function dbGenerator(id, M, N, ...
                     NumDelete, NumInsert, NumSearch, ...
                     TotalCycles, TotalRecords, dX, dY)

% Constants representing flags that need to be represented in the
% instruction list.
END_OF_CYCLE_FLAG = 0.0;
SEARCH_OP_FLAG    = -0.5;
INSERT_OP_FLAG    = -1.0;
DELETE_OP_FLAG    = -1.5;

% Fraction of targets that are initially placed into the database
INIT_TARGET       = 0.9;

% Check arguments
if nargin ~= 10
  error('Incorrect number of parameters.');
end
if isnumeric(id) 
  id = num2str(id);
end

% Check whether files already exist
dbPath = '.';
dataPath = [dbPath filesep 'data'];
if exist([dataPath filesep id '-db-param.dat'], 'file') || ...
      exist([dataPath filesep id '-db-inst.dat'], 'file')
  ret = input('Files for this data set number exist.  Would you like to overwrite them? (y/n) ', 's');
  if ~strcmpi(ret, 'y')
    disp('Exiting');
    return;
  end
end

% Initialize variables
num_PT = floor(TotalRecords * INIT_TARGET);
num_UT = TotalRecords - num_PT;
PT = zeros(num_PT, 2);
UT = zeros(num_UT, 2);
nr_per_square = floor(TotalRecords/(M*N));

% Check whether arguments make sense
if num_UT - (NumInsert-NumDelete)*TotalCycles < 0 || ...
      num_UT - TotalCycles*NumInsert + (TotalCycles-1)*NumDelete < 0
  error(['Insufficient number of unplaced targets to insert.  Try ' ...
         'decreasing the number of inserts, increasing the number ' ...
         'of deletes, or increasing the number of targets.']);
elseif num_PT - (NumDelete-NumInsert)*TotalCycles < 0
  error(['Insufficient number of placed targets to delete.  Try ' ...
         'increasing the number of inserts, decreasing the number ' ...
         'of deletes, or increasing the number of targets.']);
 
end


% Generate the targets
num_UT = 0;
num_PT = 0;
for(i = 1:M)
  for(j = 1:N)
    for(k = 1:nr_per_square)
      if (rand <= (1-INIT_TARGET) || num_PT >= size(PT, 1)) && ...
                  num_UT < size(UT, 1)
        num_UT = num_UT + 1;
        UT(num_UT, :) = [rand(1)+i-1 rand(1)+j-1];
      else
        num_PT = num_PT + 1;
        PT(num_PT, :) = [rand(1)+i-1 rand(1)+j-1];
      end
    end
  end
end
for i = num_PT+num_UT+1:TotalRecords
  if (rand <= (1-INIT_TARGET) || num_PT >= size(PT, 1)) && ...
              num_UT < size(UT, 1)
    num_UT = num_UT + 1;
    UT(num_UT, :) = [rand*M rand*N];
  else
    num_PT = num_PT + 1;
    PT(num_PT, :) = [rand*M rand*N];
  end
end

% Cast lists back to single precision
PT = single(PT);
UT = single(UT);

disp(['Placed ' num2str(num_PT) ' targets, left ' ...
      num2str(num_UT) ' unplaced.']);

% Create search results array
SearchResults = zeros(TotalCycles * NumSearch, 1);

% Create instructions to populate our initial database
RandList = randperm(num_PT);
InstList = zeros(num_PT*3 + 1 + TotalCycles * ...
                 (NumSearch*5 + (NumInsert+NumDelete)*3 + 1), 1);

% InstListIdx:3:3*(num_PT-1)+InstListIdx
InstList(1:3:3*num_PT-2) = INSERT_OP_FLAG;
InstList(2:3:3*num_PT-1) = PT(RandList, 1)';
InstList(3:3:3*num_PT)   = PT(RandList, 2)';
InstList(3*num_PT+1)     = END_OF_CYCLE_FLAG;

% Insert an end of cycle flag
InstListIdx = 3*num_PT+2;

% Create the benchmark commands
for(cycle=1:TotalCycles)
  % SEARCH: SEARCH_OP_FLAG x0 x1 y0 y1
  RandList = rand(NumSearch*2, 1);
  InstList(InstListIdx:5:5*(NumSearch-1)+InstListIdx)     = SEARCH_OP_FLAG;
  InstList(InstListIdx+1:5:5*(NumSearch-1)+InstListIdx+1) = ...
      single(RandList(1:2:end) .* M - dX/2);
  InstList(InstListIdx+2:5:5*(NumSearch-1)+InstListIdx+2) = ...
      single(RandList(2:2:end) .* N - dY/2);
  InstList(InstListIdx+3:5:5*(NumSearch-1)+InstListIdx+3) = ...
      single(RandList(1:2:end) .* M + dX/2);
  InstList(InstListIdx+4:5:5*(NumSearch-1)+InstListIdx+4) = ...
      single(RandList(2:2:end) .* N + dY/2);
  for j=1:NumSearch
    SearchResults((cycle-1)*NumSearch + j) = size(find(PT(:,1) > InstList(InstListIdx+1) & ...
                                                      PT(:,1) < InstList(InstListIdx+3) & ...
                                                      PT(:,2) > InstList(InstListIdx+2) & ...
                                                      PT(:,2) < InstList(InstListIdx+4)), 1);
    InstListIdx = InstListIdx + 5;
  end
  
  % INSERT: INSERT_OP_FLAG x y
  RandList = randperm(size(UT, 1));
  InstList(InstListIdx:3:3*(NumInsert-1)+InstListIdx)     = INSERT_OP_FLAG;
  InstList(InstListIdx+1:3:3*(NumInsert-1)+InstListIdx+1) = ...
      UT(RandList(1:NumInsert), 1);
  InstList(InstListIdx+2:3:3*(NumInsert-1)+InstListIdx+2) = ...
      UT(RandList(1:NumInsert), 2);
  PT = [PT ; UT(RandList(1:NumInsert), :)];
  UT = UT(RandList(NumInsert+1:end), :);
  
  % DELETE: DELETE_OP_FLAG x y
  InstListIdx = InstListIdx + 3*NumInsert;
  RandList = randperm(size(PT, 1));
  InstList(InstListIdx:3:3*(NumDelete-1)+InstListIdx)     = DELETE_OP_FLAG;
  InstList(InstListIdx+1:3:3*(NumDelete-1)+InstListIdx+1) = ...
      PT(RandList(1:NumDelete), 1);
  InstList(InstListIdx+2:3:3*(NumDelete-1)+InstListIdx+2) = ...
      PT(RandList(1:NumDelete), 2);
  UT = [UT ; PT(RandList(1:NumDelete), :)];
  PT = PT(RandList(NumDelete+1:end), :);
  
  % Insert an end of cycle flag
  InstListIdx = InstListIdx + 3*NumDelete;
  InstList(InstListIdx) = END_OF_CYCLE_FLAG;
  InstListIdx = InstListIdx + 1;
end

% Add path to needed matlab functions
oldPath = path;
addpath([dbPath filesep '..' filesep 'matlab']); 

% Write parameters
writeFile([dataPath filesep id '-db-param.dat'], ...
          [0 M N NumDelete NumInsert NumSearch ...
           TotalCycles TotalRecords dX dY], ...
          'int32');
disp(['Wrote database parameters to ' id '-db-param.dat.']);
writeFile([dataPath filesep id '-db-inst.dat'], InstList, 'float32');
disp(['Wrote ' num2str(TotalCycles * (NumInsert + NumSearch + NumDelete)) ...
      ' instruction(s) to ' id '-db-inst.dat.']);

% Write results file
writeFile([dataPath filesep id '-db-soln.dat'], SearchResults, 'int32');
disp(['Wrote search results file to ' id '-db-soln.dat.']);

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
