/*************************************************************************
 *    NAME: Cagatay Demiralp
 *    USER: cad
 *    FILE: BrownICurveDist.C
 *    DATE: Sun Jul 23  0:42:16 2006
 *************************************************************************/
#include "ICurveDist.H"
#include "BrownICurveDist.H"
#include "points.H"
#include <vector>


/*************************************************************************
 * Function Name: BROWN_MEANDIST
 * Parameters:
 * Returns: double
 * Effects:
 *************************************************************************/

double BROWN_MEANDIST(int n1,
    const double c1[][3],
    int n2,
    double c2[][3] ,
    double threshold,
    int f){

  std::vector<Greal> C1(3 * n1);
  std::vector<Greal> C2(3 * n2);

  for(int i=0; i< n1; i++){
    C1[3*i] = c1[i][0]; C1[3*i+1]=c1[i][1]; C1[3*i+2] =c1[i][2];
  }
  for(int i=0; i< n2; i++){
    C2[3*i] =c2[i][0]; C2[3*i+1]=c2[i][1]; C2[3*i+2] =c2[i][2];
  }
  return (double) (ICurveDist::MEANDIST(C1, C2, threshold, f));
}


/*************************************************************************
 * Function Name: BROWN_MEANDIST_VTK
 * Parameters: vtkIdType *idLinePoints1, int numberOfPoints1, vtkIdType *idLinePoints2, int numberOfPoints2, float Threshold_Tt
 * Returns: float
 * Effects:
 *************************************************************************/
/*
float
BROWN_MEANDIST_VTK(vtkIdType *idLinePoints1,
    int numberOfPoints1,
    vtkIdType *idLinePoints2,
    int numberOfPoints2,
    float Threshold_Tt,
    int f)
{
  std::vector<Greal> c1(3 * numberOfPoints1);
  std::vector<Greal> c2(3 * numberOfPoints2);

  float* p = NULL:
  for(int i=0; i< numberOfPoints1; i++){
    p = points->GetPoint(idLinePoints1[i]);
    c1[3*i] =(Greal)p[0]; c1[3*i+1]=(Greal)p[1]; c1[3*i+2]=(Greal)p[2];
  }
  for(int i=0; i< numberOfPoints2; i++){
    p = points->GetPoint(idLinePoints2[i]);
    c2[3*i]=(Greal)p[0]; c2[3*i+1]=(Greal)p[1]; c2[3*i+2]=(Greal)p[2];
  }
  return (float) (ICurveDist::MEANDIST(c1,c2, (double)Threshold_Tt, f));
}
*/

