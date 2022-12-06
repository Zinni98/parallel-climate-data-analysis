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
void print_2d_matrix(float **reduced_matrix, int num_elements, char *var_name);
int check_status(int *status, const char* var_name);
bool check_var(const char *options[NUM_VARS], const char *var_name);