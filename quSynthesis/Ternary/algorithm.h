
namespace Ternary {
  namespace Algorithm {
      // <summary>
      // 
      // <inputs>
      //
      // <outputs>
      static void Prepare(PINT pOut)
      {
        int nTerms = (int)pow(Config::Radix, (double)Config::nBits);

        for(int i=0; i<nTerms; i++)
          pOut[i] = QuLogic::CGlobals::RadixDigits(pOut[i]);
      }
  }
}