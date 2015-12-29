
#include "global.H"
#include <math.h>

const double gEpsZeroMath  = 1e-12;  // Really a very small value

double       gEpsAbsMath     = 1e-8;   // Absolute epsilon
double       gEpsAbsSqrdMath = 1e-16;  // Absolute epsilon squared

const double gEpsNorMath     = 1e-10;  // Normalized epsilon
const double gEpsNorSqrdMath = 1e-20;  // Normalized epsilon squared

void setEpsAbsMath(double eps)
{
    assert(eps > 0);

    gEpsAbsMath     = eps;
    gEpsAbsSqrdMath = eps * eps;
}

double Acos(
   double x
   )
{
   return acos(clamp(x, -1.0, 1.0));
}
