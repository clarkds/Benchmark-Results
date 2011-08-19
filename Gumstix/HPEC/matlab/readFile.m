%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: readFile.m
%
% HPEC Challenge Benchmark Suite
% Common File Read Matlab Function 
%
% function data = readFile(filename, type)
%  Reads the data in from filename as the specified type.
%
%   filename - Name of file to write.
%   data     - Data in the file.
%   type     - Type that data is stored as; can be any of 
%              MATLAB's supported file types, but commonly 'int32'
%              or 'float32'.
%
% Author: Edmund L. Wong 
%         MIT Lincoln Laboratory
%

function data = readFile(filename, type)

% open file and read data
fid = fopen(filename, 'r', 'ieee-le');
if (fid >= 0)
  % read back in the endianness indicator
  endian = fread(fid, 1, 'uint32');
  if (endian == hex2dec('efbeadde'))
    fclose(fid);
    fid = fopen(filename, 'r', 'ieee-be');
    fseek(fid, 4, 'cof');
  elseif (endian ~= hex2dec('deadbeef'))
    fclose(fid);
    error(['Unrecognized header format for ' filename]);
  end
  
  % read in options word
  opts = fread(fid, 1, 'int32');
  
  % read in options
  opt_cplx = bitand(opts, 2);
  
  % read in size information
  nd = fread(fid, 1, 'int32');
  dims = fread(fid, nd, 'int32');
  if nd < 2
    dims(2) = 1;
  end
  dims = [dims(2) dims(1) dims(3:end)];
  
  % read in actual data
  data = fread(fid, type);
  
  % change to complex if needed
  if (opt_cplx) 
    data = complex(data(1:2:length(data)), ...
                   data(2:2:length(data)));
  end
  
  % reshape and organize the matrix to be the correct size and
  % change from row to column major
  if (nd > 0) data = permute(reshape(data, dims), [2 1 3:nd]); end
  
  % done
  fclose(fid);
else 
  error([filename ' does not exist']);
end

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
