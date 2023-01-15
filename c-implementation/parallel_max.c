#include <math.h>
#include <mpi.h>
#include <netcdf.h>
#include <stdint.h>
#include <stdio.h>

#include "utils.h"

int main(int argc, char **argv) {
  /**
   * Note that the number of processes should be at least 6
   * Otherwise it doesn't work when all nodes of the mesh
   * are used.
   */
  int comm_sz, rank, cols;
  int counts[comm_sz], displace[comm_sz];
  float vnod[TIME][DEPTH][NODE2];

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    int status = 0;
    const char *vars[] = {"nz1", "time", "vnod"};
    int var_ids[3];
    const int startv[3] = {0, 0, 0};
    const int countv[3] = {TIME, DEPTH, NODE2};
    const int stridev[3] = {1, 1, 1};

    int ncid = get_file_id(PATH2VNODE);
    get_var_ids(ncid, vars, var_ids, 3);
    status = read_velocity(ncid, vars, vars[2], startv, countv, stridev, vnod);
    if (status) {
      printf("Error while retrieving vnod data\n");
    }
    printf("\n");
    /* at this point process 0 should have read the velocity matrix */
  }

  MPI_Barrier(MPI_COMM_WORLD);

  double start = MPI_Wtime();

  cols = NODE2 / comm_sz;
  if (rank == comm_sz - 1) {
    cols = NODE2 - (comm_sz - 1) * cols;
  }

  MPI_Datatype vcol, vcoltype;
  if (rank == 0) {
    MPI_Type_vector(TIME * DEPTH, 1, NODE2, MPI_FLOAT, &vcol);
    MPI_Type_commit(&vcol);
    MPI_Type_create_resized(vcol, 0, sizeof(float), &vcoltype);
    MPI_Type_commit(&vcoltype);
  }

  for (int i = 0; i < comm_sz; i++) {
    if (i == comm_sz - 1) {
      counts[i] = NODE2 - (comm_sz - 1) * cols;
    } else {
      counts[i] = cols;
    }
  }
  for (int i = 0; i < comm_sz; i++)
    displace[i] = i * cols;
  float local_vnod[cols][TIME][DEPTH];
  int local_n = TIME * DEPTH * cols;
  MPI_Scatterv(vnod, counts, displace, vcoltype, local_vnod, local_n, MPI_FLOAT,
               0, MPI_COMM_WORLD);

  float local_max[cols][TIME];
  compute_inverted_max(TIME, DEPTH, cols, local_vnod, local_max);
  waste_time((long)TIME * DEPTH * cols * 100);

  float max[TIME][NODE2];

  MPI_Datatype recv, recv_type;
  // (count, block_size, step)
  MPI_Type_vector(TIME, 1, NODE2, MPI_FLOAT, &recv);
  MPI_Type_commit(&recv);
  MPI_Type_create_resized(recv, 0, sizeof(float), &recv_type);
  MPI_Type_commit(&recv_type);

  int recv_counts[comm_sz];
  for (int i = 0; i < comm_sz; i++) {
    if (i == comm_sz - 1) {
      recv_counts[i] = TIME * (NODE2 - (comm_sz - 1) * cols);
    } else {
      recv_counts[i] = cols * TIME;
    }
  }
  int recv_displace[comm_sz];
  for (int i = 0; i < comm_sz; i++)
    recv_displace[i] = i * cols;

  MPI_Gatherv(local_max, cols * TIME, MPI_FLOAT, max, recv_counts,
              recv_displace, recv_type, 0, MPI_COMM_WORLD);

  double end = MPI_Wtime();
  double tot = end - start;

  if (rank == 0) {
    printf("# Processes,Time\n");
    printf("%d, %f", comm_sz, tot);
    MPI_Type_free(&vcol);
    MPI_Type_free(&vcoltype);
  }
  MPI_Type_free(&recv);
  MPI_Type_free(&recv_type);
  MPI_Finalize();

  return 0;
}
