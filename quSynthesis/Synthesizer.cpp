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

			m_nBufSize = 10*1024;
			m_pTarget = new ULONGLONG[m_nBufSize];
			m_pControl = new ULONGLONG[m_nBufSize];

			m_nGates = 0;
			m_nBits = qa->m_nBits;

			PULONGLONG pIn = qa->m_pIn;
			PULONGLONG pOut = qa->m_pOut;

			for (int i=0; i < qa->m_nTerms; i++)
				Process(*pIn++, *pOut++);

			qa->m_pTarget = m_pTarget;
			qa->m_pControl = m_pControl;
	 
			qa->m_QuantumCost = QuantumCost();
			qa->m_nGates = m_nGates;
		
		// till here everything is saved ok.
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
	void CSynthesizer::Process(ULONGLONG inTerm, ULONGLONG outTerm)
	{
		outTerm = Propogate(outTerm);

		// Do we still have a difference?
		// TODO: redo this thing with hash tables
		ULONGLONG diff = inTerm ^ outTerm;

		if (diff > 0) {
			// Flip the 0 bits first
			ULONGLONG mask = 1;
			for (int j = 0; j< m_nBits; j++) {
				if ( (diff & mask) && !(outTerm & mask)) {
					if (m_nGates > m_nBufSize - 8)  // make sure there is enough memory
						Resize();

					m_pControl[m_nGates] = outTerm & ~mask;		// clear inTerm bit at current bit position
					m_pTarget[m_nGates++] = mask;						  // Mark bit position for as NOT gate
					diff &= ~mask;								            // reset bit in diff, since it should be same in outTerm
					outTerm ^= mask;
				}
				mask <<= 1;
			}

			mask = 1;
			// Flip the 1 bits second
			for (int j = 0; j< m_nBits; j++) {
				if ( (diff & mask) && (outTerm & mask)) {
					if (m_nGates > m_nBufSize - 8)  // make sure there is enough memory
						Resize();

					m_pControl[m_nGates] = outTerm & ~mask;		// clear inTerm bit at current bit position
					m_pTarget[m_nGates++] = mask;						// Mark bit position for as NOT gate
					diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
					outTerm ^= mask;
				}
				mask <<= 1;
			}
		}	
	//Target = m_pTarget;
	//cout << "m_nGates:" << m_nGates << endl;
	}



	ULONGLONG CSynthesizer::Propogate(ULONGLONG outTerm)
	{
		// Apply current list of gates..
		for (ULONGLONG i=0; i<m_nGates; i++) {
			ULONGLONG x = outTerm & m_pControl[i];
			if (x == m_pControl[i])
				outTerm ^= m_pTarget[i];
		}
		return outTerm;
	}  

	ULONGLONG CSynthesizer::QuantumCost()
	{
		PULONGLONG pnCount = new ULONGLONG[m_nBits];
		ZeroMemory(pnCount, m_nBits * sizeof(ULONGLONG));
		for (int i=0; i<m_nGates; i++) 
			pnCount[ControlLines(m_pControl[i])]++;

		ULONGLONG nCost=0;
		for (int i=0; i<m_nBits; i++)
			nCost += GateCost(i) * pnCount[i];

		return nCost;
	}


	///   ULONGLONG CSynthesizer::GateCost(int i)
	///
	///
	/// Inputs:
	///
	/// Outputs:
	///
	ULONGLONG CSynthesizer::GateCost(int i)
	{
		return Math::Max(1, (int)Math::Pow(2, 1 + i) - 3);
	}

	///   ULONGLONG CSynthesizer::ControlLines(ULONGLONG n)
	///
	///
	/// Inputs:
	///
	/// Outputs:
	///
	ULONGLONG CSynthesizer::ControlLines(ULONGLONG n)
	{
		ULONGLONG nCount=0;

		for (int i=0; i<8; i++) {
			nCount += Ones(n & 0xFF);
			n >>= 8;
		}
		return nCount;
	}

	void CSynthesizer::Resize()
	{
		PULONGLONG p=new ULONGLONG[m_nBufSize+2048]; 
		CopyMemory(p, m_pControl, m_nBufSize*sizeof(ULONGLONG));
		delete m_pControl;
		m_pControl= p;

		p=new ULONGLONG[m_nBufSize+2048]; 
		CopyMemory(p, m_pTarget, m_nBufSize*sizeof(ULONGLONG));
		delete m_pTarget;
		m_pTarget= p;

		m_nBufSize += 2048;
	}

	CSynthesizer::~CSynthesizer(void)
	{

	}
}