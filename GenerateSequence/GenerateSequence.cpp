// GenerateSequence.cpp : main project file.

#include "stdafx.h"

#define BITS 11

void test();

int main(array<System::String ^> ^args)
{
  if(!Directory::Exists("../inputs"))
    Directory::CreateDirectory("../inputs");
      

  for(int i=2; i<BITS; i++) {
    Utility::m_nBits = i;
    HiddenWeight g;
    g.Generate(i);
  }
  return 0;
}


void test()
{

  for (int i=0; i<27; i++) {
    int trit = Utility::ToBinaryTrit(i);
    printf("%04d: %04d %d\n", i, trit, Utility::NonZeros(trit));

  }



}