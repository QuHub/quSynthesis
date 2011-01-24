#pragma once
namespace QuLogic {

	class QuConductor : public CGlobals
	{
	public:
		ULONGLONG m_nBits;
		ULONGLONG m_nTerms;
		PCHAR Name;
		HANDLE *m_phMutex;
		int m_nThreads;
		CSynthesizer *m_pSynth;

	public:
		QuConductor(void){};
		~QuConductor(){
				// Now that we're done, kill them all...
			for (int i=0; i<m_nThreads; i++)
				m_pSynth[i].Stop();

		}
		QuConductor(int nBits)
		{
			m_nBits = nBits;
			m_nTerms = (ULONGLONG)Math::Pow(2,(double)nBits);

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

		void virtual Synthesize(PULONGLONG pOut){throw "Must implement this";}

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
