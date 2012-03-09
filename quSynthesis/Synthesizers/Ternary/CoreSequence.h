#include <windows.h>
namespace Ternary {
  class CoreSequence {
  public:
    PINT m_pIn;
    PINT m_pOut;
    INT  m_nBits;
    PINT m_pControl;
    PINT m_pTarget;
    PINT m_pOperation;
    INT  m_nGates;

  public:
    CoreSequence(PINT pIn, PINT pOut, INT nBits)
    {
      m_pIn = pIn;
      m_pOut = pOut;
      m_nBits = nBits;
    }
  };
}