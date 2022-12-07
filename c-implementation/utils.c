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
        printf("\n============= Error during the file opening =============\n");
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
        printf("\n============= Error during var_id retrieval =============\n");
    else   
        printf("\n============= File read succesfully =============\n");
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
        printf("\n============= Comparing (%s, %s) : %s =============\n", options[i], var_name, strcmp(options[i], var_name) ? "false":"true");
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