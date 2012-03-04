#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "CudaSequence.h"

__global__ void cuSynthesizeKernel(CudaSequence *seq)
{
  int index = blockIdx.x;
  seq->m_cuTarget[index] = index;
  
  //for(int i=0; i < 10; i++) {
  //  target[index+i] = 0x20;
  //  control[index+i] = index*100;
  //}

}

void Process(int inTerm, int outTerm)
{
//  outTerm = Propogate(outTerm);

//  if (m_nGates > (m_nBufSize - THRESHOLD))  // make sure there is enough memory
//    Resize();

//  // Process low to high transitions 
//  for (int i=0; i < m_nBits; i++) 
//    ProcessBit(inTerm, outTerm, i, +1);

//  // Process high to low transitions 
//  for (int i=0; i < m_nBits; i++) 
//    ProcessBit(inTerm, outTerm, i, -1);
}

// Since each block executes on a core, we want to have a single thread per core
void SynthesizeKernel(const int *inp, const int *outp, int *target, int *control, int *operation, int size)
{
//  CudaSequence seq;

//  cuSynthesizeKernel<<<size, 1>>>(inp, outp, target, control, operation);
}

void SynthesizeKernel(CudaSequence &seq)
{
  int bufferSizeBytes = seq.m_nVectorSizeBytes * seq.m_nSequences;

  cudaMalloc( (void**)&seq.m_cuIn, bufferSizeBytes);
  cudaMalloc( (void**)&seq.m_cuOut, bufferSizeBytes);
  cudaMalloc( (void**)&seq.m_cuTarget, seq.m_outputBlockSize);
  cudaMalloc( (void**)&seq.m_cuOperation, seq.m_outputBlockSize);
  cudaMalloc( (void**)&seq.m_cuControl, seq.m_outputBlockSize);

  // Copy memory block to CUDA device
  cudaMemcpy(seq.m_cuIn, seq.m_pIn, bufferSizeBytes, cudaMemcpyHostToDevice);
  cudaMemcpy(seq.m_cuOut, seq.m_pOut, bufferSizeBytes, cudaMemcpyHostToDevice);

  cuSynthesizeKernel<<<1024, 1>>>(&seq);

  if(seq.m_nGates * sizeof(INT) > seq.m_outputBlockSize)
    throw "Exceeded limit of buffer";

  seq.m_pTarget = new INT[seq.m_nGates];
  seq.m_pControl = new INT[seq.m_nGates];
  seq.m_pOperation = new INT[seq.m_nGates];

  int size = seq.m_nGates * sizeof(INT);
  cudaMemcpy(seq.m_pTarget, seq.m_cuTarget, size, cudaMemcpyDeviceToHost);
  cudaMemcpy(seq.m_pControl, seq.m_cuControl, size, cudaMemcpyDeviceToHost);
  cudaMemcpy(seq.m_pOperation, seq.m_cuOperation, size, cudaMemcpyDeviceToHost);

  cudaFree(seq.m_cuIn);
  cudaFree(seq.m_cuOut);
  cudaFree(seq.m_cuTarget);
  cudaFree(seq.m_cuOperation);
  cudaFree(seq.m_cuControl);
}