#!/bin/bash

#SBATCH -J 
#SBATCH -A 
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o result.out
#SBATCH -e result.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=wengchic@oregonstate.edu

for block in 16 32 64 128
do  

    for trial in 16384 32768 65536 131072 262144 524288 1048576
    do
        /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DBLOCKSIZE=$block -DNUMTRIALS=$trial -o main main.cu
        ./main
    done
done