#!/bin/bash
#PBS -l select=1:ncpus=1:mem=2gb
#PBS -l walltime=01:30:00
#PBS -q short_cpuQ


#rm ~/local/runner.sh.o*
#rm ~/local/runner.sh.e*

mpirun.actual -n 1 ~/UnStructuredGrid/parallel-climate-data-analysis/c-implementation/node