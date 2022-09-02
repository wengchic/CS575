#!/bin/bash

#SBATCH -J project5
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o project5.out
#SBATCH -e project5.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=wengchic@oregonstate.edu

for block in 16 32 64 128
do  

    for trial in 16384 32768 65536 131072 262144 524288 1048576
    do
        /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DBLOCKSIZE=$block -DNUMTRIALS=$trial -o project5 project5.cu
        ./project5
    done
done