#include <mutex>
#include "common.hpp"
#include "VVTestBase.hpp"

using std::thread;
using std::mutex;
using cv::Mat;

static mutex FinishTestMutex;
static const int timeoutTime = 1;

void VVTestBase::SetUp()
{
   VVLOG("[ SetUp    ]\n");
   testFinishedLFlag=new bool(false);
   //launch timeout thread
   timeoutThread= new thread(&timeoutThreadRoutine,testFinishedLFlag);
   timeoutThread->detach();
   VVLOG("[ TIMEOUT  ] Timer started. %ds remainig.\n",timeoutTime);

   VVLOG("[ postSetUp] \n");
   postSetUp();

   VVLOG("[ Test body] \n");
}

void VVTestBase::TearDown()
{
   VVLOG("[preTearDwn]\n");
   preTearDown();

   FinishTestMutex.lock();
   VVLOG("[ TearDown ]\n");
   delete timeoutThread;
   timeoutThread=NULL;

   //if not testFinished timeout not happened
   if (!*testFinishedLFlag)
   {
      //we need set it true for timeout to know test finished
      *testFinishedLFlag=true;
   }
   //else if finished that mean timeout was already been
   else
   {
     //and you can delete this part without worring abut timeout
     delete testFinishedLFlag;
   }

   FinishTestMutex.unlock();
}

void VVTestBase::timeoutThreadRoutine(bool *testFinishedLFlag)
{
  sleep (timeoutTime);
  FinishTestMutex.lock();

  //if not testFinishedLFlag, tearDown not happened
  if ( !*testFinishedLFlag )
  {
     //so we set it true and call FAIL to start tearDown
     VVLOG("[ TIMEOUT  ] Time is over\n");
     *testFinishedLFlag = true;
     FinishTestMutex.unlock();
     FAIL();
  }
  //if tearDown happened already
  else
  {
    //we should delete this variable
    delete testFinishedLFlag;
    FinishTestMutex.unlock();
  }
}

bool VVTestBase::matsEqual(Mat mat1, Mat mat2, double epsilon)
{
  if ( mat1.cols != mat2.cols || mat1.rows != mat2.rows)
  {
    return false;
  }
  Mat diff=abs(mat1-mat2);
  for (int i=0; i<diff.cols; i++)
  {
    for (int j=0; j<diff.rows; j++)
    {
      if (diff.at<double>(i,j)>epsilon)
      {
        return false;
      }
    }
  }
  return true;
}
