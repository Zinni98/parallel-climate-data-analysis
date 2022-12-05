#pragma once

// imports
#include <stdbool.h>

// constants
#define TIME 12
#define DEPTH 69
#define NODE2 8852366
#define NUM_VARS 3


int get_file_id(const char* filename);
void get_var_ids(int ncid, const char *var_names[NUM_VARS], int var_ids[], int count);
int read_velocity(int ncid, const char *options[NUM_VARS], const char* var_name , const int *start_idxs, const int *count_idxs, 
                const int *stride_steps, float target_buffer[TIME][DEPTH][NODE2]);
bool check_var(const char *options[NUM_VARS], const char *var_name);