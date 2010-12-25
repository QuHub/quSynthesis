#include "StdAfx.h"
#include "Synthesizer.h"

CSynthesizer::CSynthesizer(void) 
{
  
}


DWORD CSynthesizer::Run( LPVOID arg )
{
  m_hMutex = (HANDLE)arg;
  while(1) {
    // Look for something in the queue and synthesize it
    Lock();
    CQueueItem *qi = gQueue.Pop();
    if (!qi) {
      Release();
      Sleep(10);
      cout << "Nothing to do\n" ;
      continue;
    }

    cout << "We are doing it dude\n";
    Release();
  }
  return 0;
}


CSynthesizer::~CSynthesizer(void)
{
}
