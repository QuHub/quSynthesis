#include "stdafx.h"
#pragma once
#include <vcclr.h>
#include <stdlib.h>
#include <time.h>

using namespace System::Reflection;

namespace Rand
{
  gcroot<Random^> m_rnd = gcnew Random(777);
  
  double NextDouble() {return m_rnd->NextDouble();}
  int NextInteger(int n) { return rand() % n ;}
};

