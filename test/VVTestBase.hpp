#include <iostream>
#include <thread>
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

//it is a base for VeniVidi tests
class VVTestBase : public testing::Test
{
 protected:

  virtual void postSetUp(){}

  virtual void SetUp();


  virtual void preTearDown(){}

  virtual void TearDown();


  static void timeoutThreadRoutine(bool *testFinishedLFlag);

  static bool matsEqual(cv::Mat mat1, cv::Mat mat2, double epsilon);

  std::thread *  timeoutThread;
  bool * testFinishedLFlag;
};
