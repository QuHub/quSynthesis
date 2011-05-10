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
    if (Config::Synthesizer.compare("TernaryBasic") == 0) qSyn = new TernaryBasic();

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

			PINT pIn =  qa->m_pIn;
			PINT pOut = qa->m_pOut;

      qSyn->Before(qa->m_nBits);
      //qa->Dump();

			for (int i=0; i < qa->m_nTerms; i++)
				qSyn->Process(*pIn++, *pOut++);

			qa->m_pTarget  = qSyn->m_pTarget;
			qa->m_pControl = qSyn->m_pControl;
			qa->m_pOperation = qSyn->m_pOperation;

			qa->m_QuantumCost = qSyn->QuantumCost();
			qa->m_nGates = qSyn->m_nGates;
      //qa->Dump();
		
			Release();
		}
	
		return 0;
	}

  
	/// void CSynthesizer::Process(int inTerm, int outTerm)
	///
	/// Inputs:
	///
	/// Outputs:
	///
	

	CSynthesizer::~CSynthesizer(void)
	{

	}
}