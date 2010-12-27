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

#define ALGO CMiller

int main(array<System::String ^> ^args)
{
	FileSrc fs(NBITS, FILE_PATTERN);
	
	PULONGLONG p;
	
  ALGO algo(NBITS);
	while (p = fs.Next() ) {
    Console::WriteLine("Function: " + fs.Name);
    algo.Synthesize(p);
  }
  
	return 0;
}

