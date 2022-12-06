#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <netcdf.h>
#include "vnode.h"
#include "utils.h"


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