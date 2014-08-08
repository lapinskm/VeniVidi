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
  }

  virtual void preTearDown()
  {
  }
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

