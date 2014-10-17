#include <mutex>
#include "common.hpp"
#include "VVTestBase.hpp"

using std::thread;
using std::mutex;
using cv::Mat;

static mutex FinishTestMutex;
static const int timeoutTime = 1;

void VVTestBase::waitUntil(bool& condition)
{
  while ( !condition )
  {
    std::this_thread::sleep_for( checkInterval );
  }
}

void VVTestBase::SetUp()
{
   VVLOG("[ SetUp    ]\n");
   checkInterval = std::chrono::milliseconds (100);
   testFinished= std::shared_ptr <bool> (new bool(false));
   //launch timeout thread
   timeoutThread= new thread(&timeoutThreadRoutine, testFinished);
   timeoutThread->detach();
   VVLOG("[ TIMEOUT  ] Timer started. %ds remainig.\n", timeoutTime);

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

   //We need set it true, to let timeout know test already finished.
   *testFinished.get()=true;

   FinishTestMutex.unlock();
}

void VVTestBase::timeoutThreadRoutine(std::shared_ptr <bool> testFinished)
{
  //Give some time to perform test.
  sleep (timeoutTime);

  //Time is over. Check if test finished.
  FinishTestMutex.lock();
  //if not testFinished,
  if ( ! *testFinished.get() )
  {
     //we call FAIL because it took too much time.
     VVLOG("[ TIMEOUT  ] Time is over\n");
     FinishTestMutex.unlock();
     FAIL();
  }
  FinishTestMutex.unlock();
}

bool VVTestBase::matsEqual(Mat mat1, Mat mat2, double epsilon)
{
  //Check if matrices have the same size.
  if ( mat1.cols != mat2.cols || mat1.rows != mat2.rows)
  {
    return false;
  }
  //Make one sign diff matrix
  Mat diff=abs(mat1-mat2);
  for (int i=0; i<diff.cols; i++)
  {
    for (int j=0; j<diff.rows; j++)
    {
      //and check if each element is smaller than epsilon.
      if (diff.at<double>(i,j)>epsilon)
      {
        return false;
      }
    }
  }
  return true;
}

