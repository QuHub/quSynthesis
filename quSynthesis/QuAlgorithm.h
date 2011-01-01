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
		}

		void virtual Synthesize(PULONGLONG pOut) {throw "Must implement this";}
    void virtual Mutate() {throw "Must implement this";}
	};


}
