#ifndef GLOBAL_H
#define GLOBAL_H
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// The basic include file to be included into any modeler file. It contains
// some very basic stuff, needed almost everywhere in the modeler.
//
///////////////////////////////////////////////////////////////////////////////

#include <math.h>
#ifndef GLUE_REAL
#  define GLUE_REAL double
#endif

typedef GLUE_REAL Greal;

// Some compilers don't define these constants
#if defined(WIN32) || defined(__KCC)
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_LN2           0.69314718055994530942
#define M_SQRT2         1.41421356237309504880
#endif

// Include some very frequently used header files so that they do not have
// to be included every time

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Don't use the following global variables directly, use the inline functions
// instead. We have to keep these variables in the header file to be able to
// inline the access functions.
//
extern double       gEpsAbsMath;
extern double       gEpsAbsSqrdMath;
extern const double gEpsNorMath;
extern const double gEpsNorSqrdMath;
extern const double gEpsZeroMath;  // Really a very small value

inline double epsAbsMath    () { return gEpsAbsMath;     }
inline double epsAbsSqrdMath() { return gEpsAbsSqrdMath; }

inline double epsNorMath    () { return gEpsNorMath;     }
inline double epsNorSqrdMath() { return gEpsNorSqrdMath; }

extern void setEpsAbsMath(double eps);

// Hack to get around bug in KAI's math functions
#ifdef __KCC
#define sqrt(x)  sqrt(double(x))
#define log(x)   log(double(x))
#define pow(x,y) pow(double(x),double(y))
#endif

#ifndef GLUE_NEEDS_DOUBLE_STAR_EXPLICIT
template <class T>
inline T operator *(double s, const T &p) { return p * s; }
#endif

template <class T>
inline T clamp(const T a,const T b,const T c) { return a > b ? (a < c ? a : c) : b ; }

// ensure numerical error doesn't cause a value outside the range [-1,1]
// to be passed to the acos function.
extern double   Acos(double x);
#endif
