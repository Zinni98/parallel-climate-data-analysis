#include <stdio.h>
#include <netcdf.h>
#include <stdint.h>
#include <mpi.h>

void compute_max(int dim1,
                   int dim2,
                   int dim3,
                   float local_velocity[dim1][dim2][dim3],
                   float local_max[dim1][dim3])
{
    for(int i=0; i<dim1; i++)
    {
        for(int j=0; j<dim3; j++)
        {
            local_max[i][j] = local_velocity[i][0][j];
            for(int k=0; k<dim2; k++)
            {
                if(local_velocity[i][k][j] > local_max[i][j])
                {
                    local_max[i][j] = local_velocity[i][k][j];
                }
            }
        }
    }
    return;
}

int main(int argc, char**argv)
{
    int comm_sz, rank;
    int time = 2;
    int depth = 3;
    int size = 4;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    float a[time][depth][size];
    if(rank == 0)
    {
        for(int i=0; i<time; i++)
        {
            for(int j=0; j<depth; j++)
            {
                for(int k=0; k<size; k++)
                {
                    a[i][j][k] = i*size*depth + j*size + k;
                    printf("%f ", a[i][j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
    // inspiration from https://stackoverflow.com/questions/5512245/mpi-scatter-sending-columns-of-2d-array
    int cols=size/comm_sz;
    MPI_Datatype acol, acoltype;
    if(rank == 0)
    {
        MPI_Type_vector(time*depth, cols, size, MPI_FLOAT, &acol);
        MPI_Type_commit(&acol);
        MPI_Type_create_resized(acol, 0, sizeof(float), &acoltype);
        MPI_Type_commit(&acoltype);
    }

    /* MPI_Scatter(a, cols, acoltype, local_a, size*cols, MPI_FLOAT, 0, MPI_COMM_WORLD); */

    int counts[comm_sz];
    for(int i=0; i<comm_sz; i++) counts[i] = 1;
    int displace[comm_sz];
    for(int i=0; i<comm_sz; i++) displace[i] = i*cols;
    float local_a[time][depth][cols];
    int local_n = time*depth*cols;
    MPI_Scatterv(a, counts, displace, acoltype, local_a, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for(int p=0; p<comm_sz; p++)
    {
        if(rank==p)
        {
            printf("Process %d:\n", rank);
            for(int i=0;i<time;i++)
            {
                for(int j=0; j<depth; j++)
                {
                    for(int k=0; k<cols; k++)
                    {
                        printf("%d:%d:%d -> %f\n", i, j, k, local_a[i][j][k]);
                    }
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    float local_max[time][cols];
    compute_max(time, depth, cols, local_a, local_max);

    for(int p=0; p<comm_sz; p++)
    {
        if(rank==p)
        {
            printf("Max @ Process %d:\n", rank);
            for(int i=0;i<time;i++)
            {
                for(int j=0; j<cols; j++)
                {
                    printf("%d:%d -> %f\n", i, j, local_max[i][j]);
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    

    float max[time][size];

    MPI_Datatype bcol, bcoltype;
    if(rank == 0)
    {
        MPI_Type_vector(time, cols, size, MPI_FLOAT, &bcol);
        MPI_Type_commit(&bcol);
        MPI_Type_create_resized(bcol, 0, sizeof(float), &bcoltype);
        MPI_Type_commit(&bcoltype);
    }

    int recv_counts[comm_sz];
    for(int i=0; i<comm_sz; i++) recv_counts[i] = 1;
    int recv_displace[comm_sz];
    for(int i=0; i<comm_sz; i++) recv_displace[i] = i*cols;


    MPI_Gatherv(local_max, time*cols, MPI_FLOAT, max, recv_counts, recv_displace, bcoltype, 0, MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("After gathering %d:\n", rank);
        for(int i=0; i<time; i++)
        {
            for(int j=0; j<size; j++)
            {
                printf("max %d:%d -> %f\n", i, j, max[i][j]);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
    {
        MPI_Type_free(&acoltype);
        MPI_Type_free(&acol);
        MPI_Type_free(&bcoltype);
        MPI_Type_free(&bcol);
    }
    MPI_Finalize();
    return 0;
}