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
#define ALGO CoveredSetPartition::typeid
//#define ALGO Nouraddin::typeid
//#define ALGO Miller::typeid


int main(array<System::String ^> ^args)
{
  GAConductor algo(NBITS, ALGO);

	PULONGLONG p;
  FileSrc fs(NBITS, FILE_PATTERN);

	while (p = fs.Next() ) {
		Console::WriteLine("Function: " + fs.Name);
		algo.Synthesize(p);
	}
	
	return 0;
}

