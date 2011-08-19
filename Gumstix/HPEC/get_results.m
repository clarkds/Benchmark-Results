%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: get_results.m
%
% HPEC Challenge Benchmark Suite
% Matlab Script for Results
%
% Contents:
%  Matlab Script to load kernel latencies and calculate all throughputs.
%
% Usage:
%  Typing ./get_results will calculate all latencies and througputs.  
%  At the end the results will be stored in arrays called t_-kernel_name-(:).
%  Also, a results file ("hpecKernelResults-<date>.txt") will be created.
%
% Author: Jeanette Baran-Gale
%         MIT Lincoln Laboratory
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

cd cfar;
l_cfar(1) = cfarLatency(1);
l_cfar(2) = cfarLatency(2);
l_cfar(3) = cfarLatency(3);
l_cfar(4) = cfarLatency(4);
t_cfar(1) = cfarThroughput(1);
t_cfar(2) = cfarThroughput(2);
t_cfar(3) = cfarThroughput(3);
t_cfar(4) = cfarThroughput(4);

cd ../ct;
l_ct(1) = ctLatency(1);
l_ct(2) = ctLatency(2);
t_ct(1) = ctThroughput(1);
t_ct(2) = ctThroughput(2);

cd ../db; 
l_db(1) = dbLatency(1);
l_db(2) = dbLatency(2);
t_db(1) = dbThroughput(1);
t_db(2) = dbThroughput(2);

cd ../fdfir; 
l_fdFir(1) = fdFirLatency(1);
l_fdFir(2) = fdFirLatency(2);
t_fdFir(1) = fdFirThroughput(1);
t_fdFir(2) = fdFirThroughput(2);

cd ../ga; 
l_genalg(1) = genalgLatency(1);
l_genalg(2) = genalgLatency(2);
l_genalg(3) = genalgLatency(3);
l_genalg(4) = genalgLatency(4);
t_genalg(1) = genalgThroughput(1);
t_genalg(2) = genalgThroughput(2);
t_genalg(3) = genalgThroughput(3);
t_genalg(4) = genalgThroughput(4);

cd ../pm; 
l_pm(1) = pmLatency(1);
l_pm(2) = pmLatency(2);
t_pm(1) = pmThroughput(1);
t_pm(2) = pmThroughput(2);

cd ../qr; 
l_qr(1) = qrLatency(1);
l_qr(2) = qrLatency(2);
l_qr(3) = qrLatency(3);
t_qr(1) = qrThroughput(1);
t_qr(2) = qrThroughput(2);
t_qr(3) = qrThroughput(3);

cd ../svd;
l_svd(1) = svdLatency(1);
l_svd(2) = svdLatency(2);
l_svd(3) = svdLatency(3);
t_svd(1) = svdThroughput(1);
t_svd(2) = svdThroughput(2);
t_svd(3) = svdThroughput(3);

cd ../tdfir;
l_tdFir(1) = tdFirLatency(1);
l_tdFir(2) = tdFirLatency(2);
t_tdFir(1) = tdFirThroughput(1);
t_tdFir(2) = tdFirThroughput(2);

cd ../;

dset   = [ ...
          t_tdFir(1)/1e6  t_tdFir(2)/1e6  0               0 ; ...
	  t_fdFir(1)/1e6  t_fdFir(2)/1e6  0               0 ;...
          t_qr(1)/1e6     t_qr(2)/1e6     t_qr(3)/1e6     0;...
          t_svd(1)/1e6    t_svd(2)/1e6    t_svd(3)/1e6    0;...
          t_cfar(1)/1e6   t_cfar(2)/1e6   t_cfar(3)/1e6   t_cfar(4)/1e6;...
	  t_pm(1)/1e6     t_pm(2)/1e6     0               0;...	 
    	  t_genalg(1)/1e6 t_genalg(2)/1e6 t_genalg(3)/1e6 t_genalg(4)/1e6;...
	  ];

bar(1:7, dset);
title('Results for Floating-point Kernel Benchmarks');
ylabel('Throughput (Mflop/s)');
set(gca, 'XTickLabel', char('TDFIR', 'FDFIR', 'QR', ...
			    'SVD', 'CFAR', 'PM', 'GA'));
xlabel('Kernel');
legend('Dataset 1', 'Dataset 2', 'Dataset 3', 'Dataset 4');

dset2   = [t_ct(1)/1e6 t_ct(2)/1e6 ;];
figure;
bar( dset2);
title('Results for CT Kernel Benchmark');
ylabel('Throughput (MByte/s)');
xlabel('CT Datasets');
set(gca,'XLim',[0 3]);

dset3   = [t_db(1) t_db(2)  ;];
figure;
bar( dset3);
ylabel('Throughput (Transactions/s)');
xlabel('DB Datasets');
title('Results for DB Kernel Benchmark');
set(gca,'XLim',[0 3]);

disp(['TDFIR']);
disp(['Dataset 1: ' num2str(l_tdFir(1)) ' s, ' num2str(t_tdFir(1)/1e6) ' Mflops/s']);
disp(['Dataset 2: ' num2str(l_tdFir(2)) ' s, ' num2str(t_tdFir(2)/1e6) ' Mflops/s']);
disp(['FDFIR']);
disp(['Dataset 1: ' num2str(l_fdFir(1)) ' s, ' num2str(t_fdFir(1)/1e6) ' Mflops/s']);
disp(['Dataset 2: ' num2str(l_fdFir(2)) ' s, ' num2str(t_fdFir(2)/1e6) ' Mflops/s']);
disp(['QR']);
disp(['Dataset 1: ' num2str(l_qr(1)) ' s, ' num2str(t_qr(1)/1e6) ' Mflops/s']);
disp(['Dataset 2: ' num2str(l_qr(2)) ' s, ' num2str(t_qr(2)/1e6) ' Mflops/s']);
disp(['Dataset 3: ' num2str(l_qr(3)) ' s, ' num2str(t_qr(3)/1e6) ' Mflops/s']);
disp(['SVD']);
disp(['Dataset 1: ' num2str(l_svd(1)) ' s, ' num2str(t_svd(1)/1e6) ' Mflops/s']);
disp(['Dataset 2: ' num2str(l_svd(2)) ' s, ' num2str(t_svd(2)/1e6) ' Mflops/s']);
disp(['Dataset 3: ' num2str(l_svd(3)) ' s, ' num2str(t_svd(3)/1e6) ' Mflops/s']);
disp(['CFAR']);
disp(['Dataset 1: ' num2str(l_cfar(1)) ' s, ' num2str(t_cfar(1)/1e6) ' Mflops/s']);
disp(['Dataset 2: ' num2str(l_cfar(2)) ' s, ' num2str(t_cfar(2)/1e6) ' Mflops/s']);
disp(['Dataset 3: ' num2str(l_cfar(3)) ' s, ' num2str(t_cfar(3)/1e6) ' Mflops/s']);
disp(['Dataset 4: ' num2str(l_cfar(4)) ' s, ' num2str(t_cfar(4)/1e6) ' Mflops/s']);
disp(['PM']);
disp(['Dataset 1: ' num2str(l_pm(1)) ' s, ' num2str(t_pm(1)/1e6) ' Mflops/s']);
disp(['Dataset 2: ' num2str(l_pm(2)) ' s, ' num2str(t_pm(2)/1e6) ' Mflops/s']);
disp(['GA']);
disp(['Dataset 1: ' num2str(l_genalg(1)) ' s, ' num2str(t_genalg(1)/1e6) ' Mflops/s']);
disp(['Dataset 2: ' num2str(l_genalg(2)) ' s, ' num2str(t_genalg(2)/1e6) ' Mflops/s']);
disp(['Dataset 3: ' num2str(l_genalg(3)) ' s, ' num2str(t_genalg(3)/1e6) ' Mflops/s']);
disp(['Dataset 4: ' num2str(l_genalg(4)) ' s, ' num2str(t_genalg(4)/1e6) ' Mflops/s']);
disp(['CT']);
disp(['Dataset 1: ' num2str(l_ct(1)) ' s, ' num2str(t_ct(1)/1e6) ' MBytes/s']);
disp(['Dataset 2: ' num2str(l_ct(2)) ' s, ' num2str(t_ct(2)/1e6) ' MBytes/s']);
disp(['DB']);
disp(['Dataset 1: ' num2str(l_db(1)) ' s, ' num2str(t_db(1)) ' Transactions/s']);
disp(['Dataset 2: ' num2str(l_db(2)) ' s, ' num2str(t_db(2)) ' Transactions/s']);

% Output results to a text file for results submission
filename = ['hpecKernelResults-' datestr(now, 'yyyy.mm.dd-HH.MM.SS') '.txt'];
fid = fopen(filename, 'w');

if(fid == -1)
  error(['Could not open results file: ' filename]);
end

% Insert the date
fprintf(fid,'%s\n',datestr(now, 'yyyymmddTHHMMSS'));

% Insert the results
fprintf(fid,'%s\n',['* TDFIR']);
fprintf(fid,'%s\n',['1, ' num2str(l_tdFir(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_tdFir(2)) ]);
fprintf(fid,'%s\n',['* FDFIR']);
fprintf(fid,'%s\n',['1, ' num2str(l_fdFir(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_fdFir(2)) ]);
fprintf(fid,'%s\n',['* QR']);
fprintf(fid,'%s\n',['1, ' num2str(l_qr(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_qr(2)) ]);
fprintf(fid,'%s\n',['3, ' num2str(l_qr(3)) ]);
fprintf(fid,'%s\n',['* SVD']);
fprintf(fid,'%s\n',['1, ' num2str(l_svd(1)) ', ' num2str(t_svd(1))]);
fprintf(fid,'%s\n',['2, ' num2str(l_svd(2)) ', ' num2str(t_svd(2))]);
fprintf(fid,'%s\n',['3, ' num2str(l_svd(3)) ', ' num2str(t_svd(3))]);
fprintf(fid,'%s\n',['* CFAR']);
fprintf(fid,'%s\n',['1, ' num2str(l_cfar(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_cfar(2)) ]);
fprintf(fid,'%s\n',['3, ' num2str(l_cfar(3)) ]);
fprintf(fid,'%s\n',['4, ' num2str(l_cfar(4)) ]);
fprintf(fid,'%s\n',['* PM']);
fprintf(fid,'%s\n',['1, ' num2str(l_pm(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_pm(2)) ]);
fprintf(fid,'%s\n',['* GA']);
fprintf(fid,'%s\n',['1, ' num2str(l_genalg(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_genalg(2)) ]);
fprintf(fid,'%s\n',['3, ' num2str(l_genalg(3)) ]);
fprintf(fid,'%s\n',['4, ' num2str(l_genalg(4)) ]);
fprintf(fid,'%s\n',['* CT']);
fprintf(fid,'%s\n',['1, ' num2str(l_ct(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_ct(2)) ]);
fprintf(fid,'%s\n',['* DB']);
fprintf(fid,'%s\n',['1, ' num2str(l_db(1)) ]);
fprintf(fid,'%s\n',['2, ' num2str(l_db(2)) ]);
fclose(fid);

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
