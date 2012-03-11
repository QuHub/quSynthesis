#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "CudaSequence.h"
#include "stdio.h"
#include "cuda_debug.h"


__global__ void cuSynthesizeKernel(CudaSequence *data);
void InitializeConstants();

void SynthesizeKernel(CudaSequence &seq)
{
  int bufferSizeBytes = seq.m_nVectorSizeBytes * seq.m_nSequences;
  CudaSequence *pcuSeq;

  // This is essential for Parallel Nsight debugging, since GPU1 is used to debug the
  // code, while GPU0 is used for the display.
  cudaSetDevice(1);  

  int *ptr;
  CS( cudaMalloc( (void**)&pcuSeq, sizeof(CudaSequence)) );
  CS( cudaMalloc( (void**)&ptr, bufferSizeBytes) );
  CS( cudaMalloc( (void**)&seq.m_cuIn, bufferSizeBytes) );
  CS( cudaMalloc( (void**)&seq.m_cuOut, bufferSizeBytes) );
  CS( cudaMalloc( (void**)&seq.m_cuTarget, seq.m_outputBlockSize) );
  CS( cudaMalloc( (void**)&seq.m_cuOperation, seq.m_outputBlockSize) );
  CS( cudaMalloc( (void**)&seq.m_cuControl, seq.m_outputBlockSize) );
  CS( cudaMalloc( (void**)&seq.m_cuGates, seq.m_nSequences * sizeof(INT)) );

  // Copy memory block to CUDA device
  CS( cudaMemcpy(seq.m_cuIn, seq.m_pIn, bufferSizeBytes, cudaMemcpyHostToDevice) );
  CS( cudaMemcpy(seq.m_cuOut, seq.m_pOut, bufferSizeBytes, cudaMemcpyHostToDevice) );
  CS( cudaMemcpy(pcuSeq, &seq, sizeof(seq), cudaMemcpyHostToDevice) );

  InitializeConstants();
  cuSynthesizeKernel<<<1, 1>>>(pcuSeq);
  
  //// make the host block until the device is finished with foo
  //cudaThreadSynchronize();

  // check for error
  cudaError_t error = cudaGetLastError();
  if(error != cudaSuccess)
  {
    // print the CUDA error message and exit
    printf("My CUDA error: %s\n", cudaGetErrorString(error));
  }

  cudaMemcpy(seq.m_pTarget, seq.m_cuTarget, seq.m_outputBlockSize, cudaMemcpyDeviceToHost);
  cudaMemcpy(seq.m_pControl, seq.m_cuControl, seq.m_outputBlockSize, cudaMemcpyDeviceToHost);
  cudaMemcpy(seq.m_pOperation, seq.m_cuOperation, seq.m_outputBlockSize, cudaMemcpyDeviceToHost);
  cudaMemcpy(seq.m_pnGates, seq.m_cuGates, seq.m_nSequences * sizeof(INT), cudaMemcpyDeviceToHost);

  // TODO: free up device memory
  cudaFree(seq.m_cuIn);
  cudaFree(seq.m_cuOut);
  cudaFree(seq.m_cuTarget);
  cudaFree(seq.m_cuOperation);
  cudaFree(seq.m_cuControl);
  cudaFree(seq.m_cuGates);
}