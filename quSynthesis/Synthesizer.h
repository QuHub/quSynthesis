#pragma once
class CSynthesizer : public CThread
{
public:
  CSynthesizer(void);
  ~CSynthesizer(void);
  DWORD Run( LPVOID /* arg */ );

};

