#include <cmath>
#include "common.hpp"
#include "VVTestBase.hpp"
#include "MathModule.hpp"

using namespace cv;

/*************************FIXTURE SECTION**************************/
class MathModuleTest : public VVTestBase
{
 protected:

  virtual void postSetUp()
  {
    initTestData();
  }
  virtual void preTearDown()
  {}
  void initTestData()
  {
   double Mdata[6][4] =  {49,   4,  -4,  22,
                           2,  56, -45,   4,
                          33,  -4,   2,   4,
                          24,  45,  45,  11,
                         -31,  17,   1,   5,
                          11,   3,   9,  -2};

    M = Mat(6 ,4 ,numericDataType, Mdata);
    M.copyTo(MFull);
    MPart = MFull.colRange(0,3);

    C = MFull.colRange(3,4);

    SVD::compute(MPart,S,U,V,SVD::FULL_UV);

    V = V.t();
    S = Mat::diag(S);

    Mat tmp = Mat(6, 3, numericDataType, Scalar(0));
    S.copyTo(tmp(Rect(0, 0, S.cols, S.rows)));
    S=tmp;
  }
  Mat V,S,U,C,MFull,MPart,M;
};

/**************************TESTS SECTION***************************/


TEST_F(MathModuleTest, vectorProjection)
{
   Mat V1 = Mat(2, 1, CV_64FC1);
   V1.at<double>(0, 0)=1;
   V1.at<double>(1, 0)=0;

   Mat V2 = Mat(2, 1, CV_64FC1);
   V2.at<double>(0, 0)=2;
   V2.at<double>(1, 0)=1;

   Mat V3=MathModule::vectorProjection(V2,V1);
   EXPECT_EQ(2, V3.at<double>(0, 0));
   EXPECT_EQ(0, V3.at<double>(1, 0));
}

TEST_F(MathModuleTest, orthogonalize)
{
  Mat M=Mat(2, 2, CV_64FC1);
  M.at<double>(0, 0)=1;
  M.at<double>(1, 0)=0;
  M.at<double>(0, 1)=2;
  M.at<double>(1, 1)=1;

  MathModule::orthogonalize(M);
  EXPECT_EQ(1, M.at<double>(0, 0));
  EXPECT_EQ(0, M.at<double>(1, 0));
  EXPECT_EQ(0, M.at<double>(0, 1));
  EXPECT_EQ(1, M.at<double>(1, 1));
}

/** check if algorithm keeps ||X|| = 1 constraint
 */
TEST_F(MathModuleTest, yellowAlgorithmResultNormIsOne)
{
  Mat A(10, 10,CV_64FC1);
  randu(A, Scalar(-100), Scalar(100));
  EXPECT_GE(VVEpsilon, 1 - norm(MathModule::yellowAlgorithm(A)));
}

/*Check if the retult is minimal. Compare result to
 *precalculated via wolfram alpha values
 */
TEST_F(MathModuleTest, yellowAlgorithmResultIsMinimal)
{
  Mat A = Mat::eye(2, 2, CV_64FC1);
  A.at<double>(0, 1) = 1;
  A.at<double>(1, 1) = 2;
  Mat X = MathModule::yellowAlgorithm(A);

  Mat AX=A*X;
  double x1=X.at<double>(0);
  double x2=X.at<double>(1);
  
  EXPECT_NE(copysign(1.0, x1), copysign(1.0, x2));
  EXPECT_GE(VVEpsilon, abs(abs(x1)  - 0.9732489894677301637880577480));
  EXPECT_GE(VVEpsilon, abs(abs(x2)  - 0.2297529205473611835239373694));
  EXPECT_GE(VVEpsilon, abs(norm(AX) - 0.8740320488976421415986024101));
}

/***************************negative tests**************************/
//wrong input params - matrix sizes
TEST_F(MathModuleTest, updateSVD_V_not_sqare )
{
  V = Mat(3, 4, numericDataType);
  EXPECT_FALSE(MathModule::updateSVD(U,S,V,C));
}

TEST_F(MathModuleTest, updateSVD_U_not_sqare )
{
  U = Mat(6, 7, numericDataType);
  EXPECT_FALSE(MathModule::updateSVD(U,S,V,C));
}

TEST_F(MathModuleTest, updateSVD_U_S_mismatch )
{
  U = Mat(3, 3, numericDataType);
  EXPECT_FALSE(MathModule::updateSVD(U,S,V,C));
}

TEST_F(MathModuleTest, updateSVD_V_S_mismatch )
{
  V = Mat(2, 2, numericDataType);
  EXPECT_FALSE(MathModule::updateSVD(U,S,V,C));
}

TEST_F(MathModuleTest, updateSVD_C_not_vector )
{
  C= Mat(6, 2, numericDataType);
  EXPECT_FALSE(MathModule::updateSVD(U,S,V,C));
}

TEST_F(MathModuleTest, updateSVD_C_S_mismatch )
{
  C = Mat(3, 1, numericDataType);
  EXPECT_FALSE(MathModule::updateSVD(U,S,V,C));
}

/***************************positive tests**************************/

/* check if it is really decomosition and if
 * it multiplies together to the result a matrix
 * which was used to create these matrices
 */
TEST_F(MathModuleTest, updateSVD_is_decomposition)
{
  VVLOG("[ Test Body] MathModule::updateSVD()\n");
  ASSERT_TRUE(MathModule::updateSVD(U,S,V,C));
  //TODO: remove diagonal matrix composing after
  // adding it to the algorithm
  Mat S_full=Mat::zeros(6,4,numericDataType);
  Mat::diag(S).copyTo(S_full(Rect(0,0,4,4)));
  Mat MFullReconstructed = U * S_full*V.t();

  VVLOG("Folowing matrices should be equal:\n");
  VV_PRINT_MAT(MFull);
  VV_PRINT_MAT(MFullReconstructed);

  EXPECT_TRUE(matsEqual(MFullReconstructed, MFull,VVEpsilon));

}

 /*Check if result U matrix stay orthogonal. If this condition
  * is fulfilled its transpositions are its inverse.
  */

TEST_F(MathModuleTest, updateSVD_U_stay_orthogonal )
{
  ASSERT_TRUE(matsEqual(U*U.t(),
                      Mat::eye(U.cols, U.rows, numericDataType),
                      VVEpsilon));

  VVLOG("[ Test Body] MathModule::updateSVD()\n");
  ASSERT_TRUE(MathModule::updateSVD(U,S,V,C));

  EXPECT_TRUE(matsEqual(U*U.t(),
                        Mat::eye(U.cols, U.rows, numericDataType),
                        VVEpsilon));

}

/*Check if result V matrix stay orthogonal. If this condition
 * is fulfilled its transpositions are its inverse.
 */
TEST_F(MathModuleTest, updateSVD_V_stay_orthogonal )
{
  ASSERT_TRUE(matsEqual(V*V.t(),
                        Mat::eye(V.cols, V.rows, numericDataType),
                        VVEpsilon));


  VVLOG("[ Test Body] MathModule::updateSVD()\n");
  ASSERT_TRUE(MathModule::updateSVD(U,S,V,C));
  EXPECT_TRUE(matsEqual(V*V.t(),
                        Mat::eye(V.cols, V.rows, numericDataType),
                        VVEpsilon));


}


/*check if obtained matrices are the same as the
 * ones generated by standard opencv SVD solver
 */
TEST_F(MathModuleTest, updateSVD_compare_to_build_in )
{
  //reference result provided by standard OpenCV algorithm
  Mat ocvS, ocvU, ocvVT;
  VVLOG("[ Test Body] SVD::compute()\n");
  SVD::compute(MFull, ocvS, ocvU, ocvVT, SVD::FULL_UV);

  //tested function
  VVLOG("[ Test Body] MathModule::updateSVD()\n");
  ASSERT_TRUE(MathModule::updateSVD(U,S,V,C));
  //printing it's reasults

  VVLOG("Folowing pairs of matrices should be equal:\n");

  VV_PRINT_MAT(U);
  VV_PRINT_MAT(ocvU);
  EXPECT_TRUE(matsEqual(ocvU,U,VVEpsilon));

  VV_PRINT_MAT(S);
  VV_PRINT_MAT(ocvS);
  EXPECT_TRUE(matsEqual(ocvS,S,VVEpsilon));

  VV_PRINT_MAT(V);
  VV_PRINT_MAT(ocvVT.t());
  EXPECT_TRUE(matsEqual(ocvVT.t(),V, VVEpsilon));
}
