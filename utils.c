#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <netcdf.h>
#include "utils.h"


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
