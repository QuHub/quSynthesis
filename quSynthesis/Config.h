#pragma once
#ifndef _INCLUDE_CONFIG_
#define _INCLUDE_CONFIG_

#ifdef INCLUDE_GLOBALS
#define EXTERN
#define DATA(x) = x
#else
#define EXTERN extern
#define DATA(x)
#endif

namespace QuLogic
{
  EXTERN int *BandBoundary DATA(NULL);
}

namespace Config
{
  EXTERN int PartitionSize DATA(3);
  EXTERN int nBits         DATA(2);
  EXTERN int Radix         DATA(2);
  EXTERN int RadixBits     DATA(1);
  EXTERN int RadixMask     DATA(1);   // pow(2,RadixBits) - 1 
  EXTERN string HomeFolder DATA("");
  EXTERN string Synthesizer DATA("BinaryBasic");

  static void SetTernary()
  {
    PartitionSize=0;
    Config::Radix = 3;
    Config::RadixBits = 2;
    Config::RadixMask = (int)(pow(2.0,RadixBits) - 1);
  }


  static void SetBinary()
  {
    PartitionSize=0;
    Config::Radix = 2;
    Config::RadixBits = 1;
    Config::RadixMask = (int)(pow(2.0,RadixBits) - 1);
  }
}

#undef EXTERN
#undef DATA
#endif

