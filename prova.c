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
    int size = 8;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    float a [size][size];
    if(rank == 0)
    {
            for(int j=0; j<size; j++)
            {
                for(int k=0; k<size; k++)
                {
                    a[j][k] = j*size + k;
                    printf("%f ", a[j][k]);
                }
                printf("\n");
            }

    } 
    int cols=size/comm_sz;
    MPI_Datatype acol, acoltype;
    if(rank == 0)
    {
        MPI_Type_vector(size, cols, size, MPI_FLOAT, &acol);
        MPI_Type_commit(&acol);
        MPI_Type_create_resized(acol, 0, sizeof(float), &acoltype);
        MPI_Type_commit(&acoltype);
    }

    /* MPI_Scatter(a, cols, acoltype, local_a, size*cols, MPI_FLOAT, 0, MPI_COMM_WORLD); */

    int counts[comm_sz];
    for(int i=0; i<comm_sz; i++) counts[i] = 1;
    int displace[comm_sz];
    for(int i=0; i<comm_sz; i++) displace[i] = i*cols;
    float local_a[size][cols];
    int local_n = size*cols;
    MPI_Scatterv(a, counts, displace, acoltype, local_a, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for(int p=0; p<comm_sz; p++)
    {
        if(rank==p)
        {
            printf("Process %d:\n", rank);

                for(int j=0; j<size; j++)
                {
                    for(int k=0; k<cols; k++)
                    {
                        printf("%d:%d -> %f\n", j, k, local_a[j][k]);
                    }
                }

        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    /* float local_max[size][cols];
    compute_max(size, size, cols, local_a, local_max);
    float max[size][size];
    
    MPI_Datatype output_matrix; //scaled_matrix;

    MPI_Type_contiguous(size*cols, MPI_FLOAT, &output_matrix);
    MPI_Type_commit(&output_matrix);

    int recv_counts[comm_sz];
    for(int i=0; i<comm_sz; i++) recv_counts[i] = size*cols;
    int recv_displace[comm_sz];
    for(int i=0; i<comm_sz; i++) recv_displace[i] = i*size*cols;

    MPI_Gatherv(local_max, 1, output_matrix, max, recv_counts, recv_displace, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("After gathering %d:\n", rank);
        for(int i=0; i<size; i++)
        {
            for(int j=0; j<size; j++)
            {
                printf("max %d:%d -> %f\n", i, j, max[i][j]);
            }
        }
    } */

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
    {
        MPI_Type_free(&acoltype);
        MPI_Type_free(&acol);
    }
    MPI_Finalize();
    return 0;
}