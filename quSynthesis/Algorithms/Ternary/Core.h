namespace Algorithm {
    namespace Ternary {
      static void Prepare(PINT pOut)
      {
        int nTerms = (int)pow(Config::Radix, (double)Config::nBits);

        for(int i=0; i<nTerms; i++)
          pOut[i] = QuLogic::CGlobals::RadixDigits(pOut[i]);
      }

      class Core {
      public:
        Core{}
      };
  }
}