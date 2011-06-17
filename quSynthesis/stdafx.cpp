// stdafx.cpp : source file that includes just the standard includes
// quSynthesis.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#define INCLUDE_GLOBALS
#include "stdafx.h"

HANDLE ghPrintMutex;
QuLogic::CQueue gQueue;
//QuLogic::CResult gResult;
int QuLogic::CHasse::m_nBits;
int QuLogic::CHasse::m_nTerms;
int QuLogic::CHasse::m_nBands;
bool QuLogic::CHasse::m_fInitialized;
bool QuLogic::CGlobals::m_fInitialized;
int QuLogic::CGlobals::m_BandSum[];
vector<int> *QuLogic::CHasse::m_pBands;
gcroot<String^> QuLogic::CGlobals::date;

