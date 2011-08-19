%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: gen_datasets.m
%
% HPEC Challenge Benchmark Suite
% Matlab Script Dataset Generator
%
% Contents:
%  Matlab Script to regenerate all datasets.
%
% Author: Jeanette Baran-Gale
%         MIT Lincoln Laboratory
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


cd cfar;
disp('--- CFAR ---');
cfarGenerator(1, 16,  24,   64,  4,  5, 100, 30);
cfarGenerator(2, 48, 128, 3500,  8, 10, 100, 30);
cfarGenerator(3, 48,  64, 1909,  8, 10, 100, 30);
cfarGenerator(4, 16,  16, 9900, 16, 20, 100, 30);

cd ../ct;
disp('--- CT ---');
ctGenerator(1,  50, 5000);
ctGenerator(2, 750, 5000);

cd ../db; 
disp('--- DB ---');
dbGenerator(1,  5,  5,  20,  20, 400, 100,    500, 2, 2);
dbGenerator(2, 32, 32, 300, 300, 100, 100, 102400, 2, 2);

cd ../fdfir;
disp('--- FDFIR ---');
fdFirGenerator(1, 4096, 128, 64);
fdFirGenerator(2, 1024,  12, 20);

cd ../ga; 
disp('--- GA ---');
genalgGenerator(1,    4,  8,  50,  500,  50,  0.01, 0.6);
genalgGenerator(2,    8, 96, 150, 2000, 200, 0.002, 0.6);
genalgGenerator(3,  100,  5,  50,  500, 100,  0.02, 0.6);
genalgGenerator(4, 1000, 10, 500, 5000, 400,  0.03, 0.3);

cd ../pm; 
disp('--- PM ---');
pmGenerator(1, 0.1, 10, 40, 0.02,  64,  72);
pmGenerator(2, 0.1, 10, 40, 0.02, 128, 256);

cd ../qr; 
disp('--- QR ---');
qrGenerator(1, 500, 100);
qrGenerator(2, 180,  60);
qrGenerator(3, 150, 150);

cd ../svd;
disp('--- SVD ---');
svdGenerator(1, 500, 100);
svdGenerator(2, 180,  60);
svdGenerator(3, 150, 150);

cd ../tdfir;
disp('--- TDFIR ---');
tdFirGenerator(1, 4096, 128, 64);
tdFirGenerator(2, 1024,  12, 20);

cd ../;

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
