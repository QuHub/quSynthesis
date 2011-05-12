// Verify.cpp : main project file.

#include "stdafx.h"
#include <vector>
#include "result.h"

using namespace System;
using namespace std;

int Propogate(vector<int> control, vector<int> target, vector<int> operation, int outTerm);

int main(array<System::String ^> ^args)
{
  if (args->Length == 0) {
    Console::WriteLine("Usage: Verify filename");
    exit(0);
  }

  CResult r(args[0]);

  for(int i=0; i<r.input.size(); i++) {
    if(r.input[i] != Propogate(r.control, r.target, r.operation, r.output[i])) {
      Console::WriteLine ("* DO NOT MATCH ******* {0}: [{1}:{2}]********** ", i, r.input[i], r.output[i]);
      return 0;
    }
  }

  Console::WriteLine("Well!  They appear to match");

  return 0;
}


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

int Propogate(vector<int> control, vector<int> target, vector<int> operation, int outTerm)
{
    // Apply current list of gates..
    for (int i=0; i<target.size(); i++) {
      //cout << "Gate: [C:T:Op] [" << m_pControl[i] << " " << m_pTarget[i] << " " << m_pOperation[i] << endl;
	    int mask = gBitMask[target[i]];
	    if ( control[i] == (~mask & outTerm) ) {                 // Control Bits for gate matches All bits in output excluding target bits.
	        int val = (mask & outTerm) >> 2*target[i];           // Bring target bits to lower two bits.
          val = (m_Op[operation[i]][val] << 2*target[i]);   // Apply operation on bits.
	        outTerm = (~mask & outTerm) | val;
	    }
    }

    return outTerm;
}
