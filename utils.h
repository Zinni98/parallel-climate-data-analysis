#pragma once

// imports
#include <stdbool.h>

// constants
#define TIME 12
#define DEPTH 69
#define NODE2 500//8852366
#define NUM_VARS 3


int get_file_id(const char* filename);
void get_var_ids(int ncid, const char *var_names[NUM_VARS], int var_ids[], int count);
int read_velocity(int ncid, const char *options[NUM_VARS], const char* var_name , const int *start_idxs, const int *count_idxs, 
                const int *stride_steps, float target_buffer[TIME][DEPTH][NODE2]);
bool check_var(const char *options[NUM_VARS], const char *var_name);
int ceiling(int a, int b);
void pop_max_matrix(float buffer[TIME][NODE2], int rows, int cols);
void counts_and_displace(int *counts, int *displace, int comm_sz, int cols, char flag);
//void compute_max(int dim1, int dim2, int dim3, float local_velocity[dim3][dim1][dim2], float local_max[dim1][dim3]);