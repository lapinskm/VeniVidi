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

