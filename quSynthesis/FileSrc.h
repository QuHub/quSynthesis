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
using namespace System::IO;

#define SRC "..\\inputs\\"

namespace QuLogic {
  public ref class FileSrc {
    UINT m_nCount;
    ULONGLONG m_nFiles, m_nSequence, m_nBits, m_nTerms;
    array<String^, 1>^ m_Files;
    StreamReader ^m_sr;
    ULONGLONG *m_pInput;

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
      m_pInput = new ULONGLONG[m_nTerms];
    }

    ~FileSrc()
    {
      delete m_pInput;
    }

    PULONGLONG Next()
    {
      String ^s;
      if(m_sr->Peek() >= 0)
        s = m_sr->ReadLine();
      else
        return NULL;

      array<String^>^ list = s->Split(' ');
      PULONGLONG p = m_pInput;
      Name = list[0];
      for (int i=0; i<m_nTerms; i++)
        *p++ = Convert::ToUInt64(list[i+1]);

      return m_pInput;
    }
  };
}
