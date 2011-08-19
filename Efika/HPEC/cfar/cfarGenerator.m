%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: cfarGenerator.m
%
% HPEC Challenge Benchmark Suite
% CFAR Data Generator Matlab Function 
%
% function cfarGenerator(id, Nbm, Ndop, Nrg, G, Ncfar, mu, Ntgts)
%
%  Generates the needed CFAR datasets.
%
%  This function works properly ONLY if run within the cfar
%  subdirectory (i.e. the location of this file is matlab's current
%  working directory).
%
%  Parameters:
%   id    - number representing which dataset this will be; files
%           written will be of the form "<id>-cfar-param.dat",
%           "<id>-cfar-cube.dat" and "<id>-cfar-actual.dat". 
%   Nbm   - number of beams (dimension of the data cube).
%   Ndop  - number of dopplers (dimension of the data cube).
%   Nrg   - number of range gates (dimension of the data cube).
%   G     - number of guard cells (cells adjacent to the cell under
%           test that are not part of the average background noise 
%           calculation).
%   Ncfar - size of one side of the CFAR window.
%   mu    - background noise threshold; a power level above this is
%           considered a target.
%   Ntgts - number of targets to place in the data cube.
%
%  Output files:
%   data/<id>-cfar-param.dat  - the parameters used in dataset <id>.
%   data/<id>-cfar-cube.dat   - the data cube with placed targets.
%   data/<id>-cfar-actual.dat - the actual placed targets.
%
%
%  Note that Ncfar should be > 0, and Nrg >= 2*(Ncfar + G) + 1
%  (the size of the two windows and guard cells plus cell under
%  test) for the CFAR benchmark to work correctly. 
%
%  Author: Edmund L. Wong 
%          MIT Lincoln Laboratory
%

function cfarGenerator(id, Nbm, Ndop, Nrg, G, Ncfar, mu, Ntgts)

% Target power constant
CFAR_TARGET_POWER = 50;

% Check arguments
if nargin ~= 8
  error('Incorrect number of parameters.');
end
if isnumeric(id) 
  id = num2str(id);
end

% Check Ncfar and Nrg for compliance
if Ncfar <= 0
  error('Ncfar must be > 0; see help for more details.');
end
if Nrg < 2*(Ncfar + G) + 1
  error(['Nrg must be >= 2*(Ncfar + G) + 1; see help for more ' ...
         'details.']);
end

% Check whether files already exist
cfarPath = '.';
dataPath = [cfarPath filesep 'data'];
if exist([dataPath filesep id '-cfar-param.dat'], 'file') || ...
      exist([dataPath filesep id '-cfar-cube.dat'], 'file') || ...
      exist([dataPath filesep id '-cfar-actual.dat'], 'file')
  ret = input(['Files for this data set number exist.  Would you ' ...
               'like to overwrite them? (y/n) '], 's'); 
  if ~strcmpi(ret, 'y')
    disp('Exiting');
    return;
  end
end

% Add path to needed matlab functions
oldPath = path;
addpath([cfarPath filesep '..' filesep 'matlab']); 

% Initialize random state
rand('state', 1);
%TODO
%rand('state',sum(100*clock));

% First, place a background of uniform noise 
% Originally Nbm x Nrg x Ndop
% We want Nrg x Ndop x Nbm
C = 1e-1 * rand(Ndop, Nrg, Nbm);

% Set rand to a known state so that we place the Ntgts targets at the same places
rand('state',20);

% Now let's place the targets with some side bleed (i.e. the signal
% power of the target "leaks" to adjacent cells)
PlacedTargets = ceil(rand(Ntgts, 3) .* ...
                     repmat([0.8*Nbm 0.8*Nrg 0.8*Ndop], Ntgts, 1) + ...
                     repmat([0.1*Nbm 0.1*Nrg 0.1*Ndop], Ntgts, 1));
PlacedTargets = [PlacedTargets(:, 3) ... % Ndop
                 PlacedTargets(:, 2) ... % Nrg
                 PlacedTargets(:, 1)];   % Nbm
PlacedTargets = sortrows(PlacedTargets);
for tgt = 1:Ntgts
    C(PlacedTargets(tgt, 1), PlacedTargets(tgt, 2), PlacedTargets(tgt, 3)) = ...
        CFAR_TARGET_POWER;
end
disp(['Placed all ' num2str(Ntgts) ' Targets']);

% Write to file
writeFile([dataPath filesep id '-cfar-param.dat'], [G Nbm Ncfar Ndop Nrg mu], 'int32');
disp(['Wrote CFAR parameters to ' id '-cfar-param.dat.']);
writeFile([dataPath filesep id '-cfar-cube.dat'], C, 'float32');
disp(['Wrote CFAR data cube to ' id '-cfar-cube.dat.']);
writeFile([dataPath filesep id '-cfar-actual.dat'], PlacedTargets, 'int32');
disp(['Wrote CFAR placed targets to ' id '-actual.dat.']);

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
