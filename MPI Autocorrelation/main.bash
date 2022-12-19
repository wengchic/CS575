#!/bin/bash

#SBATCH -J 
#SBATCH -A 
#SBATCH -p class
#SBATCH -N 8 # number of nodes
#SBATCH -n 8 # number of tasks
#SBATCH -o result.out
#SBATCH -e result.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=wengchic@oregonstate.edu
module load slurm
module load openmpi/3.1

for t in 1 2 4 8 16 32 
    do

    mpic++ main.cpp -o main -lm
    mpiexec -mca btl self,tcp -np $t main

    done