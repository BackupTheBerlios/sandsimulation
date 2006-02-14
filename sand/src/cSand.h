// 10. Feb. 2006: written by Christian Klein
#ifndef _CSAND_H
#define _CSAND_H

#include "configtool.h"
#include "cVector.h"
#include "cSurface.h"
#include "math.h"

class cSand
{
  public:
  cSand(){count=0; koerner=NULL;};
  ~cSand(){Quit();};
  
  void Init();
  void NextStep();
  void NextStep2();
  void Quit();
  cVector *koerner;
  cVector *geschw;
  int count;
  double dt;
  cVector grav;
  cSurface umgebung;
  cSurface umgvorgabe;
  ConfigTool *Config;
  
};

#endif /* _CSAND_H */
