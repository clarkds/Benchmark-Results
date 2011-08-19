%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: genalgGenerator.m
%
% HPEC Challenge Benchmark Suite
% Genetic Algorithm Data Generator Matlab Function 
%
% function genalgGenerator(id, Codes, Genes, MaxElite, MaxGen,
%                          PopSize, PrCrossover, PrMutation)
%
%  Generates the needed genetic algorithm datasets.
%
%  This function works properly ONLY if run within the genalg
%  subdirectory (i.e. the location of this file is matlab's current
%  working directory).
%
%  Parameters:
%   id - identifier representing which dataset this will be; files
%        written will be of the form "<id>-genalg-param.dat",
%        "<id>-genalg-score.dat" and "<id>-genalg-soln.dat". 
%
%   Codes       - number of possible values for each gene.
%   Genes       - number of positions in a chromosome.
%   MaxElite    - termination criteria; terminate the genetic
%                 algorithm if the number of generations exceed
%                 MaxElite without a change in the elite
%                 chromosome.
%   MaxGen      - termination criteria; terminate the genetic
%                 algorithm if the number of generations exceed
%                 MaxGen.
%   PopSize     - number of chromosomes in the population.
%   PrCrossover - probability of crossover for a given pair of chromosomes.
%   PrMutation  - probability of mutation for any particular gene.
%
%  Output files:
%   data/<id>-genalg-param.dat - the parameters used in dataset <id>.
%   data/<id>-genalg-score.dat - the score matrix used to assign a 
%                                score to a code in a particular gene.
%   data/<id>-genalg-soln.dat  - the actual elite chromosome.
%
%  Author: Edmund L. Wong 
%          MIT Lincoln Laboratory
%
function genalgGenerator(id, Codes, Genes, MaxElite, MaxGen, PopSize, ...
                         PrCrossover, PrMutation)

% Check arguments
if nargin ~= 8
  error('Incorrect number of parameters.');
end
if isnumeric(id) 
  id = num2str(id);
end

% Check whether files already exist
genalgPath = '.';
dataPath = [genalgPath filesep 'data'];
if exist([dataPath filesep id '-genalg-param.dat'], 'file') || ...
      exist([dataPath filesep id '-genalg-score.dat'], 'file') || ...
      exist([dataPath filesep id '-genalg-soln.dat'], 'file')
  ret = input(['Files for this data set number exist.  Would you ' ...
               'like to overwrite them? (y/n) '], 's');
  if ~strcmpi(ret, 'y')
    disp('Exiting');
    return;
  end
end

% Add path to needed matlab functions
oldPath = path;
addpath([genalgPath filesep '..' filesep 'matlab']); 

% Initialize random state
rand('state', 1);

% Generate the score matrix that will be used to score a particular
% gene position/code pair.
Scores = 0.5 * rand(Genes, Codes) + 0;

% Now generate a solution in the score matrix by setting a
% particular code in each gene position equal to 1
for gnd = 1:Genes
    pr = floor(Codes*rand+1);
    Scores(gnd, pr) = 1;
    Solution(gnd) = pr;
end

% Write to file
writeFile([dataPath filesep id '-genalg-param.dat'], ...
          [Codes Genes MaxElite MaxGen ...
           PopSize PrCrossover PrMutation], 'float32');
disp(['Wrote GA parameters to ' id '-genalg-param.dat.']);
writeFile([dataPath filesep id '-genalg-score.dat'], Scores, 'float32');
disp(['Wrote GA scorecare to ' id '-genalg-score.dat.']);
writeFile([dataPath filesep id '-genalg-soln.dat'], Solution, 'int32');
disp(['Wrote GA solution to ' id '-solution.dat.']);

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
