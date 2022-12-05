#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <netcdf.h>
#include "vnode.h"
#include "utils.h"


float** compute_maximum(const int var_idx, float (*matrix)[DEPTH][NODE2], int *dimension){
    /**
     * @brief returns reduction matrix
     * @param var_idx : variable index along which the reduction is performed
     * @param matrix : 3d matrix of data [time][depth][nodes]
     * @param dimension : support variable to adapt code
     * @returns 2d matrix of reduced data
    */
    
    float **maximums;  // dynamically allocate memory space for a matrix of floats
    
    if(var_idx == 0){                                 // returns 0 if equal
        printf("Depth reduction\n");
        *dimension = TIME;
        maximums = (float **)malloc(TIME * sizeof(float *));
        // maximum matrix initialiaztion
        for(int time=0; time<TIME; time++){
            maximums[time] = (float*)malloc(NODE2 * sizeof(float*));
            maximums[time][0] = matrix[time][0][0];
            for(int dpth=0; dpth<DEPTH; dpth++){
                for(int node=0; node<NODE2; node++){
                    if(maximums[time][node] < matrix[time][dpth][node]){
                        maximums[time][node] = matrix[time][dpth][node];
                    }
                }
            }
        }
    }else{
       printf("Time reduction\n");
        *dimension = DEPTH;
        maximums = (float **)malloc(DEPTH * sizeof(float *));
        // maximum matrix initialiaztion
        for(int dpth=0; dpth<DEPTH; dpth++){
            maximums[dpth] = (float*)malloc(NODE2 * sizeof(float*));
            maximums[dpth][0] = matrix[0][dpth][0];
            for(int time=0; time<TIME; time++){
                for(int node=0; node<NODE2; node++){
                    if(maximums[dpth][node] < matrix[time][dpth][node]){
                        maximums[dpth][node] = matrix[time][dpth][node];
                    }
                }
            }
        } 
    }
    return maximums;
}