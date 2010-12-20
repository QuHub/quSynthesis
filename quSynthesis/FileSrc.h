#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <msclr\marshal.h>
#include <windows.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace std;
using namespace msclr::interop;
using namespace System::IO;

#define SRC "..\\inputs\\"

namespace QuLogic {
  public ref class FileSrc {
    UINT m_nCount;
    ULONGLONG m_nFiles, m_nSequence, m_nBits, m_nTerms;
    array<String^, 1>^ m_Files;
    StreamReader ^m_sr;
    ifstream *m_fsCurrent;

  public:
    property String^ Name;
    property String^ SeqName;


    //**************
    FileSrc(ULONG nBits, String^ FilePrefix) {
      SeqName = FilePrefix->Replace('*', ' ')->TrimEnd();
      m_nBits = nBits;
      m_nTerms = (ULONGLONG)Math::Pow(2,(double)nBits);

      String^ sDir = SRC + m_nBits.ToString() + "-bits-src";
      if ( !Directory::Exists(sDir) )
        throw gcnew Exception(sDir + " Does not exist");

       array<String^, 1>^ files = Directory::GetFiles(sDir, FilePrefix + ".txt");

      m_sr = gcnew StreamReader(files[0]);
    }

    vector<ULONGLONG> Next()
    {
      vector<ULONGLONG> v;

      String ^s;
      if(m_sr->Peek() >= 0)
        s = m_sr->ReadLine();
  
      array<String^>^ list = s->Split(' ');

      Name = list[0];
      for (int i=0; i<m_nTerms; i++)
        v.push_back(Convert::ToUInt64(list[i+1]));

      return v;
    }
  };
}
