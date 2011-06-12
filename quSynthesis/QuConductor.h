#pragma once
#include "Result.h"
namespace QuLogic {

	class QuConductor : public CGlobals, public CResult
	{
	public:
		PCHAR Name;
    int m_nBits;
		HANDLE *m_phMutex;
		int m_nThreads;
		CSynthesizer *m_pSynth;
    gcroot<Type^> m_AlgoType;


  private:
    QuAlgorithm *m_pAlgo[1];

	public:
		QuConductor(void){};
		
    QuConductor(int nBits, Type^ T)
    {
      m_AlgoType = T;
      m_nBits = nBits;

      if(m_AlgoType->Name == "NaturalOrder") m_pAlgo[0] = new Ternary::Algorithm::NaturalOrder(m_nBits);
      Setup(nBits);
    }

		~QuConductor(){
				// Now that we're done, kill them all...
			for (int i=0; i<m_nThreads; i++) {
				m_pSynth[i].Stop();
        ReleaseMutex(m_phMutex[i]);
      }

      // Give the threads a chance to die (otherwise, problems loom)
      Sleep(1000);
      delete [] m_pSynth;
      delete [] m_phMutex;
      delete m_pAlgo[0];
     }

    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
		QuConductor(int nBits)
		{

      Setup(nBits);
    }

    void Setup(int nBits)
    {
      m_nBits = nBits;
			// Allocate a set of Worker Threads based on the number of CPU cores for synthesizing outputs.
			SYSTEM_INFO sysinfo;
			GetSystemInfo( &sysinfo );

			// Lanuch threads as many as there are cores
			m_nThreads = sysinfo.dwNumberOfProcessors;

			// A set of Mutex objects necessary to figure out when all threads
			// Have finished processing their inputs, after queue has been emptied.
			// These Mutecis are used in WaitForQueue()
			m_phMutex = new HANDLE[m_nThreads];
			m_pSynth = new CSynthesizer[m_nThreads];
			for (int i=0; i<m_nThreads; i++) {
				m_phMutex[i] = CreateMutex(NULL, false, NULL);
				m_pSynth[i].Start(m_phMutex[i]);
			}
		}


    void virtual Synthesize(PINT pOut)
    {
      m_pAlgo[0]->Synthesize(pOut);

      WaitForQueue();
      Console::Write("BestCost: {0}\n", m_pAlgo[0]->m_QuantumCost);
      SaveResult(m_pAlgo[0]);
    }


    // <summary>
    // 
    // <inputs>
    //
    // <outputs>
		void WaitForQueue()
		{
			Print("Waiting for Queue To Empty");
			while (!gQueue.Empty())
				Sleep(1);

			Print("Waiting for All Mutexes");
			WaitForMultipleObjects(m_nThreads, m_phMutex, true, INFINITE);
			Print("All Mutexes are released");

			// WaitForMultipleObjects() will hold the lock on all mutecis, which means
			// they have to be released in order for the threads to proceed...
			for (int i=0; i<m_nThreads; i++) {
				::ReleaseMutex(m_phMutex[i]);
			}
			Print("Releasing All Mutecis");
		}
	};


}
