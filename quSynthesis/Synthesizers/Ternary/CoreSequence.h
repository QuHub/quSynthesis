#include <windows.h>
namespace Ternary {
  class CoreSequence {
  public:
    PINT m_pIn;
    PINT m_pOut;
    INT  m_nBits;
    PINT m_pControl;
    PINT m_pTarget;
    PINT m_pGates;
    INT  m_nGates;

  public:
    CoreSequence(PINT pInt, PINT pOut, INT nBits)
    {
      m_pIn = new INT[100];
      m_pOut = new INT[100];
    }
  };
}