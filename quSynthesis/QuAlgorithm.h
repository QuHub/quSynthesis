#pragma once
namespace QuLogic {

	class QuAlgorithm 
	{
	public:
		ULONGLONG m_nBits;
		ULONGLONG m_nTerms;
		ULONGLONG m_nGates;
    ULONGLONG m_QuantumCost;
		PCHAR Name;
    PULONGLONG m_pIn;
    PULONGLONG m_pOut;

	// I identified target and control here to get them from Synthesizer.cpp
	PULONGLONG m_pTarget;
	PULONGLONG m_pControl;

	
	public:
		QuAlgorithm(void){
		m_pTarget = new ULONGLONG[10*1024];
		m_pControl = new ULONGLONG[10*1024];
		}
    
		QuAlgorithm(int nBits)
		{
			m_nBits = nBits;
			m_nTerms = (ULONGLONG)Math::Pow(2,(double)nBits);
      m_pIn = m_pOut = NULL;
	  m_pTarget = m_pControl = NULL;

		}

		void virtual Synthesize(PULONGLONG pOut) {throw "Must implement this";}
    void virtual Mutate(double Prob) {throw "Must implement this";}
    QuAlgorithm virtual *Copy() {throw "Must implement this";}
    QuAlgorithm virtual *SinglePointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
	QuAlgorithm virtual *TwoPointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
    virtual ~QuAlgorithm(){delete m_pTarget; delete m_pControl;	}
	};



}
