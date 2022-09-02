#!/bin/bash

#SBATCH -J project6
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o project6.out
#SBATCH -e project6.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=wengchic@oregonstate.edu


echo "--------------------------------------------"
echo "Local Size, Array Size,  MegaPerformance"

for LOCALSIZE in 8 16 32 64 128 256 512
    do  
        for NUM_ELEMENTS in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608
            do
                g++ -DLOCAL_SIZE=$LOCALSIZE -DNUM_ELEMENTS=$NUM_ELEMENTS -o first first.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
                ./first
            done
    done