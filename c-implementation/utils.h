#pragma once

// imports
#include <stdbool.h>

// constants
#define TIME 12
#define DEPTH 69
#define NODE2 8852364
#define NUM_VARS 3

#define PATH2UNODE "/shares/HPC4DataScience/FESOM2/unod.fesom.2010.nc"
#define PATH2VNODE "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc"


int get_file_id(const char* filename);
void get_var_ids(int ncid, const char *var_names[NUM_VARS], int var_ids[], int count);
int read_velocity(int ncid, const char *options[NUM_VARS], const char* var_name , const int *start_idxs, const int *count_idxs, 
                const int *stride_steps, float target_buffer[TIME][DEPTH][NODE2]);
bool check_var(const char *options[NUM_VARS], const char *var_name);
int check_status(int *status, const char* var_name);
void pop_max_matrix(float buffer[TIME][NODE2], int rows, int cols);
void counts_and_displace(int *counts, int *displace, int comm_sz, int cols, char flag);
//void compute_max(int dim1, int dim2, int dim3, float local_velocity[dim3][dim1][dim2], float local_max[dim1][dim3]);

float** compute_maximum(const int var_idx, float (*matrix)[DEPTH][NODE2], int *dimension);

void compute_norm(float vnode_mat[TIME][DEPTH][NODE2], float unode_mat[TIME][DEPTH][NODE2], float target_matrix[TIME][DEPTH][NODE2]);

void compute_inverted_max(int dim1, int dim2, int dim3, float local_velocity[dim3][dim1][dim2], float local_max[dim3][dim1]);