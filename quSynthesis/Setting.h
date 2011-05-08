#pragma once
#ifdef DEFINE_SETTINGS
  #define EXTERN 
#else
  #define EXTERN extern
#endif

namespace QuLogic
{
  EXTERN int *BandBoundary;
  EXTERN int PartitionSize;
  EXTERN int nBits;
  EXTERN int Radix;
  EXTERN int RadixBits;
  EXTERN int RadixMask; 
  EXTERN string HomeFolder;
  EXTERN string Synthesizer;

  static void Ternary()
  {
    BandBoundary=NULL;
    PartitionSize=3;
    Radix=2;
    RadixBits=1;
    RadixMask=1;
    HomeFolder = "";
    Synthesizer = "TernaryBasic";
  }

};

