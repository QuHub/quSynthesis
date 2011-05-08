#pragma once
class BinaryBasic : public QuSynthesizer
{
public:

  BinaryBasic(void) : QuSynthesizer()
  {

  }



  void Process(int inTerm, int outTerm)
  {
    outTerm = Propogate(outTerm);

    // Do we still have a difference?
    // TODO: redo this thing with hash tables
    ULONGLONG diff = inTerm ^ outTerm;

    if (diff > 0) {
      // Flip the 0 bits first
      ULONGLONG mask = 1;
      for (int j = 0; j< m_nBits; j++) {
        if ( (diff & mask) && !(outTerm & mask)) {
          if (m_nGates > m_nBufSize - 8)  // make sure there is enough memory
            Resize();

          m_pControl[m_nGates] = outTerm & ~mask;		// clear inTerm bit at current bit position
          m_pTarget[m_nGates++] = mask;						  // Mark bit position for as NOT gate
          diff &= ~mask;								            // reset bit in diff, since it should be same in outTerm
          outTerm ^= mask;
        }
        mask <<= 1;
      }

      mask = 1;
      // Flip the 1 bits second
      for (int j = 0; j< m_nBits; j++) {
        if ( (diff & mask) && (outTerm & mask)) {
          if (m_nGates > m_nBufSize - 8)  // make sure there is enough memory
            Resize();

          m_pControl[m_nGates] = outTerm & ~mask;		// clear inTerm bit at current bit position
          m_pTarget[m_nGates++] = mask;						// Mark bit position for as NOT gate
          diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
          outTerm ^= mask;
        }
        mask <<= 1;
      }
    }	
    //Target = m_pTarget;
    //cout << "m_nGates:" << m_nGates << endl;
  }



  ULONGLONG Propogate(ULONGLONG outTerm)
  {
    // Apply current list of gates..
    for (ULONGLONG i=0; i<m_nGates; i++) {
      ULONGLONG x = outTerm & m_pControl[i];
      if (x == m_pControl[i])
        outTerm ^= m_pTarget[i];
    }
    return outTerm;
  }  

  ULONGLONG QuantumCost()
  {
    PULONGLONG pnCount = new ULONGLONG[m_nBits];
    ZeroMemory(pnCount, m_nBits * sizeof(ULONGLONG));
    for (int i=0; i<m_nGates; i++) 
      pnCount[ControlLines(m_pControl[i])]++;

    ULONGLONG nCost=0;
    for (int i=0; i<m_nBits; i++)
      nCost += GateCost(i) * pnCount[i];

    return nCost;
  }


  ///   ULONGLONG GateCost(int i)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  ULONGLONG GateCost(int i)
  {
    return Math::Max(1, (int)Math::Pow(2, 1 + i) - 3);
  }

  ///   ULONGLONG ControlLines(ULONGLONG n)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  ULONGLONG ControlLines(ULONGLONG n)
  {
    ULONGLONG nCount=0;

    for (int i=0; i<8; i++) {
      nCount += NonZeroCount(n & 0xFF);
      n >>= 8;
    }
    return nCount;
  }

  void Resize()
  {
    PULONGLONG p=new ULONGLONG[m_nBufSize+2048]; 
    CopyMemory(p, m_pControl, m_nBufSize*sizeof(ULONGLONG));
    delete m_pControl;
    m_pControl= p;

    p=new ULONGLONG[m_nBufSize+2048]; 
    CopyMemory(p, m_pTarget, m_nBufSize*sizeof(ULONGLONG));
    delete m_pTarget;
    m_pTarget= p;

    m_nBufSize += 2048;
  }
};

