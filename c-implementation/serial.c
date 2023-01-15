#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <netcdf.h>
#include <mpi.h>
#include "utils.h"


int main(int argc, char**argv)
{
    int comm_sz, rank;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("\n============= Serial =============\n");
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

    double start = MPI_Wtime();
    vnod_reduced = compute_maximum(0, vnod, &dimension);
    waste_time((long)TIME*DEPTH*NODE2*50);
    double end = MPI_Wtime();

    double tot = end - start;

    printf("proc,time\n");
    printf("%d, %f", 1, tot);
    // printf("Printing first %d elements of reduced matrix:\n", NODE2);
    /* for(int d=0; d<TIME; d++){
        for(int node=0; node<NODE2;node++){
            printf("%f ", vnod_reduced[d][node]);
        }
        printf("\n");
    } */

    free(vnod_reduced);
    MPI_Finalize();
    return 0;
}