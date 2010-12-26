#pragma once
namespace QuLogic {
  class CGlobals 
  {
  public:
    static int m_Ones[256];
    CGlobals(void)
    {
      for (int i=0; i<256; i++)
        m_Ones[i] = NumOfOnes(i);
    }


    static int CGlobals::NumOfOnes(int num)
    {
      int nCount = 0;
      for (int i=0; i<8; i++) {
        nCount = num & 1;
        num >>= 1;
      }
      return nCount;
    }
  };
}
