// quSynthesis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "FileSrc.h"
#include "QuAlgorithm.h"
#include "Miller.h"


namespace QuLogic
{
  int *BandBoundary=NULL;
  int PartitionSize=3;
  int nBits;
  int Radix=2;
  int RadixBits=1;
  int RadixMask=1;      // pow(2,RadixBits) - 1
  string HomeFolder = "..\\";
  string Synthesizer = "TernaryBasic";
}


HANDLE *gphMutex;    // Set of mutex objects to indicate when a Synthesizer finishes synthesis.

using namespace System;
using namespace QuLogic;

#define FILE_PATTERN "Ternary\\hwt2"
#define NBITS 2
#define RADIX        3    // 2: Binary, 3: Ternary
#define RADIXBITS    2    // 1: Binary, 2: Ternary
// STEP(1): Add your algoirthm here
// STEP(2): See the Conductor Class for details..
//#define ALGO CoveredSetPartition::typeid
//#define ALGO Nouraddin::typeid
//#define ALGO Miller::typeid

#define ALGO TernaryOrderedSet::typeid

namespace QuLogic {
  void TotalReset()
  {
    if(BandBoundary) delete BandBoundary;
    BandBoundary=NULL;
    PartitionSize=0;
    CHasse::m_fInitialized = false;
    CGlobals::m_fInitialized = false;
    if(CHasse::m_pBands) CHasse::m_pBands->clear();
    QuLogic::nBits = NBITS;
    QuLogic::Radix = RADIX;
    QuLogic::RadixBits = RADIXBITS;
    QuLogic::RadixMask = (int)(pow(2.0,RADIXBITS) - 1);
  }
}


int main(array<System::String ^> ^args)
{
  QuLogic::TotalReset();

  GAConductor *algo = new GAConductor(NBITS, ALGO);

  PULONGLONG p;
  FileSrc fs(NBITS, FILE_PATTERN);

  while (p = fs.Next() ) {
    Console::WriteLine("Function: " + fs.Name);
    algo->Synthesize(p);
  }

  delete p;
  delete algo;
  return 0;



}


int binary(array<System::String ^> ^args)
{
  QuLogic::TotalReset();
  QuLogic::PartitionSize = 5;
  if (args->Length > 0) {
    QuLogic::PartitionSize = (int)Convert::ToInt64(args[0]);
  }
  Console::WriteLine("PartitionSize: {0}", QuLogic::PartitionSize );

  GAConductor *algo = new GAConductor(NBITS, ALGO);

  PULONGLONG p;
  FileSrc fs(NBITS, FILE_PATTERN);

  while (p = fs.Next() ) {
    Console::WriteLine("Function: " + fs.Name);
    algo->Synthesize(p);
  }

  delete p;
  delete algo;
  return 0;
}
