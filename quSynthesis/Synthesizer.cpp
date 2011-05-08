#include "StdAfx.h"
#include "Synthesizer.h"
#include <math.h>
using namespace System;

namespace QuLogic {
	CSynthesizer::CSynthesizer(void) 
	{
	}

	DWORD CSynthesizer::Run( LPVOID arg )
	{
		m_hMutex = (HANDLE)arg;
    
    QuSynthesizer *qSyn;
    if (QuLogic::Synthesizer.compare("TernaryBasic") == 0) qSyn = new TernaryBasic();

		while(1) {
			// Look for something in the queue and synthesize it
			Lock();
			QuAlgorithm *qa = gQueue.Pop();
			if (!qa) {
				Release();
				Sleep(1);
				Print("Nothing to do") ;
				continue;
			}

			PULONGLONG pIn = qa->m_pIn;
			PULONGLONG pOut = qa->m_pOut;

      qSyn->Before(qa->m_nBits);

			for (int i=0; i < qa->m_nTerms; i++)
				qSyn->Process(*pIn++, *pOut++);

			qa->m_pTarget  = qSyn->m_pTarget;
			qa->m_pControl = qSyn->m_pControl;
	 
			qa->m_QuantumCost = qSyn->QuantumCost();
			qa->m_nGates = qSyn->m_nGates;
		
			Release();
		}
	
		return 0;
	}

  
	/// void CSynthesizer::Process(ULONGLONG inTerm, ULONGLONG outTerm)
	///
	/// Inputs:
	///
	/// Outputs:
	///
	

	CSynthesizer::~CSynthesizer(void)
	{

	}
}