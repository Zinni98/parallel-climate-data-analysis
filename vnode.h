#pragma once

// imports
#include <stdbool.h>
#include "utils.h"


float** compute_maximum(const int var_idx, float (*matrix)[DEPTH][NODE2], int *dimension);
