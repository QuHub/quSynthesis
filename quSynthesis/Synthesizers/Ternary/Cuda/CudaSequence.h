#include <windows.h>
#pragma once

typedef struct  
{
  PINT m_pIn, m_pOut, m_pTarget, m_pControl, m_pOperation;
  PINT m_cuIn, m_cuOut, m_cuTarget, m_cuControl, m_cuOperation;
  PINT m_pnGates, m_cuGates;
  PINT m_pgBitMask;
  PINT m_pgOpMapi[3], m_pgTernaryOps[3];
  INT m_nTerms, m_nBits;
  INT m_outputBlockSize;
  INT m_nSequences;
  INT m_nVectorSizeBytes;
} CudaSequence;