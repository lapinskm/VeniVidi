#include <memory>
#include <thread>
#include <chrono>
#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

//it is a base for VeniVidi tests
class VVTestBase : public testing::Test
{
 protected:

  void waitUntil(bool& condition);

  virtual void postSetUp(){}

  virtual void SetUp();


  virtual void preTearDown(){}

  virtual void TearDown();


  static void timeoutThreadRoutine(std::shared_ptr <bool>  testFinishedFlag);

  static bool matsEqual(cv::Mat mat1, cv::Mat mat2, double epsilon);

  std::thread *  timeoutThread;
  std::shared_ptr <bool>  testFinished;
  std::chrono::milliseconds checkInterval;
};
