// quSynthesis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "FileSrc.h"
#include "QuAlgorithm.h"
#include "Miller.h"


HANDLE *gphMutex;    // Set of mutex objects to indicate when a Synthesizer finishes synthesis.

using namespace System;
using namespace QuLogic;

#define FILE_PATTERN "ankit*"
#define NBITS 8

// STEP(1): Add your algoirthm here
// STEP(2): See the Conductor Class for details..
#define ALGO CoveredSetPartition::typeid
//#define ALGO Nouraddin::typeid
//#define ALGO Miller::typeid

namespace QuLogic {
  void TotalReset()
  {
    if(BandBoundary) delete BandBoundary;
    BandBoundary=NULL;
    PartitionSize=0;
    CHasse::m_fInitialized = false;
    CGlobals::m_fInitialized = false;
    if(CHasse::m_pBands) CHasse::m_pBands->clear();
  }
}

int main(array<System::String ^> ^args)
{
  QuLogic::TotalReset();
  QuLogic::PartitionSize = 7;
  if (args->Length > 0) {
    QuLogic::PartitionSize = Convert::ToInt64(args[0]);
  }
  Console::WriteLine("PartitionSize: {0}", QuLogic::PartitionSize );


  PULONGLONG p;
  FileSrc fs(NBITS, FILE_PATTERN);

  while (p = fs.Next() ) {
    GAConductor *algo = new GAConductor(NBITS, fs.nTerms(), ALGO);
    Console::WriteLine("Function: " + fs.Name);
    algo->Synthesize(p);
    delete algo;
  }

  delete p;
  return 0;
}
