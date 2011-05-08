// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#include "targetver.h"
#include <math.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include "Thread.h"
#include <string>

#define Print(x) 
extern HANDLE ghPrintMutex;

// TODO: reference additional headers your program requires here
#include <iostream>
#include <fstream>
#include <queue>
using namespace std;
using namespace System;
using namespace System::IO;


namespace QuLogic
{
  extern int *BandBoundary;
  extern int PartitionSize;
  extern int nBits;
  extern int Radix;
  extern int RadixBits;
  extern int RadixMask; 
  extern string HomeFolder;
  extern string Synthesizer;
}


#include "Rand.h"
#include "Globals.h"
#include "QuSynthesizer.h"
#include "QuAlgorithm.h"
#include "Queue.h"
#include "Synthesizer.h"
#include "Hasse.h"
#include "GaParam.h"

extern QuLogic::CQueue gQueue;

// Synthesizers
#include "TernaryBasic.h"


// Algorithms go here
#include "CoveredSetPartition.h"
#include "TernaryOrderedSet.h"

#include "QuConductor.h"
#include "GAConductor.h"

