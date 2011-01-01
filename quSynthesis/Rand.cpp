#include "stdafx.h"
#pragma once
#include <vcclr.h>
using namespace System::Reflection;
namespace Rand
{
  gcroot<Random^> m_rnd = gcnew Random();
  double NextDouble() {return m_rnd->NextDouble();}
  int NextInteger(int n) {return m_rnd->Next(n);}
};

