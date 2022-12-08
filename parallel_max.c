#include <stdio.h>
#include <netcdf.h>
#include <mpi.h>
#include "utils.h"

double** parallel_max(const int var_idx,
                      int time,
                      int depth,
                      int nod2,
                      float matrix[time][depth][nod2])
{
    int comm_sz, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
    float **max_values;
    max_values = (float **)malloc(TIME * sizeof(float *));
    // maximum matrix initialiaztion
    for(int time=0; time<TIME; time++){
        max_values[time] = (float*)malloc(NODE2 * sizeof(float*));
        max_values[time][0] = matrix[time][0][0];
        for(int dpth=0; dpth<DEPTH; dpth++){
            for(int node=0; node<NODE2; node++){
                if(max_values[time][node] < matrix[time][dpth][node]){
                    max_values[time][node] = matrix[time][dpth][node];
                }
            }
        }
    }
    

    MPI_Finalize();
}

int main(int argc, char** argv)
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



    return 0;
}