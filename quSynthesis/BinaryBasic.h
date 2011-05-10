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
    int diff = inTerm ^ outTerm;

    if (diff > 0) {
      // Flip the 0 bits first
      int mask = 1;
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



  int Propogate(int outTerm)
  {
    // Apply current list of gates..
    for (int i=0; i<m_nGates; i++) {
      int x = outTerm & m_pControl[i];
      if (x == m_pControl[i])
        outTerm ^= m_pTarget[i];
    }
    return outTerm;
  }  

  int QuantumCost()
  {
    PINT pnCount = new int[m_nBits];
    ZeroMemory(pnCount, m_nBits * sizeof(int));
    for (int i=0; i<m_nGates; i++) 
      pnCount[ControlLines(m_pControl[i])]++;

    int nCost=0;
    for (int i=0; i<m_nBits; i++)
      nCost += GateCost(i) * pnCount[i];

    return nCost;
  }


  ///   int GateCost(int i)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int GateCost(int i)
  {
    return Math::Max(1, (int)Math::Pow(2, 1 + i) - 3);
  }

  ///   int ControlLines(int n)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int ControlLines(int n)
  {
    int nCount=0;

    for (int i=0; i<8; i++) {
      nCount += NonZeroCount(n & 0xFF);
      n >>= 8;
    }
    return nCount;
  }

  void Resize()
  {
    PINT p=new int[m_nBufSize+2048]; 
    CopyMemory(p, m_pControl, m_nBufSize*sizeof(int));
    delete m_pControl;
    m_pControl= p;

    p=new int[m_nBufSize+2048]; 
    CopyMemory(p, m_pTarget, m_nBufSize*sizeof(int));
    delete m_pTarget;
    m_pTarget= p;

    m_nBufSize += 2048;
  }
};

