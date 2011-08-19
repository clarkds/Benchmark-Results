%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: fdFirGenerator.m
%
% HPEC Challenge Benchmark Suite
% FDFIR Dataset Generator Matlab Function 
%
% function fdFirGenerator(dataSet, inputSize, filterSize, numFilters)
%   Generates input data and filters for the generic C kernel
%   Frequency-domain FIR Filter Bank.
%
%   dataSet    - dataSet index to be used to name file
%   inputSize  - number of input samples (must be a power of 4).
%   filterSize - filter length
%   numFilters - number of filters in filter bank
%
%   outputs: 1) ./data/<dataSet>-fdFir-input.dat
%            2) ./data/<dataSet>-fdFir-filter.dat
%            3) ./data/<dataSet>-fdFir-answer.dat
%
%   author:  Matthew A. Alexander
%            MIT Lincoln Laboratory                            
%


function fdFirGenerator(dataSet, inputSize, filterSize, numFilters)

oldpath = path;
addpath ../matlab;


inputFileName  = ['./data/' num2str(dataSet) '-fdFir-input.dat'];
filterFileName = ['./data/' num2str(dataSet) '-fdFir-filter.dat'];
resultFileName = ['./data/' num2str(dataSet) '-fdFir-answer.dat'];

if(exist(inputFileName))
  ret = input('Files for this data set number exist.  Would you like to overwrite them?  (y/n) ', 's');
  if ~strcmpi(ret, 'y') 
    disp('Exiting');
    path(oldpath);
    return;
  end
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                Generate Input                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp(['Generating input of size ' num2str(inputSize)]); %
disp(['Input  stored in file ' inputFileName]);         %
INPUT = zeros(numFilters,filterSize);                   %
for xx = 1:numFilters                                   %
  for yy = 1:inputSize                                  %
    INPUT(xx,yy) = rand + i*rand;                       %
  end                                                   %
end                                                     %
writeFile(inputFileName, INPUT, 'float32');             %
                                                        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                Generate Filter                           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp(['Generating ' num2str(numFilters) ' filters']);      %
disp(['   of size ' num2str(filterSize)]);                 %
disp(['Filters stored in file ' filterFileName]);          %
for yy = 1:numFilters					   %
  for xx=1:filterSize                                      %
    filter(xx)=rand + i*rand;                              %
  end                                                      %
  filters(yy,:) = filter;                                  %
end                                                        %
writeFile(filterFileName, filters, 'float32');             %
                                                           %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%             Generate Correct Result                              %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
disp('Generating Result');                                         %
disp(['Result stored in file ' resultFileName]);                   %
%%%%%% perform fdFir filters %%%%%%                                %
paddedFilters = filters;                                           %
paddedFilters(:,(filterSize+1):inputSize) = 0;                     %
for yy=1:numFilters                                                %
  paddedFilters(yy,:) = fft(paddedFilters(yy,:));                  %
end                                                                %
                                                                   %
for yy=1:numFilters                                                %
  result(yy,:) = ifft( paddedFilters(yy,:) .* fft(INPUT(yy,:)) );  %
end                                                                %
writeFile(resultFileName, result, 'float32');                      %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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
