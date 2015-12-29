/**************************************************************
 *	Name: Cagatay Demiralp (cad)
 *	File: ICurveDist.H
 *	Proj: ICurveDist
 *	Date: Wed Jul 19 22:26:08 EDT 2006
 **************************************************************/

#ifndef ICurveDist_Header
#define ICurveDist_Header


#include "global.H"
#include <limits>
#include <vector>

const int  __MAX_MEANDIST = 0;
const int  __AVG_MEANDIST = 1;
const int  __MIN_MEANDIST = 2;



class Wpt;

class ICurveDist {
  private:

  //protected:
  public:
    static Wpt NEAREST_LINE_PT(const Wpt &pt,
        const Wpt &s, const Wpt &e);

    static Greal PT_TO_LINE_SEG_DIST(const Wpt &pt,
        const Wpt &s, const Wpt &e);

    static Greal CURVE_TO_CURVE_APPROX_DIST(const std::vector<Greal>& c1,
        const std::vector<Greal>& c2, Greal threshold);

    static Greal CURVE_TO_CURVE_DIST(const std::vector<Greal>& c1,
        const std::vector<Greal>& c2, Greal threshold);


  public:


    ICurveDist(){ }

    static Greal MEANDIST(const std::vector<Greal>& c1,
        const std::vector<Greal>& c2,
        Greal threshold,
        int f);
    ~ICurveDist(){ }
};


#endif
