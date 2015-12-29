#ifndef POINTS_H
#define POINTS_H

#define GLUE_MLIB

class Wpt;
class Wvec;
typedef const class Wvec cWvec;
typedef const class Wpt cWpt;

#include "point3d.H"
#include "vector3d.H"

class Wvec : public _vec3d<Wvec>{
  public :
    static cWvec X;
    static cWvec Y;
    static cWvec Z;
    Wvec()        { }
    Wvec(Greal x, Greal y, Greal z):_vec3d<Wvec>(x,y,z) { }
};

class Wpt : public _point3d<Wpt, Wvec> {
  public :
    Wpt() { }
    Wpt(Greal x, Greal y, Greal z):_point3d<Wpt,Wvec>(x,y,z) { }

};

#endif // POINTS_H

