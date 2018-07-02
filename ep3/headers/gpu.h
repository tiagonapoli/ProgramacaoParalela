#pragma once
#include "utils.h"
const int GPU_THREADS = 64;

pff gpu(ll n, int m, int k, float* sum, float* sum2, ll* new_n);

void gpu_tester();
