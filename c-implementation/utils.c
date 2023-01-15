#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <netcdf.h>
#include <math.h>
#include "utils.h"


int get_file_id(const char* filename){
    /**
     * @param filename path to the netcdf file
     * @returns id of the file
    */
    int ncid;
    int status = nc_open(filename, NC_NOWRITE, &ncid);
    if(status != NC_NOERR)
        printf("\n============= Error during the file opening =============\n");
    return ncid;
}


void get_var_ids(int ncid, const char *var_names[NUM_VARS], int var_ids[], int count){
    /**
     * @param ncid id of netcdf file
     * @param var_names array of netcdf variable names for which ids should be extracted
     * @param var_ids array used to store the result
     * @param count number of elements in var_names
    */
    int status;
    for(int i=0; i<count; i++)
    {
        int var_id;
        status = nc_inq_varid(ncid, var_names[i], &var_id);
        if(status != NC_NOERR)
            break;
        else
            var_ids[i] = var_id;
    }
    if (status != NC_NOERR)
        printf("\n============= Error during var_id retrieval =============\n");
}


int read_velocity(int ncid, const char *options[NUM_VARS], const char* var_name , const int *start_idxs, const int *count_idxs, 
                    const int *stride_steps, float target_buffer[TIME][DEPTH][NODE2]){
    /**
     * @brief Wrapper for reading velocity (unod, vnod) from fesom netcdf files.
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


void print_2d_matrix(float **reduced_matrix, int num_elements, char *var_name){
    /*
    * @brief print the elements of a 2D matrix
    * @param reduced_matrix : matrix with maximums
    * @param num_elemnts : elemnts to be prited
    */
    printf("\n============= Printing first %d elements of reduced matrix: =============\n", NODE2);
    for(int d=0; d<num_elements; d++){
        for(int node=0; node<NODE2;node++){
            printf("\n%d %d %f", d, node, reduced_matrix[d][node]);
        }
    }
}


int check_status(int *status, const char *var_name){
    /*
     * @brief check exit status
     * @param status : where to save error status
     * @param var_name : name of the variable read in a command that returns status
     */
    int local_status = 0;
    if ((*status) != NC_NOERR){
        printf("\n============= Error during %s var retrieval =============\n", var_name);
        local_status = 1;
    }
    return local_status;
}


bool check_var(const char *options[NUM_VARS], const char *var_name){
    /**
     * @brief return a bool for checking if a variable name is valid
     * @param options : array of strings containing all valid names
     * @param var_name : string representing the var_name to be checked
     * @returns boolean flag, which is 0 if the variable exists, otherwise 1
    */
    bool flag=0;
    for(int i=0; i<3; i++){
        if(!strcmp(options[i], var_name)){
            break;
        }
        if(i==2){
            flag = 1;
            printf("\n============= Undefined variable name as input =============\n");
            }
        }
    return flag;
}


void pop_max_matrix(float buffer[TIME][NODE2], int rows, int cols){
    /**
     * @brief populate a new brand matrix
     * @param buffer : target matrix to populate
     * @param rows : row dimension
     * @param cols : columns dimension
    */
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            buffer[i][j] = -1;
        }
    }
}


float** compute_maximum(const int var_idx, float (*matrix)[DEPTH][NODE2], int *dimension){
    /**
     * @brief returns reduction matrix
     * @param var_idx : variable index along which to reduce
     * @param matrix : 3d matrix of data [time][depth][nodes]
     * @param dimension : support variable to adapt code
     * @returns 2d matrix of reduced data
    */
    
    float **maximums;
    
    if(var_idx == 0){
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


void waste_time(long iterations){
    long a = 0;
    for(int i; i<iterations; i++){
        a += 1;
    }
}





