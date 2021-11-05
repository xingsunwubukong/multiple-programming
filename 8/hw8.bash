#!/bin/bash
#SBATCH -J hw8
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 8 # number of nodes
#SBATCH -n 8 # number of tasks
#SBATCH -o hw8.out
#SBATCH -e hw8.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=sunhu@oregonstate.edu
        module load slurm
        module load openmpi/3.1
for s in 1 2 4 8
  do
    for y in 1 2 4 8 16 32
      do
        mpic++ -DNEL=$s hw8.cpp -o hw8 -lm
        mpiexec -mca btl self,tcp -np $y hw8
      done
  done