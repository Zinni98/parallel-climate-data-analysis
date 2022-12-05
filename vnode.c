#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <netcdf.h>


#define TIME 12
#define DEPTH 69
#define NODE2 8852366
#define NUM_VARS 3


int get_file_id(const char* filename){
    /**
     * @param filename path to the netcdf file
     * @returns id of the file
    */
    int ncid;
    int status = nc_open(filename, NC_NOWRITE, &ncid);
    if(status != NC_NOERR)
        printf("Error during the file opening\n");
    return ncid;
}

void get_var_ids(int ncid, const char *var_names[NUM_VARS], int var_ids[], int count){
    /**
     * @param ncid id of netcdf file
     * @param var_names array of netcdf variable names for which ids should be ectracted
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
        printf("Error during var_id retrieval\n");
    else   
        printf("File read succesfully\n");
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
        printf("Comparing (%s, %s) : %s\n", options[i], var_name, strcmp(options[i], var_name) ? "false":"true");
        if(!strcmp(options[i], var_name)){
            break;
        }
        if(i==2){
            flag = 1;
            printf("Undefined variable name as input\n");
            }
        }
    return flag;
}


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
            // wrting all "var_name" related data from file with id "ncid"
            // from "startv" to "countv" with step "stridev" (dimension-wise) to "target buffer"
            status_buffer = nc_get_vars_float(ncid, var_id, &startv[0], &countv[0], &stridev[0], &target_buffer[0][0][0]);
            if (status_buffer != NC_NOERR){
                printf("Error during data retrieval\n");
                return status_buffer;
            }
        }else{
            printf("Detected error while reading %s id", var_name);
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


int main(int argc, char**argv)
{
    // var declaration
    const char* path = "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc";
    const char* vars[] = {"nz1", "time", "vnod"};
    int var_ids[3];
    float vnod[TIME][DEPTH][NODE2];
    float **vnod_reduced;
    int status = 0;
    int dimension = 0;
    
    // get file id
    int ncid = get_file_id(path);
    // get var ids
    get_var_ids(ncid, vars, var_ids, 3);
    
    const int startv[3] = {0, 0, 0};
    const int countv[3] = {TIME, DEPTH, NODE2};
    const int stridev[3] = {1, 1, 1};

    status = read_velocity(ncid, vars, vars[2], startv, countv, stridev, vnod);
    if (status != NC_NOERR){
        printf("Error during lon var retrieval\n");
        return 1;
    }

    vnod_reduced = compute_maximum(0, vnod, &dimension);
    printf("Printing first %d elements of reduced matrix:\n", NODE2);
    for(int d=0; d<dimension; d++){
        for(int node=0; node<NODE2;node++){
            printf("\n%d %d %f", d, node, vnod_reduced[d][node]);
        }
    }

    free(vnod_reduced);
    return 0;
}
