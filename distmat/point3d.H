
#ifndef POINT3D_H
#define POINT3D_H


#include "global.H"
#include <iostream>

template <class P, class V>
class _point3d
{
  protected:
    Greal _x, _y, _z;

  public:

    _point3d()                             : _x(0),    _y(0),    _z(0)   {}
    _point3d(Greal x, Greal y, Greal z) : _x(x),    _y(y),    _z(z)   {}

    void          set(Greal x, Greal y, Greal z)  { _x=x; _y=y; _z=z; }
    const Greal *data()            const{ return &_x; }

    P  operator  *(Greal s)        const{ return P(_x*s, _y*s, _z*s);}
    P  operator  /(Greal s)        const{ return P(_x/s, _y/s, _z/s);}
    //the '%' operator allows points to be added together
    P  operator  %(const P &p)      const{ return P(_x+p[0], _y+p[1], _z+p[2]);}
    P  operator  +(const V &v)      const{ return P(_x+v[0], _y+v[1], _z+v[2]);}
    V  operator  -(const P &p)      const{ return V(_x-p[0], _y-p[1], _z-p[2]);}
    P  operator  -(const V &v)      const{ return P(_x-v[0], _y-v[1], _z-v[2]);}
    P  operator  -()                const{ return P(-_x, -_y, -_z);}

    Greal  operator [](int index)  const{ return (&_x)[index]; }
    Greal& operator [](int index)       { return (&_x)[index]; }

    Greal  distSqrd   (const P &p) const{ return (_x-p._x)*(_x-p._x) +
      (_y-p._y)*(_y-p._y) +
        (_z-p._z)*(_z-p._z); }
    Greal  dist       (const P &p) const{ return sqrt(distSqrd(p));}

    bool    isEqual    (const P &p, Greal epsSqrd = epsAbsSqrdMath()) const
    { return distSqrd(p) <= epsSqrd; }
    bool    operator ==(const P &p) const{ return _x==p._x&&_y==p._y&&_z==p._z;}
    bool    operator !=(const P &p) const{ return _x!=p._x||_y!=p._y||_z!=p._z;}
    // Point = Point + Point
    void    operator %=(const P &p)      { _x += p[0]; _y += p[1]; _z += p[2]; }
    // Point = Point + Vector
    void    operator +=(const V &v)      { _x += v[0]; _y += v[1]; _z += v[2]; }
    void    operator -=(const V &v)      { _x -= v[0]; _y -= v[1]; _z -= v[2]; }
    void    operator *=(Greal   s)       { _x *= s; _y *= s; _z *= s; }
    void    operator /=(Greal   s)       { _x /= s; _y /= s; _z /= s; }
}; // class point3d


/* ---------- inlined global functions using _point3d template ------ */

template <class P, class V>
  inline std::ostream &
operator<<(std::ostream &os, const _point3d<P,V> &p)
{
  return os << "< " << p[0] << " " << p[1] << " " << p[2] << " > ";
}


template <class P, class V>
  inline std::istream &
operator>>(std::istream &is, _point3d<P,V> &p)
{
  char dummy;
  return is >> dummy >> p[0] >> p[1] >> p[2] >> dummy;
}


template <class P, class V>
  inline Greal
det(const _point3d<P,V> &a, const _point3d<P,V> &b, const _point3d<P,V> &c)
{
  return (a[0] * (b[1]*c[2] - b[2]*c[1]) +
      a[1] * (b[2]*c[0] - b[0]*c[2]) +
      a[2] * (b[0]*c[1] - b[1]*c[0]));
}

#endif // POINT3D_H
