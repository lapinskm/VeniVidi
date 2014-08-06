#include "common.hpp"
#include "MathModule.hpp"
using namespace cv;

/** 
  * Function bases on Gramm-Shmitt method. To work corectly, 
  * all collumns of input matrix should be linear independent.
  */
void MathModule::orthogonalize(Mat& M, unsigned startIndex)
{
  for (int i = startIndex; i < M.cols; i++)
  {
    Mat u = M.colRange(i, i+1);
    for (int j = 0; j < i; j++)
    {   
      u -= vectorProjection(u, M(Rect(j, 0, 1, M.rows)));
    }
    u.copyTo(M(Rect(i ,0 ,1, M.rows)));
  }
}

/** Generates vector projection of vector a on vector b
  */ 
inline Mat MathModule::vectorProjection(const Mat& a, const Mat& b)
{
   Mat bNormal = b / norm(b);
   return ((a.dot(bNormal))) * bNormal;
}
