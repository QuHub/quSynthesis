#pragma once
// Ternary Gates
static int m_Op[5][3]= 
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
static int gOp[3][3] =
                     {
          // Output    0, 1, 2     Input 
                      {4, 2, 3}, // 0
                      {2, 3, 4}, // 1
                      {3, 4, 2}  // 2
                      };

// Bitmask two bits at a time for ternary operations.
static int gBitMask[] = {3, 3<<2, 3<<4, 3<<6, 3<<8, 3<<10, 3<<12, 3<<14, 3<<16};

// Bring down the ith trit to the lower two LSB bits.
#define BIT(x,i) ((x & gBitMask[i]) >> 2*i)

class TernaryBasic : public QuSynthesizer
{
public:
  PULONGLONG m_pMask;     // Array holding target bit value
	
  TernaryBasic(void) : QuSynthesizer()
  {
  }


  void Process(int inTerm, int outTerm)
  {
    outTerm = Propogate(outTerm);

    if (m_nGates > m_nBufSize - 50)  // make sure there is enough memory
      Resize();

    // Process low to high transitions 
    for (int i=0; i < m_nBits; i++) 
      ProcessBit(inTerm, outTerm, i, +1);

    // Process high to low transitions 
    for (int i=0; i < m_nBits; i++) 
      ProcessBit(inTerm, outTerm, i, -1);
  }

  
  ///   inline void ProcessBit(QuTerm t1, QuTerm t2, int i)
  ///
  ///
  /// Inputs:
  ///     type: > 0: Flip Up    0->1, 0->2, 1->2
  ///           < 0: Flip Down  1->0, 2->0, 2->1
  /// Outputs:
  ///
  void ProcessBit(int inTerm, int &outTerm, int i, int type)
  {
    // Isloate bit (i) for processing
    int inBit  = (gBitMask[i] & inTerm);        // must be defined as signed int
    int outBit = (gBitMask[i] & outTerm);
    
    if ( type * (inBit - outBit) > 0) {         // Difference? Yes!
      m_pTarget   [m_nGates] = i;                           // Save index of target bits
      m_pControl  [m_nGates] = ~gBitMask[i] & outTerm;      // For now, it is everything except target bits is a control bit
      m_pOperation[m_nGates++] = gOp[BIT(inTerm,i)][BIT(outTerm,i)];  // Find the appropriate operation. 
      outTerm = (~gBitMask[i] & outTerm) | (gBitMask[i] & inTerm);
    }
  }


  int Propogate(int outTerm)
  {
    // Apply current list of gates..
    for (ULONGLONG i=0; i<m_nGates; i++) {
	    int mask = gBitMask[m_pTarget[i]];
	    if ( m_pControl[i] == (~mask & outTerm) ) {                 // Control Bits for gate matches All bits in output excluding target bits.
	        int val = (mask & outTerm) >> 2*m_pTarget[i];           // Bring target bits to lower two bits.
          val = (m_Op[m_pOperation[i]][val] << 2*m_pTarget[i]);   // Apply operation on bits.
	        outTerm = (~mask & outTerm) | val;
	    }
    }

    return outTerm;
  }  

  int QuantumCost()
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
  int GateCost(int i)
  {
    return 0;//Math::Max(1, (int)Math::Pow(2, 1 + i) - 3);
  }

  ///   ULONGLONG ControlLines(ULONGLONG n)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int ControlLines(ULONGLONG n)
  {
    ULONGLONG nCount=0;

    //for (int i=0; i<8; i++) {
    //  nCount += NonZeroCount(n & 0xFF);
    //  n >>= 8;
    //}
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

