// This is the main DLL file.

#include "stdafx.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "CudaTernary.h"
void SynthesizeKernel(const int *inp, const int *outp, int *target, int *control, int *operation, int size);

using namespace std;

namespace Ternary {
  namespace Cuda {

    bool Synthesizer::AddSequence(PINT pIn, PINT pOut)
    {
      CoreSequence *s = new CoreSequence(pIn, pOut, m_nBits);
      m_sequences.push_back(s);
      return true;
    }

    void Synthesizer::Synthesize()
    {
      // Allocate CUDA memory for input vectors
      int *pcuIn, *pcuOut, *pcuControl, *pcuTarget, *pcuOperation;

      m_nTerms = 10;
      int vectorSize = m_nTerms * sizeof(PINT);

      cudaMalloc( (void**)&pcuIn, vectorSize);
      cudaMalloc( (void**)&pcuOut, vectorSize);
      cudaMalloc( (void**)&pcuTarget, m_outputBlockSize);
      cudaMalloc( (void**)&pcuOperation, m_outputBlockSize);
      cudaMalloc( (void**)&pcuControl, m_outputBlockSize);

      m_pIn = new INT[m_nTerms * m_sequences.size()];
      m_pOut = new INT[m_nTerms * m_sequences.size()];

      // Copy input and output buffers to device
      for(int i=0; i<m_sequences.size(); i++) {
        CopyMemory(&m_pIn[i*m_nTerms],  m_sequences[i]->m_pIn, vectorSize);
        CopyMemory(&m_pOut[i*m_nTerms], m_sequences[i]->m_pOut, vectorSize);
      }

      cudaMemcpy(pcuIn, m_pIn, vectorSize, cudaMemcpyHostToDevice);
      cudaMemcpy(pcuOut, m_pOut, vectorSize, cudaMemcpyHostToDevice);

      SynthesizeKernel(pcuIn, pcuOut, pcuTarget, pcuControl, pcuOperation, 500);

      int *target = new int[500];
      cudaMemcpy(target, pcuTarget, 500*4, cudaMemcpyDeviceToHost);

      // Copy results to local buffer 
      cudaFree(pcuIn);
      cudaFree(pcuOut);
      cudaFree(pcuTarget);
      cudaFree(pcuOperation);
      cudaFree(pcuControl);
    }
  }
}
