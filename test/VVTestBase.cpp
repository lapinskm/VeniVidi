#include <mutex>

#include "common.hpp"
#include "VVTestBase.hpp"

using std::thread;
using std::mutex;

static mutex FinishTestMutex;
static const int timeoutTime = 1;

void VVTestBase::SetUp()
{
   VVLOG("[ SetUp    ]\n");
   testFinishedLFlag=new bool(false);
   //launch timeout thread
   timeoutThread= new thread(&timeoutThreadRoutine,testFinishedLFlag);
   timeoutThread->detach();

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
  VVLOG("[ TIMEOUT  ] Timer started. %ds remainig.\n",timeoutTime);

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

