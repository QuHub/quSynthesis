// TernaryBasic.h

#include "Windows.h"
#include "..\CoreSynthesizer.h"
#pragma once

using namespace System;


namespace Ternary {
  namespace Cuda {
    public class Synthesizer : public Ternary::CoreSynthesizer
		{
      protected:
    public:
			__declspec(dllexport) bool AddSequence(PINT pIn, PINT pOut);
			__declspec(dllexport) void Synthesize();

      Synthesizer(int nBits) : CoreSynthesizer(nBits)
      {
          m_outputBlockSize = 250*1024 * sizeof(INT);
      }

		};
	}
}
