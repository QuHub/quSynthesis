// This is the main DLL file.

#include "stdafx.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "CudaTernary.h"
#include "CudaSequence.h"

void SynthesizeKernel(const int *inp, const int *outp, int *target, int *control, int *operation, int size);
void SynthesizeKernel(CudaSequence &seq);

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

      CudaSequence seq;
      seq.m_pIn = m_pIn;
      seq.m_pOut = m_pOut;
      seq.m_nVectorSizeBytes =  m_nTerms * sizeof(INT);
      seq.m_nSequences = m_sequences.size();

      // Allocate memory for input and output for all sequences, each with m_nTerms
      m_pIn = new INT[m_nTerms * m_sequences.size()];
      m_pOut = new INT[m_nTerms * m_sequences.size()];

      // Copy input and output buffers to device
      for(int i=0; i<m_sequences.size(); i++) {
        CopyMemory(&m_pIn[i*m_nTerms],  m_sequences[i]->m_pIn, seq.m_nVectorSizeBytes);
        CopyMemory(&m_pOut[i*m_nTerms], m_sequences[i]->m_pOut, seq.m_nVectorSizeBytes);
      }
      SynthesizeKernel(seq);
    }
  }
}
