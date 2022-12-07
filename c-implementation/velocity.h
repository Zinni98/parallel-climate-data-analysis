#include "utils.h"


int read_velocity(int ncid, const char *options[NUM_VARS], const char* var_name , const int *start_idxs, const int *count_idxs, 
                const int *stride_steps, float target_buffer[TIME][DEPTH][NODE2]);

float** compute_maximum(const int var_idx, float (*matrix)[DEPTH][NODE2], int *dimension);

void compute_norm(float vnode_mat[TIME][DEPTH][NODE2], float unode_mat[TIME][DEPTH][NODE2], float target_matrix[TIME][DEPTH][NODE2]);