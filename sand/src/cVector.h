// 10.02.2006 written by Christian Klein
#ifndef _CVECTOR_H
#define _CVECTOR_H


class cVector
{
  public:
  cVector(){x=0.0;y=0.0;z=0.0;r=0.1;};
  double x,y,z;
  double r;
  float colorr;
  float colorg;
  float colorb;
  double Normalize();
};

#endif /* _CVECTOR_H */
