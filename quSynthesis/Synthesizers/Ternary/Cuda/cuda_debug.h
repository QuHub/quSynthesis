#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "stdio.h"

#define CS(x) cudasafe(x, __FILE__, __LINE__)
void cudasafe( cudaError_t error, char* file, int line);