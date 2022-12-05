#pragma once

// imports
#include <stdbool.h>

// constants
#define TIME 12
#define DEPTH 69
#define NODE2 8852366
#define NUM_VARS 3


float** compute_maximum(const int var_idx, float (*matrix)[DEPTH][NODE2], int *dimension);
