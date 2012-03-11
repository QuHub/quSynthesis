
#include "cuda_debug.h"

void cudasafe( cudaError_t error, char* file, int line)
{
  error = cudaGetLastError();
   if(error!=cudaSuccess) { 
     fprintf(stderr,"ERROR: File: %s(%d) : %s(%i)\n",file, line, cudaGetErrorString(error), error);  
   }
}
