// quSynthesis.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "FileSrc.h"

using namespace System;
using namespace QuLogic;

int main(array<System::String ^> ^args)
{

	FileSrc fs(4, "random*");
	vector<ULONGLONG> v = fs.Next();



	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	// Lanuch threads as many as there are cores
	int numCPU = sysinfo.dwNumberOfProcessors;
	vector<CSynthesizer> synth (numCPU);
	for (int i=0; i<numCPU; i++)
		synth[i].Start();

	// Do the work here...
	// Read the file
	// Deliver the output mapping to specific algorithm for synthesis
	
	Sleep(10000);

	// Now that we're done, kill them all...
	for (int i=0; i<numCPU; i++)
		synth[i].Stop();

	return 0;
}


void Fill(int *p, int value)
{
	for (int i=0; i<1000; i++)
		p[i] = value;

}
