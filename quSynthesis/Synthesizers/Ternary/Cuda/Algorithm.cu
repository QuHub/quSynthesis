#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "CudaSequence.h"

#include "stdio.h"

// Ternary Gates
int gTernaryOps[5][3]= 
{
  {2, 0, 1},      // 0: -1
  {1, 2, 0},      // 1: -2
  {1, 0, 2},      // 2: 01
  {2, 1, 0},      // 3: 02
  {0, 2, 1}       // 4: 12
};                

// Operation to use based on [input][output] values which are an index to the gates in the m_Op array above
// Example: 
//    Input = 2, Output = 1 => Gate 4 (From m_Op above would be Swap gate 12)
int gOpMap[3][3] =
{
  // Output    0, 1, 2     Input 
  {4, 2, 3}, // 0
  {2, 3, 4}, // 1
  {3, 4, 2}  // 2
};

// Bitmask two bits at a time for ternary operations.
int gBitMask[] = {3, 3<<2, 3<<4, 3<<6, 3<<8, 3<<10, 3<<12, 3<<14, 3<<16}; 

__device__ int *gcuBitMask;
__device__ int *gcuOpMap;
__device__ int *gcuTernaryOps;

__device__ void Process(int inTerm, int outTerm, int nBits, PINT gBitMask, PINT pTarget, PINT pControl, PINT pOperation);

void cudasafe( cudaError_t error, char* file, int line)
{
  error = cudaGetLastError();
   if(error!=cudaSuccess) { 
     fprintf(stderr,"ERROR: File: %s(%d) : %s(%i)\n",file, line, cudaGetErrorString(error), error);  
   }
}

#define CS(x) cudasafe(x, __FILE__, __LINE__)

__global__ void cuSynthesizeKernel(CudaSequence *data)
{
  CudaSequence seq = data[0];
  int index = blockIdx.x * seq.m_outputBlockSize;
  printf("nTerms: %d", seq.m_nTerms);

  for(int i=0; i<seq.m_nTerms; i++) {
    Process(seq.m_cuIn[index+i], 
            seq.m_cuOut[index+i], 
            seq.m_nBits,
            &seq.m_cuGates[index],
            &seq.m_cuTarget[index], 
            &seq.m_cuControl[index],
            &seq.m_cuOperation[index]
    );
  }

  //for(int i=0; i < 10; i++) {
  //  target[index+i] = 0x20;
  //  control[index+i] = index*100;
  //}

}

__device__ int Propagate(int outTerm);

#define BIT(x,i) ((x & gcuBitMask[i]) >> 2*i)
__device__ void Process(int inTerm, int outTerm, int nBits, PINT pnGates, PINT pTarget, PINT pControl, PINT pOperation)
{
  printf("I am in cuda");
  return;

  outTerm = Propagate(outTerm);

  // Process low to high transitions 
  //for(int dir=1; dir>-2; dir-=2) {
  //  for (int i=0; i < nBits; i++) {
  //    // Isloate bit (i) for processing
  //    int inBit  = (gcuBitMask[i] & inTerm);        // must be defined as signed int
  //    int outBit = (gcuBitMask[i] & outTerm);

  //    if ( dir * (inBit - outBit) > 0) {         // Difference? Yes!
  //      pTarget   [*pnGates] = i;                           // Save index of target bits
  //      pControl  [*pnGates] = ~gcuBitMask[i] & outTerm;      // For now, it is everything except target bits is a control bit
  //      pOperation[*pnGates++] = gcuOpMap[BIT(inTerm,i)][BIT(outTerm,i)];  // Find the appropriate operation. 
  //      outTerm = (~gcuBitMask[i] & outTerm) | (gcuBitMask[i] & inTerm);
  //    }
  //  }
  //}
}


__device__ int Propagate(int outTerm)
{

  return 0;
}

void InitializeConstants()
{
  CS( cudaMalloc((void**)&gcuBitMask, sizeof(gBitMask)) );
  CS( cudaMalloc((void**)&gcuTernaryOps, sizeof(gTernaryOps)) );
  CS( cudaMalloc((void**)&gcuOpMap, sizeof(gOpMap)) );
  CS( cudaMemcpy(gcuBitMask, gBitMask, sizeof(gBitMask), cudaMemcpyHostToDevice) );
  CS( cudaMemcpy(gcuTernaryOps, gTernaryOps, sizeof(gTernaryOps), cudaMemcpyHostToDevice) );
  CS( cudaMemcpy(gcuOpMap, gOpMap, sizeof(gOpMap), cudaMemcpyHostToDevice) );
}


__global__ void cuPrintIt()
{
  // This requires setting the comupte mode 2.0 in the CUDA flags of the project configuration.
  // Properties -> CUDA C++ -> Device -> Code Generation= compute_20,sm_20 
  printf("What up: %d\n", blockIdx.x);
}

void PrintIt()
{

  cuPrintIt<<<1024,1>>>();
  cudaDeviceReset();
}


void SynthesizeKernel(CudaSequence &seq)
{
  int bufferSizeBytes = seq.m_nVectorSizeBytes * seq.m_nSequences;
  CudaSequence *pcuSeq;

  InitializeConstants();

  // This is essential for Parallel Nsight debugging, since GPU1 is used to debug the
  // code, while GPU0 is used for the display.
  cudaSetDevice(1);  

  int *ptr;
  CS( cudaMalloc( (void**)&ptr, bufferSizeBytes) );
  CS( cudaMalloc( (void**)&seq.m_cuIn, bufferSizeBytes) );
  CS( cudaMalloc( (void**)&seq.m_cuOut, bufferSizeBytes) );
  CS( cudaMalloc( (void**)&seq.m_cuTarget, seq.m_outputBlockSize) );
  CS( cudaMalloc( (void**)&seq.m_cuOperation, seq.m_outputBlockSize) );
  CS( cudaMalloc( (void**)&seq.m_cuControl, seq.m_outputBlockSize) );
  CS( cudaMalloc( (void**)&seq.m_pnGates, seq.m_nSequences * sizeof(INT)) );
  CS( cudaMalloc( (void**)&pcuSeq, sizeof(CudaSequence)) );

  // Copy memory block to CUDA device
  CS( cudaMemcpy(seq.m_cuIn, seq.m_pIn, bufferSizeBytes, cudaMemcpyHostToDevice) );
  CS( cudaMemcpy(seq.m_cuOut, seq.m_pOut, bufferSizeBytes, cudaMemcpyHostToDevice) );
  CS( cudaMemcpy(pcuSeq, &seq, sizeof(seq), cudaMemcpyHostToDevice) );

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

  cudaFree(seq.m_cuIn);
  cudaFree(seq.m_cuOut);
  cudaFree(seq.m_cuTarget);
  cudaFree(seq.m_cuOperation);
  cudaFree(seq.m_cuControl);
  cudaFree(seq.m_cuGates);
}