#include "StdAfx.h"
#include "Synthesizer.h"

CSynthesizer::CSynthesizer(void) 
{
  
}


DWORD CSynthesizer::Run( LPVOID /* arg */ )
{
  while(1) {
    // Look for something in the queue and synthesize it
    cout << GetCurrentThreadId() << "\n";
    Sleep(1000);
  }
  return 0;
}


CSynthesizer::~CSynthesizer(void)
{
}
