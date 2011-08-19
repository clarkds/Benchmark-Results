#!/bin/csh

##############################################################################
# File: exec_kernels.csh
#
# HPEC Challenge Benchmark Suite
# Top Level Run Script
#
# Contents:
#  Script to run and verify each kernel for each dataset.
#
# Usage:
#  Typing ./exec_kernels.csh will run then verify each kernel /dataset pair.
#
# Author: Jeanette Baran-Gale
#         MIT Lincoln Laboratory
#
##############################################################################


                 #cfar ct db fdfir ga pm qr svd tdfir
set exec_names = (cfar ct db fdFir genalg pm qr svd tdFir)
set ndata=       (   4  2  2     2      4  2  3   3     2) # Number of datasets for each kernel

set ikernel=1 # kernel index
 
foreach dir (cfar ct db fdfir ga pm qr svd tdfir) # for each kernel
  cd $dir #change to that directory
  set data=1 # start with dataset 1
  while (${data} <= $ndata[$ikernel]) #run all datasets
    echo $exec_names[$ikernel] " " ${data}  # echo kernel and dataset to screen
    echo $PWD
    ./$exec_names[$ikernel] ${data} # run kernel / dataset
    ./$exec_names[$ikernel]"Verify" ${data} #run verifiy dataset
    @ data +=1 # increment dataset
  end
  @ ikernel +=1 # go to next kernel
  cd ../ # return to top directory
end

##############################################################################
# Copyright (c) 2006, Massachusetts Institute of Technology
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are  
# met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Massachusetts Institute of Technology nor  
#       the names of its contributors may be used to endorse or promote 
#       products derived from this software without specific prior written 
#       permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF  
# THE POSSIBILITY OF SUCH DAMAGE.
##############################################################################
