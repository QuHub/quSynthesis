#pragma once
#include <windows.h>

namespace QuLogic
{
  typedef struct {
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
} stopWatch;

  class CStopWatch {
  private:
	  stopWatch timer;
	  LARGE_INTEGER frequency;

  public:
  double LIToSecs( LARGE_INTEGER & L) {
      return ((double)L.QuadPart /(double)frequency.QuadPart);
    }

    CStopWatch(){
      timer.start.QuadPart=0;
      timer.stop.QuadPart=0;	
      QueryPerformanceFrequency( &frequency );
    }

    void startTimer( ) {
      QueryPerformanceCounter(&timer.start);
    }

    void stopTimer( ) {
      QueryPerformanceCounter(&timer.stop);
    }


    double getElapsedTime() {
      LARGE_INTEGER time;
      time.QuadPart = timer.stop.QuadPart - timer.start.QuadPart;
      return LIToSecs( time) ;
    }
  };
}