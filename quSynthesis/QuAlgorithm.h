#pragma once
namespace QuLogic {

  ref class QuAlgorithm
  {
  public:
    ULONGLONG m_nBits;
    ULONGLONG m_nTerms;
    property String^ Name;
    HANDLE *m_phMutex;
    int m_nThreads;
    CSynthesizer *m_pSynth;

  public:
    QuAlgorithm(void){};
    ~QuAlgorithm(){
      	// Now that we're done, kill them all...
    	for (int i=0; i<m_nThreads; i++)
		    m_pSynth[i].Stop();

    }
    QuAlgorithm(int nBits)
    {
      m_nBits = nBits;
      m_nTerms = (ULONGLONG)Math::Pow(2,(double)nBits);

      // Allocate a set of Worker Threads based on the number of CPU cores for synthesizing outputs.
      SYSTEM_INFO sysinfo;
      GetSystemInfo( &sysinfo );

      // Lanuch threads as many as there are cores
      int m_nThreads = sysinfo.dwNumberOfProcessors;

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
      while (!gQueue.Empty())
        Sleep(10);

      WaitForMultipleObjects(m_nThreads, m_phMutex, true, INFINITE);
    }

  };


}
