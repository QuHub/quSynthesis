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

#define FILE_PATTERN "Ternary\\counter12_"
#define NBITS        2
#define RADIX        3    // 2: Binary, 3: Ternary
#define RADIXBITS    2    // 1: Binary, 2: Ternary
// STEP(1): Add your algoirthm here
// STEP(2): See the Conductor Class for details..
//#define ALGO CoveredSetPartition::typeid
//#define ALGO Nouraddin::typeid
//#define ALGO Miller::typeid

#define TERNARY
//#define NATURAL

//******* Conductors
//#define PASSTHROUGH_CONDUCTOR
#define GA

#if defined(TERNARY) && defined(GA)
#  define ALGO Ternary::Algorithm::OrderedSet::typeid
#  define CONDUCTOR GAConductor
#elif defined(TERNARY) && defined(PASSTHROUGH_CONDUCTOR)
#  define ALGO Ternary::Algorithm::NaturalOrder::typeid
#  define CONDUCTOR PassThrough
#elif defined(TERNARY) && defined(NATURAL)
#  define ALGO Ternary::Algorithm::NaturalOrder::typeid
#  define CONDUCTOR QuConductor
#endif

// <summary>
// 
namespace QuLogic {
  void TotalReset()
  {
    if(BandBoundary) delete BandBoundary;
    BandBoundary=NULL;
    CHasse::m_fInitialized = false;
    CGlobals::m_fInitialized = false;
    CGlobals::date = DateTime::Now.ToString("yyyyMMdd");
    if(CHasse::m_pBands) CHasse::m_pBands->clear();
  }
}


// <summary>
// 
// <inputs>
//
// <outputs>
int ternary(array<System::String ^> ^args)
{
  for (int i=3; i<=8; i++) {
    Config::SetTernary();
    Config::nBits = i;
    QuLogic::TotalReset();
    
    CONDUCTOR *algo = new CONDUCTOR(Config::nBits, ALGO);

    PINT p;
    FileSrc fs(Config::nBits, FILE_PATTERN + Convert::ToString(i));

    while (p = fs.Next() ) {
      Console::WriteLine("Function: " + fs.Name);
      algo->FunctionName = fs.Name;
      algo->Synthesize(p);
    }

    delete p;
    delete algo;
  }
  return 0;
}

// <summary>
// 
// <inputs>
//
// <outputs>
int binary(array<System::String ^> ^args)
{
  QuLogic::TotalReset();
  Config::SetBinary();
  Config::PartitionSize = 5;
  if (args->Length > 0) {
    Config::PartitionSize = (int)Convert::ToInt64(args[0]);
  }
  Console::WriteLine("PartitionSize: {0}", Config::PartitionSize );

  GAConductor *algo = new GAConductor(NBITS, ALGO);

  PINT p;
  FileSrc fs(NBITS, FILE_PATTERN);

  while (p = fs.Next() ) {
    Console::WriteLine("Function: " + fs.Name);
    algo->Synthesize(p);
  }

  delete p;
  delete algo;
  return 0;
}


int main(array<System::String ^> ^args)
{
  ternary(args);
}
