#include <windows.h>
typedef struct  
{
  PINT m_pIn, m_pOut, m_pTarget, m_pControl, m_pOperation;
  PINT m_cuIn, m_cuOut, m_cuTarget, m_cuControl, m_cuOperation;
  INT m_nTerms;
  INT m_nGates;
  INT m_outputBlockSize;
  INT m_nSequences;
  INT m_nVectorSizeBytes;
} CudaSequence;