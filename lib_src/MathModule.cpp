#include "common.hpp"
#include "MathModule.hpp"
using namespace cv;


/**Algorithm from publication:
 *
 * Matthew Brand
 *
 * Incremental singular value decomposition of
 * uncertain data with missing values.
 *
 * TR-2002-24 May 2002
 */

bool MathModule::updateSVD(Mat& U,
                           Mat& S,
                           Mat& V,
                           const Mat& C)
{
  int n=S.cols, m=S.rows;

  //validate matrice dimensions
  if (  n==-1
     || m==-1
     || !( U.rows == m
        && U.cols == m
        && V.rows == n
        && V.cols == n
        && C.rows == m
        && C.cols == 1 ) )
  {
    return false;
  }

  // L size is the same as C (m,1)
  Mat L = U.t()*C;

  //same dimension as C (m,1)
  //Mat J= C-U*UT*C;
  Mat J= C-U*L;

  // K is (1,1)
  //norm(C-U*UT*C)=norm(J)
  double k=norm(J);

  divByScalar(J, k);

  /**Q  matrix assembly, Result should have this form:
   *
   *Q = [S, L;
   *     0, k];
   *
   * where S and L are matrices 0 is a row and k is a scalar
   * it has n+1 cols collumns and m+1 rows
   */
  Mat Q( m+1,n+1, numericDataType, Scalar(0));

  S.copyTo(Q(Rect(0,0,n,m)));
  L.copyTo(Q(Rect(n,0,1,m)));
  Q.at<double>(n,m)=k;
  Mat V1, V1T, S1, U1;
  //Q matrix SVD. Much faster than SVD for whole data.
//  SVD::compute(Q, S1, U1, V1T,0/* SVD::FULL_UV*/);
  SVD::compute(Q, S1, U1, V1T, SVD::FULL_UV);
  if(U1.cols>m)
  {
    U1=U1.colRange(0,m);
  }
  else if(U1.cols<m)
  {
    Mat U1_tmp=Mat::zeros (m,m,numericDataType);
    U1.copyTo(U1_tmp(Rect(0,0,U1.cols,U1.rows)));

    U1=U1_tmp;
  }
  ///////////////////////////////////////////////////////////////
  V1=V1T.t();
  //Updated whole data SVD
  Mat U2, V2, V2T;
  Mat& S2 =S1;

  VVLOG("  //assembling U2=[U, J]*U1;\n");
  hconcat(U, J, U2);
  U2=U2*U1;

  V2=Mat::eye(n+1,n+1, numericDataType);
  V.copyTo(V2(Rect(0,0,n,n)));
  V2=V2*V1;

  orthogonalize(U2);

  // Updating values
  U = U2;
  S = S2;
  V = V2;

  return true;
}

void MathModule::divByScalar(Mat& M, const double k)
{
  for(int i=0; i<M.cols; i++)
   {
    for(int j=0; j<M.rows; j++)
    {
      M.at<double>(i,j)/=k;
    }
  }
}

void MathModule::addTo(Mat& M1, const Mat& M2, bool isPositive)
{

  if(M1.cols != M2.cols || M1.rows != M2.rows)
  return ;

  for(int i=0; i<M1.cols; i++)
  {
    for(int j=0; j<M1.rows; j++)
    {
      if (isPositive)
      {
        M1.at<double>(i,j)=M1.at<double>(i,j)+M2.at<double>(i,j);
      }
      else
      {
        M1.at<double>(i,j)=M1.at<double>(i,j)-M2.at<double>(i,j);
      }
    }
  }
}

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

/** Sollution of the problem:
    Minimize ||Ax|| subject to ||x||=1
  */
Mat MathModule::yellowAlgorithm(const Mat& A)
{
  Mat U,S,VT,V;
  SVD::compute(A,U,S,VT, SVD::FULL_UV);
  V = VT.t();
  return V.colRange(VT.rows - 1, VT.rows);
}

