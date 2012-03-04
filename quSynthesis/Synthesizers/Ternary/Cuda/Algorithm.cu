#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void cuSynthesizeKernel(const int *inp, const int *outp, int *target, int *control, int *operation)
{
  int index = blockIdx.x;
  target[index] = index;
  
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
  cuSynthesizeKernel<<<size, 1>>>(inp, outp, target, control, operation);
}