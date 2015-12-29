/**************************************************************
 *	Name: Cagatay Demiralp (cad)
 *	File: BrownICurveDist.H
 *	Proj: ICurveDist
 *	Date: Sun Jul 23 00:23:14 EDT 2006
 **************************************************************/
#ifndef BrownICurveDist_Header
#define BrownICurveDist_Header

extern const int  __MAX_MEANDIST;
extern const int  __AVG_MEANDIST;
extern const int  __MIN_MEANDIST;

class vtkIdType;

double BROWN_MEANDIST(int n1,
    const double c1[][3],
    int n2,
    double c2[][3] ,
    double threshold,
    int f=__MAX_MEANDIST);

///////////////////////////////////////////////////////
// VTK interface for MEANDIST()
// assumes vtk headers are included and  curve points
// are accessed using a pointer variable `points'
// visible to the function (ie., member or global variable)
////////////////////////////////////////////////////////
/* float BROWN_MEANDIST_VTK(vtkIdType
    *idLinePoints1,
    int numberOfPoints1,
    vtkIdType *idLinePoints2,
    int numberOfPoints2,
    float Threshold_Tt,
    int f=__MAX_MEANDIST);
*/

#endif



