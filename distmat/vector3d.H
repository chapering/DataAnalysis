#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>
#include "global.H"
template <class V>
class _vec3d
{
protected:
    Greal _x, _y, _z;

public:

    _vec3d()                             : _x(0),    _y(0),    _z(0)   {}
    _vec3d(Greal x, Greal y, Greal z) : _x(x),    _y(y),    _z(z)   {}

    const Greal *data()             const { return &_x; }

    void    set(Greal x, Greal y, Greal z) { _x=x; _y=y; _z=z; }

    V       operator + (const V &v) const { return V(_x+v._x,_y+v._y,_z+v._z); }
    V       operator - (const V &v) const { return V(_x-v._x,_y-v._y,_z-v._z); }
    Greal  operator * (const V &v) const { return _x*v._x+_y*v._y+_z*v._z;    }
    V       operator - ()           const { return V(-_x, -_y, -_z);           }

    V       operator * (Greal s)   const { return V(_x*s, _y*s, _z*s); }
    V       operator / (Greal s)   const { return V(_x/s, _y/s, _z/s); }
    Greal  operator [](int index)  const { return (&_x)[index]; }
    Greal& operator [](int index)        { return (&_x)[index]; }

    // So min(V, V) works (max, too)
    int     operator > (const V &v) const { return length() > v.length();}
    int     operator < (const V &v) const { return length() < v.length();}

    Greal  length     ()           const { return sqrt(_x*_x+_y*_y+_z*_z);   }
    Greal  lengthSqrd ()           const { return _x*_x+_y*_y+_z*_z;         }
    Greal  lengthRect ()           const { return fabs(_x)+fabs(_y)+fabs(_z);}

    Greal  dist       (const V &v) const { return (*this-v).length();     }
    Greal  distSqrd   (const V &v) const { return (*this-v).lengthSqrd(); }

    bool    isEqual    (const V &v, Greal epsSqrd = epsNorSqrdMath()) const
                                          { return (distSqrd(v) <= epsSqrd); }

    void    operator +=(const V &v)       { _x += v._x; _y += v._y; _z += v._z;}
    void    operator -=(const V &v)       { _x -= v._x; _y -= v._y; _z -= v._z;}
    void    operator *=(Greal   s)       { _x *= s; _y *= s; _z *= s; }
    void    operator /=(Greal   s)       { _x /= s; _y /= s; _z /= s; }


    bool    operator ==(const V &v) const{ return v[0]==_x&&v[1]==_y&&v[2]==_z;}
    bool    operator !=(const V &v) const{ return v[0]!=_x||v[1]!=_y||v[2]!=_z;}
    bool    isNull     (Greal epsSqrd = epsNorSqrdMath()) const
                                          { return lengthSqrd() <= epsSqrd; }

    inline Greal angle(const V &)        const;
    inline V      normalize ()            const;
    inline V      perpend   ()            const;
    inline bool   isParallel (const V &)  const;
    inline bool   isPerpend  (const V &)  const;
    inline void   orthoplane (int &i1, int &i2)          const;
    inline void   orthoplane (int &i1, int &i2, int &i3) const;
    inline V      orthogonalize(const V &b) {
       return *this - ((*this * b) / (b * b)) * b;
    }

}; // class _vec3d

/* ---------- inlined member functions using _point3d template ------ */

template <class V>
inline V
_vec3d<V>::normalize() const
{
    const Greal l = length();
    if (l > gEpsZeroMath)
        return V(_x/l, _y/l, _z/l);
    return V();
}


//
// The well known AutoCAD's arbitrary axis algorithm.
//
template <class V>
inline V
_vec3d<V>::perpend() const
{
    const V a = normalize();

    return cross(fabs(a._x) < 1.0/64.0 && fabs(a._y) < 1.0/64.0
                ? V(0,1,0)
                : V(0,0,1), a).normalize();

    // Note:
    //
    // Do not replace the above vec3d(0,1,0), etc. with vec3d::kYDir.
    // This method is called from a constructor executed during initialization
    // of static objects and at that time the objects such as vec3d::kYDir
    // are not yet initalized.
}


template <class V>
inline bool
_vec3d<V>::isParallel(const V &v) const
{
    const V a = normalize();

    if (a == V())
        return false;

    const V b = v.normalize();

    if (b == V())
        return false;

    return (a-b).lengthSqrd() <= epsNorSqrdMath() ||
           (a+b).lengthSqrd() <= epsNorSqrdMath();
}


template <class V>
inline bool
_vec3d<V>::isPerpend(const V &v) const
{
    const V a =   normalize();
    const V b = v.normalize();

    if (a == V() || b == V())
        return false;

    return fabs(a * b) < epsNorMath();
}

template <class V>
inline void
_vec3d<V>::orthoplane(int& i1, int& i2, int& i3) const
{
    i1 = 0;
    i2 = 1;
    i3 = 2;

    Greal max = fabs(_z);

    if (fabs(_x) > max)
    {
        max = fabs(_x);
        i1  = 1;
        i2  = 2;
        i3  = 0;
    }

    if (fabs(_y) > max)
    {
        max = fabs(_y);
        i1  = 2;
        i2  = 0;
        i3  = 1;
    }

    if ((*this)[i3] < 0)
    {
        const int tmp = i1;
        i1 = i2;
        i2 = tmp;
    }
}


template <class V>
inline void
_vec3d<V>::orthoplane(int& i1, int& i2) const
{
    int i3;
    orthoplane(i1, i2, i3);
}


template <class V>
inline Greal
_vec3d<V>::angle(const V& v) const
{
    Greal cosAngle = ((*this) * v) / (length() * v.length());

    if (cosAngle < -1)
        cosAngle = -1;
    if (cosAngle >  1)
        cosAngle =  1;

    return acos(cosAngle);
}

/* ---------- inlined global functions using _point3d template ------ */

template <class V>
inline std::ostream &
operator<<(std::ostream &os, const _vec3d<V> &v)
{
   return os << "{ " << v[0] << " " << v[1] << " " << v[2] << " } ";
}


template <class V>
inline std::istream &
operator>>(std::istream &is, _vec3d<V> &v)
{
   char dummy;
   return is >> dummy >> v[0] >> v[1] >> v[2] >> dummy;
}


template <class V>
inline V
cross(const V &v1, const V &v2)
{
   return V(v1[1]*v2[2]-v1[2]*v2[1],
            v1[2]*v2[0]-v1[0]*v2[2],
            v1[0]*v2[1]-v1[1]*v2[0]);
}


template <class V>
inline Greal
det(const V &a, const V &b, const V &c)
{
   return (a[0] * (b[1]*c[2] - b[2]*c[1]) +
           a[1] * (b[2]*c[0] - b[0]*c[2]) +
           a[2] * (b[0]*c[1] - b[1]*c[0]));
}


#endif // VECTOR3D_H
