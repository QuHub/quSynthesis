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
 
#include "Config.h"

#include "Rand.h"
#include "Globals.h"
#include "QuSynthesizer.h"
#include "QuAlgorithm.h"
#include "Queue.h"
#include "Synthesizer.h"
#include "Hasse.h"
#include "GaParam.h"
#include "StopWatch.h"

extern QuLogic::CQueue gQueue;

// Synthesizers
#include "Ternary/Synthesizer/Basic.h"


// Algorithms go here
#include "Ternary/algorithm.h"
#include "CoveredSetPartition.h"
#include "Ternary/Algorithm/OrderedSet.h"
#include "Ternary/Algorithm/NaturalOrder.h"

#include "QuConductor.h"
#include "GAConductor.h"
#include "PassThrough.h"

