#include <mpi.h>
#include <netcdf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int main(int argc, char **argv) {
  int comm_sz, rank;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  const char *path = PATH2VNODE;
  const char *vars[] = {"nz1", "time", "vnod"};
  int var_ids[3];
  float vnod[TIME][DEPTH][NODE2];
  float **vnod_reduced;
  int status = 0;
  int dimension = 0;

  int ncid = get_file_id(path);
  get_var_ids(ncid, vars, var_ids, 3);

  const int startv[3] = {0, 0, 0};
  const int countv[3] = {TIME, DEPTH, NODE2};
  const int stridev[3] = {1, 1, 1};

  status = read_velocity(ncid, vars, vars[2], startv, countv, stridev, vnod);
  if (status != NC_NOERR) {
    printf("Error during lon var retrieval\n");
    return 1;
  }

  double start = MPI_Wtime();
  vnod_reduced = compute_maximum(0, vnod, &dimension);
  waste_time((long)TIME * DEPTH * NODE2 * 50);
  double end = MPI_Wtime();

  double tot = end - start;

  printf("proc,time\n");
  printf("%d, %f", 1, tot);

  MPI_Finalize();

  free(vnod_reduced);

  return 0;
}
