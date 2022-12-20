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
    // var declaration
    const char* path2vnode = PATH2VNODE;
    //const char* path2unode = PATH2UNODE;
    const char* vars_vnode[] = {"nz1", "time", "vnod"};           // nz1 and time are unused
    //const char* vars_unode[] = {"nz1", "time", "unod"};           // nz1 and time are unused
    int var_ids[3];
    float vnod[TIME][DEPTH][NODE2];
    //float unod[TIME][DEPTH][NODE2];
    //float velocity[TIME][DEPTH][NODE2];
    float **vnod_reduced;
    //float **unod_reduced;
    //float **norm_reduced;
    int status = 0;
    int dimension = 0;
    const int start[3] = {0, 0, 0};
    const int count[3] = {TIME, DEPTH, NODE2};
    const int stride[3] = {1, 1, 1};

    double start, end, tot;
    
    // get file id
    int ncid_vnode = get_file_id(path2vnode);
    //int ncid_unode = get_file_id(path2unode);
    // get var ids
    get_var_ids(ncid_vnode, vars_vnode, var_ids, 3);
    //get_var_ids(ncid_unode, vars_unode, var_ids, 3);
    // read vnode and check outcome
    status = read_velocity(ncid_vnode, vars_vnode, vars_vnode[2], start, count, stride, vnod);
    status = check_status(&status, vars_vnode[2]);
    if(!status){
        start = MPI_Wtime();
        vnod_reduced = compute_maximum(0, vnod, &dimension);
        end = MPI_Wtime();
    }

    tot = end - start;
    printf("# Processes,Time\n");
    printf("%d, %f", 1, tot);

    // read unode and check outcome
    /*
    status = read_velocity(ncid_unode, vars_unode, vars_unode[2], start, count, stride, unod);
    status = check_status(&status, vars_unode[2]);
    if(!status){
        unod_reduced = compute_maximum(0, unod, &dimension);
    }
    */


    // print reduced vnode and unode
    //print_2d_matrix(vnod_reduced, dimension, vars_vnode[2]);
    //print_2d_matrix(unod_reduced, dimension, vars_vnode[2]);
    // compute vector

    /*
    compute_norm(vnod, unod, velocity);
    norm_reduced = compute_maximum(0, velocity, &dimension);
    print_2d_matrix(norm_reduced, dimension, "node norm");
    */
    
    // free memory
    free(vnod_reduced);
    /*free(unod_reduced);
    free(norm_reduced);*/
    
    return 0;
}