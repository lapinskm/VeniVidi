#include <iostream>
#include <thread>
#include <gtest/gtest.h>

//it is a base for VeniVidi tests
class VVTestBase : public testing::Test
{
 protected:
  
  virtual void postSetUp(){}

  virtual void SetUp();
  

  virtual void preTearDown(){}

  virtual void TearDown();
 

  static void timeoutThreadRoutine(bool *testFinishedLFlag);
  

  std::thread *  timeoutThread;
  bool * testFinishedLFlag;
};
