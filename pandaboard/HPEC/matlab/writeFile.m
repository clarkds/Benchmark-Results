%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: writeFile.m
%
% HPEC Challenge Benchmark Suite
% Common write File Matlab Function 
%
% function writeFile(filename, data, type)
%   Writes the serialized data as well as a file header to the
%   specified file as the specified type.
%
%   filename - Name of file to write.
%   data     - Data elements to write; should only be a matrix
%              although structs and cells are "technically"
%              supported.
%   type     - Any of MATLAB's supported file types, but commonly
%              'int32' or 'float32'.
%
% Author: Edmund L. Wong 
%         MIT Lincoln Laboratory
%

function writeFile(filename, data, type)

% create the file, write it and remember the handle
fid = fopen(filename, 'w+', 'ieee-le');
if (fid < 0), error(['Could not open file ' filename ' for writing.']); end

% check complex
f_complex = ~isreal(data);

% create options word
opts = bitshift(2.0, 16) + ... % version
       bitshift(double(f_complex), 1) + ... % complex flag
       1; % size flag

% write entire header (including endianness indicator)
fwrite(fid, [hex2dec('deadbeef') opts], 'uint32');
fwrite(fid, setupSize(data), 'uint32');
fwrite(fid, setupData(data), type);

% close file
fclose(fid);

return;


%
% Helper function; sets up sizes as needed.
%
function rv = setupSize(data)

if (isstruct(data)) data = convert2mat(data); end
rv = [length(size(data)) size(data)];
return;



%
% Helper function; takes care of setting up the data for writing.
%
function rv = setupData(data)

% convert the object to a matrix
data = convert2mat(data);

% reorganize the data
rv = reshape(permute(data, [2 1 3:ndims(data)]), 1, []);

% take care of complex data
if ~isreal(rv) rv = interleaveComplex(rv); end

return;


%
% Helper function; interleaves complex numbers as needed.
%
function rv = interleaveComplex(data)

% complex data is real/imag interleaved
rv = [];
for i=1:size(data,1)
  rv = [rv ; real(data(i,:)) ; imag(data(i,:))];
end
return;
  

%
% Helper function; converts object to matrix.
%
function rv = convert2mat(data)

if (isstruct(data)) 
  [i, i] = sort(fieldnames(data));
  data = struct2cell(data);
  data = data(i);
end
if (iscell(data))
  for i=1:length(data)
    if (iscell(data{i})) data{i} = cell2mat(data{i}); end
    data{i} = reshape(data{i}, [], 1);
  end
  data = cell2mat(data); 
end

rv = data;
return;
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
