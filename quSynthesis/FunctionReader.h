#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <msclr\marshal.h>
#include <windows.h>
#include "Function.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;
using namespace System::IO;

#define INPUT_DIR "..\\inputs\\"


namespace QuLogic {
  public ref class FunctionReader {
    UINT m_nCount;
    ULONGLONG m_nFiles, m_nSequence, m_nBits, m_nTerms;
    array<String^, 1>^ m_Files;
    StreamReader ^m_sr;
    ULONGLONG *m_pInput;
    List<Function^>^ m_Functions;

  public:
    int Count() {return m_nCount;}


    FunctionReader(String^ FilePattern) {
      m_Functions =  gcnew List<Function^>();
      array<String^>^ files = Directory::GetFiles(INPUT_DIR, FilePattern + ".txt");

      m_nCount = files->Length;
      for(int i =0; i < m_nCount ; i++) 
          Read(files[i]);
    }

    void Read(String^ fileName)
    {
    }

    ~FunctionReader()
    {
    }

    Function^ Next()
    {
      ULONGLONG pIn[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
      ULONGLONG pOut[] = {0, 16, 32, 48, 64, 80, 96, 112, 2, 4, 18, 20, 34, 36, 50, 52, 68, 84, 5, 21, 37, 53, 8, 24, 6, 128, 7, 1, 23, 17, 10, 12};
      return gcnew Function("Ankit", 8, 32, pIn, pOut);
    }
  };
}
