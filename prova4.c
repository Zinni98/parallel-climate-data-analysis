#include <stdio.h>
#include <netcdf.h>
#include <stdint.h>
#include <mpi.h>
#include <math.h>


int ceiling(int a, int b)
{
    if(a%b == 0) return (int) a/b;
    else return (int)(a/b) + 1;
}

void compute_inverted_max(int dim1,
                            int dim2,
                            int dim3,
                            float local_velocity[dim3][dim1][dim2],
                            float local_max[dim3][dim1])
{
    for(int i=0; i<dim3; i++)
    {
        for(int j=0; j<dim1; j++)
        {
            local_max[i][j] = local_velocity[i][j][0];
            for(int k=1; k<dim2; k++)
            {
                if(local_velocity[i][j][k] > local_max[i][j])
                {
                    local_max[i][j] = local_velocity[i][j][k];
                }
            }
        }
    }
    return;
}
void compute_max(int dim1,
                   int dim2,
                   int dim3,
                   float local_velocity[dim3][dim1][dim2],
                   float local_max[dim1][dim3])
{
    for(int i=0; i<dim3; i++)
    {
        for(int j=0; j<dim1; j++)
        {
            local_max[j][i] = local_velocity[i][j][0];
            for(int k=1; k<dim2; k++)
            {
                if(local_velocity[i][j][k] > local_max[j][i])
                {
                    local_max[j][i] = local_velocity[i][j][k];
                }
            }
        }
    }
    return;
}

int main(int argc, char**argv)
{
    int comm_sz, rank;
    int time = 3;
    int depth = 2;
    int size = 17;
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

    int cols;
    cols = size/comm_sz;//ceiling(size,comm_sz);
    if(rank == comm_sz - 1)
    {
        // cols = size/comm_sz;ceiling(size,comm_sz);
        cols = size - (comm_sz - 1) * cols;//size - ceiling(size, comm_sz) * (comm_sz - 1);
    }

    MPI_Datatype acol, acoltype;
    if(rank == 0)
    {
        MPI_Type_vector(time*depth, 1, size, MPI_FLOAT, &acol);
        MPI_Type_commit(&acol);
        MPI_Type_create_resized(acol, 0, sizeof(float), &acoltype);
        MPI_Type_commit(&acoltype);
    }

    int counts[comm_sz];
    for(int i=0; i<comm_sz; i++)
    {
        if(i == comm_sz - 1)
        {    
            counts[i] = size - (comm_sz - 1) * cols;//ceiling(size,comm_sz) * (comm_sz - 1)) ;
        }
        else
            counts[i] = cols;
    }
    int displace[comm_sz];
    for(int i=0; i<comm_sz; i++) displace[i] = i*cols;
    float local_a[cols][time][depth];
    int local_n = time*depth*cols;
    MPI_Scatterv(a, counts, displace, acoltype, local_a, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for(int p=0; p<comm_sz; p++)
    {
        if(rank==p)
        {
            printf("Process %d:\n", rank);
            for(int i=0;i<time;i++)
            {
                for(int j=0; j<cols; j++)
                {
                    for(int k=0; k<depth; k++)
                    {
                        printf("%f ", local_a[i][j][k]);
                    }
                    printf("\n");
                }
                printf("\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    

    // ##################### GATHER ############################################

    float local_max[cols][time];
    compute_inverted_max(time, depth, size, local_a, local_max);
    // compute_max(time, depth, cols, local_a, local_max);

    for(int p=0; p<comm_sz; p++)
    {
        if(rank==p)
        {
            printf("Max Process %d:\n", rank);
            for(int i=0;i<cols;i++)
            {
                for(int j=0; j<time; j++)
                {
                    printf("%f ", local_max[i][j]);
                }
                printf("\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    
    float max[time][size];
    for(int i=0; i<time; i++)
    {
        for(int j=0; j<size; j++)
            max[i][j] = -1;
    }

    MPI_Datatype bcol, bcoltype;
    //(count, block_size, step)
    MPI_Type_vector(time, 1, size, MPI_FLOAT, &bcol);
    MPI_Type_commit(&bcol);
    MPI_Type_create_resized(bcol, 0, sizeof(float), &bcoltype);
    MPI_Type_commit(&bcoltype);


    int recv_counts[comm_sz];
    for(int i=0; i<comm_sz; i++)
    {
        if(i == comm_sz - 1)
        {
            recv_counts[i] = time * (size - (comm_sz - 1) * cols);//ceiling(size,comm_sz) * (comm_sz - 1));
        }
        else
            recv_counts[i] = cols*time;
    }
    int recv_displace[comm_sz];
    for(int i=0; i<comm_sz; i++) recv_displace[i] = i*cols;

    MPI_Gatherv(local_max, cols*time, MPI_FLOAT, max, recv_counts, recv_displace, bcoltype, 0, MPI_COMM_WORLD);

    if(rank==0)
    {
        printf("After gathering %d:\n", rank);
        for(int i=0; i<time; i++)
        {
            for(int j=0; j<size; j++)
            {
                printf("%f ", max[i][j]);
            }
            printf("\n");
        }
    } 

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank==0)
    {
        MPI_Type_free(&acoltype);
        MPI_Type_free(&acol);
    }
    MPI_Type_free(&bcoltype);
    MPI_Type_free(&bcol);
    MPI_Finalize();
    return 0;
}