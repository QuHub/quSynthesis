#pragma once

#include <vcclr.h>

/*
http://webhome.cs.uvic.ca/~dmaslov/hwbs.html

Hidden weighted bit function hwbN has N inputs and N outputs. Its output equals its input shifted left on the number 
of positions equal to the number of ones in the input pattern. Thus, hwbN function is reversible. Hidden weighted bit 
function is known to have an exponential size BDD for any variable ordering. This type functions were first proposed as 
reversible benchmarks by Patel and Markov. It is known that hwbN functions may be implemented with polynomial 
quantum/reversible cost assuming a logarithmic number of garbage bits is available. It remains an open question 
if a polynomial cost reversible/quantum implementation exists if addition of garbage bits is not allowed.

For example, for a three variable sequence.
Original    HiddenWeight  Rotated By
Sequence     Sequence       #Trits
0	0	0					0	0	0           0
0	0	1					0	1	0           1
0	0	2					0	2	0           1
0	1	0					1	0	0           1
0	1	1					1	0	1           2
0	1	2					2	0	1           2
0	2	0					2	0	0
0	2	1					1	0	2
0	2	2					2	0	2
1	0	0					0	0	1
1	0	1					1	1	0
1	0	2					2	1	0
1	1	0					0	1	1
1	1	1					1	1	1
1	1	2					1	1	2
1	2	0					0	1	2
1	2	1					1	2	1
1	2	2					1	2	2
2	0	0					0	0	2
2	0	1					1	2	0
2	0	2					2	2	0
2	1	0					0	2	1
2	1	1					2	1	1
2	1	2					2	1	2
2	2	0					0	2	2
2	2	1					2	2	1
2	2	2					2	2	2
*/

ref class HiddenWeight
{
public:
  HiddenWeight()
  {
  }

  int Generate(int nBits)
  {
    char szName[256];
    char szFile[256];
    char szFile2[256];
    sprintf(szName, "hwt%d", nBits);
    sprintf(szFile, "hwt%d.txt", nBits);
    sprintf(szFile2, "hwt%d-column.txt", nBits);

    vector<int> *p = Utility::TernarySequence(nBits);

    fstream f(szFile, fstream::out);
    fstream f2(szFile2, fstream::out);
    f << szName << " ";
    f2 << szName << "\nPartitioned - Decimal\n";

    for (int i=0; i<(int)p->size(); i++) {
      int count = 2 * Utility::NonZeros(p->at(i));    // 2 bits per trit
      int high, low, origin;
      origin = low = high = p->at(i);
      low  <<= count;
      high >>= 2 * nBits - count;
      
      p->at(i) = (low + high) & (int)(pow(2.0, 2*nBits)-1);
      f2 << setw(6) << i << ": " <<  Utility::ToString(p->at(i)).c_str() << " " << Utility::ToDecimal(p->at(i)) << "\n";
      f << Utility::ToDecimal(p->at(i)) << " ";
    }

    f.close();
    f2.close();
    return 0;
  }
};

