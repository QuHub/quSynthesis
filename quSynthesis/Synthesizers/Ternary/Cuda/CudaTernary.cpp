// This is the main DLL file.

#include "stdafx.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "CudaTernary.h"
#include "CudaSequence.h"

void SynthesizeKernel(const int *inp, const int *outp, int *target, int *control, int *operation, int size);
void SynthesizeKernel(CudaSequence &seq);
void PrintIt();

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
      m_seq.m_nVectorSizeBytes =  m_nTerms * sizeof(INT);
      m_seq.m_nSequences = m_sequences.size();

      // Allocate memory for input and output for all m_sequences, each with m_nTerms
      int size = m_sequences.size();
      m_seq.m_nBits = m_nBits;

      m_seq.m_pIn = new INT[m_nTerms * m_sequences.size()];
      m_seq.m_pOut = new INT[m_nTerms * m_sequences.size()];
      m_seq.m_pTarget = new INT[m_seq.m_outputBlockSize];
      m_seq.m_pControl = new INT[m_seq.m_outputBlockSize];
      m_seq.m_pOperation = new INT[m_seq.m_outputBlockSize];

      // Copy input and output buffers to device
      for(int i=0; i<m_sequences.size(); i++) {
        CopyMemory(&m_seq.m_pIn[i*m_nTerms],  m_sequences[i]->m_pIn, m_seq.m_nVectorSizeBytes);
        CopyMemory(&m_seq.m_pOut[i*m_nTerms], m_sequences[i]->m_pOut, m_seq.m_nVectorSizeBytes);
      }
      SynthesizeKernel(m_seq);

      //for(int i=0; i<m_sequences.size(); i++) {
      //  CopyMemory(m_sequences[i]->m_pControl, &m_seq.m_pControl[i*m_seq.m_outputBlockSize],  m_seq.m_outputBlockSize);
      //  CopyMemory(m_sequences[i]->m_pTarget, &m_seq.m_pTarget[i*m_seq.m_outputBlockSize],  m_seq.m_outputBlockSize);
      //  CopyMemory(m_sequences[i]->m_pOperation, &m_seq.m_pOperation[i*m_seq.m_outputBlockSize],  m_seq.m_outputBlockSize);
      //}
    }
  }
}
