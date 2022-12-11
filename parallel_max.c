#include <stdio.h>
#include <netcdf.h>
#include <stdint.h>
#include <mpi.h>
#include <math.h>
#include "utils.h"
#include "vnode.h"
#include "velocity.h"


int main(int argc, char**argv){
    int comm_sz, rank, cols;
    int counts[comm_sz], displace[comm_sz];
    float vnod[TIME][DEPTH][NODE2];  // not very convenient since all processes reads it. Think to allocate dynamically inside rank 0

    /* Initializing the MPI environment */
    MPI_Init(NULL,NULL);
    /* Reading processes' ranks and number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank==0){
        /* Only process 0 reads velocity matrix */
        /*
        Allocating space dynamically so that the matrix is seen only by process 0
        float ***vnod = (float***)malloc(TIME*sizeof(float**));
        for (int i = 0; i < TIME; i++){
            vnod[i] = (float**)malloc(DEPTH * sizeof(float*));
            for (int j = 0; j < NODE2; j++){
                vnod[i][j] = (float*)malloc(NODE2 * sizeof(float));
            }
        }
        */ 

        int status = 0;
        const char* vars[] = {"nz1", "time", "vnod"};
        int var_ids[3];
        const int startv[3] = {0, 0, 0};
        const int countv[3] = {TIME, DEPTH, NODE2};
        const int stridev[3] = {1, 1, 1};
        // get file id
        int ncid = get_file_id(PATH2VNODE);
        // get var ids
        get_var_ids(ncid, vars, var_ids, 3);
        status = read_velocity(ncid, vars, vars[2], startv, countv, stridev, vnod);
        if(status){
            printf("Error while retrieving vnod data\n");
        }
        /* at this point process 0 should have read the velocity matrix */ 
    }

    /* number of mesh point assigned to all processes */ 
    cols = NODE2/comm_sz;

    /* If number of processes not a dividend, we need to deal with the last process */
    if(rank==comm_sz-1){
        cols = NODE2 - (comm_sz-1) * cols;
    }

    /* columns are not contiguos in memory, we need to deal with it by defining a custom type */
    MPI_Datatype vcol, vcoltype;
    if(rank == 0){
        /* Custom type definition. This matters only for the scatter since the view on the receiving side is different*/
        MPI_Type_vector(TIME*DEPTH, 1, NODE2, MPI_FLOAT, &vcol);    // type that count TIME*DEPTH elements, taken 1 at a time and jumping NODE2 elements for the next one
        MPI_Type_commit(&vcol);     // pushing the type to let MPI know
        MPI_Type_create_resized(vcol, 0, sizeof(float), &vcoltype);     // single element memory size definition
        MPI_Type_commit(&vcoltype);
    }


    /* Enstablishing how many elements to send to each process */
    /* Here all processes run this for. Doesn't make sense since it is useful just in transmission. */
    /* Evaluate to insert a condition just for rank 0*/
    /* if (rank == 0) { */
    counts_and_displace(counts, displace, comm_sz, cols, 's');
    //for (int i = 0; i < comm_sz; i++)
    //{
        //if (i==(comm_sz-1))
        //{
            //counts[i] = NODE2 - (comm_sz-1) * cols;
        //}else{
            //counts[i] = cols;
        //}
        //displace[i] = i*cols;
    //}
    /*}*/


    /* Definition of local matrix to be scattered to different processes and number of elements for each submatrix*/
    float local_vnod[cols][TIME][DEPTH];
    int local_n = TIME*DEPTH*cols;

    /* https://www.open-mpi.org/doc/v4.0/man3/MPI_Scatterv.3.php */
    MPI_Scatterv(vnod, counts, displace, vcoltype, local_vnod, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* ################################################# GATHER ############################################ */ 

    /* Each process computes locally the maximum along the mesh they know*/
    float local_max[cols][TIME], max[TIME][NODE2];
    compute_inverted_max(TIME, DEPTH, cols, local_vnod, local_max);

    /* At this point, each process has computed the local maximum */

    /* Initialization of global maximums matrix*/
    pop_max_matrix(max, TIME, cols);

    /* The gathering type is different than the scattered type. We need to define another type to build the output data as desired */
    /* This comes from how the Scatterv operation is defined */
    MPI_Datatype max_col, max_coltype;
    MPI_Type_vector(TIME, 1, NODE2, MPI_FLOAT, &max_col);
    MPI_Type_commit(&max_col);
    MPI_Type_create_resized(max_col, 0, sizeof(float), &max_coltype);
    MPI_Type_commit(&max_coltype);

    /* Enstablishing how many elements to receive from each process */
    int recv_counts[comm_sz], recv_displace[comm_sz];
    counts_and_displace(recv_counts, recv_displace, comm_sz, cols, 's');
    //for(int i=0; i<comm_sz; i++)
    //{
        //if(i == comm_sz - 1){
            //recv_counts[i] = TIME * (NODE2 - (comm_sz - 1) * cols);
        //}
        //else{
            //recv_counts[i] = cols*TIME;
        //}
        //displace[i] = cols*i;
    //}

    /* https://www.open-mpi.org/doc/v4.1/man3/MPI_Gatherv.3.php */
    MPI_Gatherv(local_max, cols*TIME, MPI_FLOAT, max, recv_counts, recv_displace, max_coltype, 0, MPI_COMM_WORLD);
    if(rank==0)
    {
        printf("After gathering %d:\n", rank);
        for(int i=0; i<TIME; i++)
        {
            for(int j=0; j<NODE2; j++)
            {
                printf("%f ", max[i][j]);
            }
            printf("\n");
        }
    } 

    /* Deallocating the pushed type. It is important to wait for all the processes to finish their task.*/
    /* For this reason, for safety we placed a barrier here. */
    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0){
        MPI_Type_free(&vcoltype);
        MPI_Type_free(&vcol);
    }
    MPI_Type_free(&max_coltype);
    MPI_Type_free(&max_col);

    /* Close MPI environment */
    MPI_Finalize();

    return 0;
}
