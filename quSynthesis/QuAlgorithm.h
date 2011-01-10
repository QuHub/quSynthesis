#pragma once
namespace QuLogic {

	class QuAlgorithm 
	{
	public:
		ULONGLONG m_nBits;
		ULONGLONG m_nTerms;
    ULONGLONG m_QuantumCost;
		PCHAR Name;
    PULONGLONG m_pIn;
    PULONGLONG m_pOut;

	public:
		QuAlgorithm(void){};
    
		QuAlgorithm(int nBits)
		{
			m_nBits = nBits;
			m_nTerms = (ULONGLONG)Math::Pow(2,(double)nBits);
      m_pIn = m_pOut = NULL;
		}

		void virtual Synthesize(PULONGLONG pOut) {throw "Must implement this";}
    void virtual Mutate(double Prob) {throw "Must implement this";}
    QuAlgorithm virtual *Copy() {throw "Must implement this";}
    QuAlgorithm virtual *SinglePointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
	QuAlgorithm virtual *TwoPointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
    virtual ~QuAlgorithm(){};
	};



}
