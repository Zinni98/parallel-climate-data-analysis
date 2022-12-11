#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <netcdf.h>
#include <math.h>
#include "utils.h"
#include "velocity.h"


int read_velocity(int ncid, const char *options[NUM_VARS], const char* var_name , const int *start_idxs, const int *count_idxs, 
                    const int *stride_steps, float target_buffer[TIME][DEPTH][NODE2]){
    /**
     * @brief returns writing status
     * @param ncid id of netcdf file
     * @param options : array of strings containing all valid names
     * @param var_name : string representing the var_name to be checked
     * @param start_idxs : array of ints representing the starting point (each dimension) for reading a variable in ncid
     * @param count_idxs : array of ints representing the end point (each dimension) for reading a variable in ncid
     * @param stride_step : array of ints representing the step with which data is read
     * @returns error status (status_buffer), if no errors occur, const int NC_NOERR is returned
    */

    // support vars
    bool flag=0;
    int var_id;
    int status_varid;
    int status_buffer;

    // starting point, end point and stride
    const size_t startv[3] = {start_idxs[0], start_idxs[1], start_idxs[2]};
    const size_t countv[3] = {count_idxs[0], count_idxs[1], count_idxs[2]};
    const ptrdiff_t stridev[3] = {stride_steps[0], stride_steps[1], stride_steps[2]};

    flag = check_var(options, var_name);
    if(!flag){
        status_varid = nc_inq_varid(ncid, var_name, &var_id);
        if(status_varid==NC_NOERR){
            status_buffer = nc_get_vars_float(ncid, var_id, &startv[0], &countv[0], &stridev[0], &target_buffer[0][0][0]);
            if (status_buffer != NC_NOERR){
                printf("\n============= Error during data retrieval =============\n");
                return status_buffer;
            }
        }else{
            printf("\n============= Detected error while reading %s id =============\n", var_name);
        }
    }    
    return status_buffer;
}


float** compute_maximum(const int var_idx, float (*matrix)[DEPTH][NODE2], int *dimension){
    /**
     * @brief returns reduction matrix
     * @param var_idx : variable index along which to reduce
     * @param matrix : 3d matrix of data [time][depth][nodes]
     * @param dimension : support variable to adapt code
     * @returns 2d matrix of reduced data
    */
    
    float **maximums;  // dynamically allocate memory space for a matrix of floats
    
    if(var_idx == 0){                                 // returns 0 if equal
        printf("\n============= Depth reduction =============\n");
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
       printf("\n============= Time reduction =============\n");
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

void compute_norm(float vnode_mat[TIME][DEPTH][NODE2], float unode_mat[TIME][DEPTH][NODE2], float target_matrix[TIME][DEPTH][NODE2]){
    for(int time=0; time<TIME; time++){
        for(int depth=0; depth<DEPTH; depth++){
            for(int node=0; node<NODE2; node++){
                target_matrix[time][depth][node] = sqrt((pow(vnode_mat[time][depth][node],2)+pow(unode_mat[time][depth][node],2)));
            }
        }
    }
}


void compute_inverted_max(int dim1, int dim2, int dim3, float local_velocity[dim3][dim1][dim2], float local_max[dim3][dim1]){
    for(int i=0; i<dim3; i++)
    {
        for(int j=0; j<dim1; j++)
        {
            local_max[i][j] = local_velocity[i][j][0];
            for(int k=1; k<dim2; k++)
            {
                if(local_velocity[i][j][k] > local_max[i][j])
                {
                    local_max[i][j] = local_velocity[i][j][k];
                }
            }
        }
    }
    return;
}
