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
	int m_i; // taboo: to be used as move index
	public:
		QuAlgorithm(void){};
    
		QuAlgorithm(int nBits)
		{
			m_nBits = nBits;
			m_nTerms = (ULONGLONG)Math::Pow(2,(double)nBits); 
      m_pIn = m_pOut = NULL;
	  m_i=0;
		}

		void virtual Synthesize(PULONGLONG pOut) {throw "Must implement this";}
    void virtual Mutate(double Prob) {throw "Must implement this";}
	void virtual Move(int band)  {throw "Must implement this";}
    QuAlgorithm virtual *Copy() {throw "Must implement this";}
	bool operator<( const QuAlgorithm& a) const	{    return this->m_QuantumCost < a.m_QuantumCost;}
    QuAlgorithm virtual *SinglePointCrossOver(QuAlgorithm *p, double Prob){throw "Must implement this";}
    virtual ~QuAlgorithm(){};
	};



}
