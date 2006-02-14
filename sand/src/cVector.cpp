#include "cVector.h"
#include "math.h"

double cVector::Normalize()
    {
	  double res;
	  res=1.0/sqrt(x*x+y*y+z*z);
	  x*=res;
	  y*=res;
	  z*=res;
	  return res;
	};
