#pragma once
namespace QuLogic {

	class QuAlgorithm 
	{
	public:
		ULONGLONG m_nBits;
		ULONGLONG m_nTerms;
		PCHAR Name;

	public:
		QuAlgorithm(void){};
		QuAlgorithm(int nBits)
		{
			m_nBits = nBits;
			m_nTerms = (ULONGLONG)Math::Pow(2,(double)nBits);
		}

		void virtual Synthesize(PULONGLONG pOut) {throw "Must implement this";}
		int virtual QuantumCost() {throw "Must implement this";}
    int virtual Mutate() {throw "Must implement this";}


	};


}
