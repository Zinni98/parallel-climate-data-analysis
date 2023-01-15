# TOC

<!--toc:start-->
- [Authors](#authors)
  - [Parallel depth data reduction of FESOM2 data](#parallel-depth-data-reduction-of-fesom2-data)
  - [Problem statement](#problem-statement)
    - [Task in details](#task-in-details)
  - [Results](#results)
<!--toc:end-->


# Authors
- Alessandro Zinni (Repository Owner) : [@github](https://github.com/Zinni98)
- Matteo Guglielmi (Collaborator) : [@github](https://github.com/MatteoGuglielmi-tech)

## Parallel depth data reduction of FESOM2 data
This repository contains an implementation of a parallel data reduction performed upon FESOM2 data as practical part of the course "High Performance Computing".

## Problem statement
In this project we were asked to perform some sort of data reduction along one arbitrary dimension of the FESOM2 data. In our case, we chose to perform a reduction along 
the depth dimension using a maximum based approach. The main reason behind this choice is that, since FESOM2 data encode measurements across the whole globe, comprising the 
oceans, depth reduction is considered to be the most realistic application.  
In particular, to perform this task we exploited the HPC@UniTrento cluster; a high performance infrastructure located in the University of Trento. 

### Task in details
The general task consisted in performing the aforementioned data reduction exploiting:
- PBS 
- netCDF
- MPI APIs

## Results
To address the improvements achieved with respect to the serial implementation we used three different performance measurements:
- time: the most trivial one. It simply consists in observing the execution times 
- speedup defined as $S=\dfrac{T_{serial}}{T_{parallel}}$
- efficiency defined as $E=\dfrac{S}{p}=\dfrac{T_{serial}}{p \cdot T_{parallel}}$

Such trends can be found in the [trends](https://github.com/Zinni98/parallel-climate-data-analysis/tree/main/python-tools/trends) folder.

