%
% function a = bit_reverse(b)
%   Base 4 reverses the indices of input vector b and stores
%       the result in vector a.
%
%   example:
%
%   a = [0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15];
%   b = bit_reverse(a);
%   a
%     [0 4 8 12 1 5 9 13 2 6 10 14 3 7 11 15]
%
%   author:  Matthew A. Alexander (maa@ll.mit.edu)
%            MIT Lincoln Laboratory                           
%
%   disclaimer:
%   This code is submitted for Government consideration and intended for
%   limited distribution to Government authorized parties only. 


function a = bit_reverse(b)
alength = length(b);
bits = log(alength) / log(4);

for ii = 0:alength - 1
  bin = dec2base(ii,4,bits);
    for mm = 1:bits

	bin2(mm) = bin(bits - (mm-1));

    end
  jj = base2dec(bin2,4);
  a(ii+1) = b(jj+1);
end