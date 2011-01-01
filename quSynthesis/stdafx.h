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

#define Print(x) 
extern HANDLE ghPrintMutex;

// TODO: reference additional headers your program requires here
#include <iostream>
#include <queue>
using namespace std;
using namespace System;
#include "Globals.h"
#include "Queue.h"
#include "Result.h"
#include "Synthesizer.h"
#include "Hasse.h"
#include "CoveredSetPartition.h"

extern QuLogic::CQueue gQueue;
extern QuLogic::CResult gResult;

#include "QuConductor.h"
#include "GAConductor.h"

