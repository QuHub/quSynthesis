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

#define FILE_PATTERN "function*"
#define NBITS 4

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
  QuLogic::PartitionSize = 0;
  if (args->Length > 0) {
    QuLogic::PartitionSize = Convert::ToInt64(args[0]);
  }
  Console::WriteLine("PartitionSize: {0}", QuLogic::PartitionSize );
  StreamReader ^file = gcnew StreamReader("parameters.txt");
  String ^line;
 line = file->ReadLine();
 int counter = 0;
  while( line = file->ReadLine())
  {
	  counter++;
	  array<String^>^ list = line->Split('	');
	  int genes = Convert::ToInt32(list[0]);
	  int run = Convert::ToInt32(list[1]);
	  double pm = Convert::ToDouble(list[2]);
	  double pc = Convert::ToDouble(list[3]);

	  GAConductor *algo = new GAConductor(NBITS, ALGO, genes, run, pm, pc);

	  PULONGLONG p;
	  FileSrc fs(NBITS, FILE_PATTERN);

	  while (p = fs.Next() ) {
		Console::WriteLine("Function: " + fs.Name);
		algo->Synthesize(p);
		algo->PrintResult(fs.Name, counter);
	  }
  
	  //delete algo;
  }
  return 0;
}
