/*************************************************************************
 *    NAME: Cagatay Demiralp
 *    USER: cad
 *    FILE: ICurveDist.C
 *    DATE: Wed Jul 19 23:11:36 2006
 *************************************************************************/

#include "ICurveDist.H"
#include "BrownICurveDist.H"
#include "points.H"


/*************************************************************************
 * Function Name: ICurveDist::NEAREST_LINE_PT
 * Parameters: const Wpt &pt, const Wpt &s, const Wpt &e
 * Returns: Wpt
 * Effects: returns the nearest point on the line defined by (s,e)
 * to the point pt by projecting pt onto the line.
 *************************************************************************/
Wpt
ICurveDist::NEAREST_LINE_PT(const Wpt &pt, const Wpt &s, const Wpt &e)
{
  return (s + (e-s)*(((e-s)*(pt-s))/(e-s).length()));
}


/*************************************************************************
 * Function Name: ICurveDist::PT_TO_LINE_SEG_DIST
 * Parameters: const Wpt &pt, const Wpt &s, const Wpt &e
 * Returns: Greal
 * Effects: returns the distance from the nearest point on the line seg
 * defined by (s,e) to the given point pt.
 *************************************************************************/
Greal
ICurveDist::PT_TO_LINE_SEG_DIST(const Wpt &pt, const Wpt &s, const Wpt &e)
{
  Wpt npt = ICurveDist::NEAREST_LINE_PT(pt, s, e);
  if ((npt - s) * (npt - e) < 0) return (pt - npt).length();
  return (pt - (((pt-s).lengthSqrd()<(pt-e).lengthSqrd()) ? s : e)).length();
}


/*************************************************************************
 * Function Name: ICurveDist::CURVE_TO_CURVE_DIST
 * Parameters: const std::vector<Greal>& c1, const std::vector<Greal>& c2, Greal threshold
 * Returns: Greal
 * Effects:
 *************************************************************************/
Greal
ICurveDist::CURVE_TO_CURVE_DIST(const std::vector<Greal>& c1, const std::vector<Greal>& c2, Greal threshold)
{

  //num of pts having distances above 'threshold'
  int cnt=0;
  Greal d=0.0f;

  //Greal totalweight = 0.0f;

  int n1 = (int)(c1.size()/3);
  int n2 = (int)(c2.size()/3);

  for(int i=0; i< n1; i++){
    Greal min_d = std::numeric_limits<Greal>::max();
    int bi= i*3;
    Wpt pt(c1[bi], c1[bi+1], c1[bi+2]);

    for(int j=0; j< n2-1; j++){

      int bj_s= j*3;
      int bj_e= (j+1)*3;

      Wpt s(c2[bj_s],
          c2[bj_s+1],
          c2[bj_s+2]);

      Wpt e(c2[bj_e],
          c2[bj_e+1],
          c2[bj_e+2]);

      Greal t = ICurveDist::PT_TO_LINE_SEG_DIST(pt, s, e);
      min_d = (t< min_d)?t:min_d;
    }

//#ifdef gaussian_weighted_distance
    //////////////////////////////////////
    //gaussian weighted distance
    //
    //if(0){
    //
    //
/*    double weight = 1.0f;
    double mu = 0.0f;
    double sigmasquare = 1.0f;
    double x =  ((i < (n1 -1 - i)) ? i : (n1 - 1 - i));

    //8.0f is somewhat arbitrary at this point
    x = 8.0f*(x/(n1-1));
    weight = (1/sqrt(2*M_PI*sigmasquare)*exp(-(x-mu)*(x-mu)/(2*sigmasquare)));
    min_d *= weight;
*/
    //}
    /////////////////////////////////////
//#endif
    // when Greal is defined to be  float the following
    // double typecasting increases numerical accuracy by
    // limiting  the numerical error to float truncation
    //
    if(min_d >= threshold){
      d=(double)(d+min_d);
      //totalweight+=weight;
      cnt++;}
  }
  //return ((cnt==0)?0:(d/(totalweight*(Greal)cnt)));
  return ((cnt==0)?0:(d/((Greal)cnt)));

}




/*************************************************************************
 * Function Name: ICurveDist::CURVE_TO_CURVE_APPROX_DIST
 * Parameters: const std::vector<Greal>& c1,
 * const std::vector<Greal>& c2, Greal threshold
 * Returns: Greal
 * Effects:
 *************************************************************************/
Greal
ICurveDist::CURVE_TO_CURVE_APPROX_DIST(
    const std::vector<Greal>& c1,
    const std::vector<Greal>& c2,
    Greal threshold)
{
  //num of pts having distances above 'threshold'
  int cnt=0;

  Greal d=0.0f;
  int n1 = (int)(c1.size()/3);
  int n2 = (int)(c2.size()/3);

  for(int i=0; i< n1; i++){
    Greal min_d_pt = std::numeric_limits<Greal>::max();

    int indx=-1;
    int bi= i*3;
    Wpt pt_c1(c1[bi], c1[bi+1], c1[bi+2]);

    for(int j=0; j< n2; j++){
      int bj= j*3;
      Wpt pt_c2(c2[bj], c2[bj+1], c2[bj+2]);
      Greal t = (pt_c1 - pt_c2).length();
      if(t< min_d_pt) { min_d_pt = t; indx = j; }
    }

    Greal min_d = 0.0f;

    if(indx == 0){
      min_d = ICurveDist::PT_TO_LINE_SEG_DIST(pt_c1,
          Wpt(c2[3*indx], c2[3*indx+1], c2[3*indx+2]),
          Wpt(c2[3*(indx+1)], c2[3*(indx+1)+1], c2[3*(indx+1)+2]));

    }else if(indx == (n2-1)){
      min_d = ICurveDist::PT_TO_LINE_SEG_DIST(pt_c1,
            Wpt(c2[3*indx], c2[3*indx+1], c2[3*indx+2]),
            Wpt(c2[3*(indx-1)], c2[3*(indx-1)+1], c2[3*(indx-1)+2]));
    }else{
      Greal min_d1 = ICurveDist::PT_TO_LINE_SEG_DIST(pt_c1,
            Wpt(c2[3*indx], c2[3*indx+1], c2[3*indx+2]),
            Wpt(c2[3*(indx-1)], c2[3*(indx-1)+1], c2[3*(indx-1)+2]));

      Greal min_d2 = ICurveDist::PT_TO_LINE_SEG_DIST(pt_c1,
            Wpt(c2[3*indx], c2[3*indx+1], c2[3*indx+2]),
            Wpt(c2[3*(indx+1)], c2[3*(indx+1)+1], c2[3*(indx+1)+2]));
      min_d = (min_d1<min_d2)?min_d1:min_d2;
    }

    // when Greal is defined to be  float the following
    // double typecasting increases numerical accuracy by
    // limiting  the numerical error to float truncation
    if(min_d >= threshold){d=(double)(d+min_d); cnt++;}

  }

  return ((cnt==0)?0:(d/(Greal)cnt));
}

/*************************************************************************
 * Function Name: ICurveDist::MEANDIST
 * Parameters: const std::vector<double>& c1, const std::vector<double>& c2, double threshold, int f=BrownICurveDist::_MAX_MEANDIST
 * Returns: double
 * Effects:
 *************************************************************************/
Greal
ICurveDist::MEANDIST(const std::vector<Greal>& c1, const std::vector<Greal>& c2, Greal threshold, int f)
{
  Greal d = 0.0f;
  Greal d_ij = ICurveDist::CURVE_TO_CURVE_APPROX_DIST(c1,c2,threshold);
  Greal d_ji = ICurveDist::CURVE_TO_CURVE_APPROX_DIST(c2,c1,threshold);

  if(f == __MAX_MEANDIST){
    d = ((d_ij > d_ji) ? d_ij : d_ji);
  } else if(f == __AVG_MEANDIST){
    d =  (d_ij + d_ji)*0.5f;
  } else {
    d = ((d_ij < d_ji) ? d_ij : d_ji);
  }
  return d;
}

