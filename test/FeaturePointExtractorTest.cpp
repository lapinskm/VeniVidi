#include <iostream>
#include <thread>
#include <mutex>
#include <gtest/gtest.h>

#include "common.hpp"
#include "FeaturePointExtractor.hpp"
#include "ImageFeaturePoints.hpp"

using namespace cv;

static mutex FinishTestMutex;
static const int timeoutTime = 3; //value in seconds

// To use a test fixture, derive a class from testing::Test.
class FeaturePointExtractorTest : public testing::Test
{
 protected:  // You should make the members protected s.t. they can be
             // accessed from sub-classes.

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp()
  {
     VVLOG("[ SetUp    ]\n");
     FinishTestMutex.lock();
     image = Mat::zeros(Size(100,100), CV_8U);
     FinishTestMutex.unlock();
     fpe=new FeaturePointExtractor();
     callbackCalled=false;
     callbackFinished=false;

     testFinishedFlag=new bool(false);
     timeoutThread= new thread(&timeoutThreadRoutine,testFinishedFlag);
     timeoutThread->detach();

     VVLOG("[ Test body] \n");
  }

  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  virtual void TearDown()
  {
     FinishTestMutex.lock();
     VVLOG("[ TearDown ]\n");
     image.release();
     delete fpe;
     delete timeoutThread;
     timeoutThread=NULL;
     callbackCalled=false;
     callbackFinished=false;
     //if not testFinishedFlag timeout not happened
     if (!*testFinishedFlag)
     {
        //we need set it true for timeout to know test finished
       *testFinishedFlag=true;
     }
     //else if finished that mean timeout was already been
     else
     {
       //and you can delete this part without worring abut timeout
       delete testFinishedFlag;
     }
     FinishTestMutex.unlock();
  }

  static void callback(void* result,void* userData)
  {
     VVLOG("[ Callback ] Start\n");
     FeaturePointExtractorTest* owner;
     owner=static_cast<FeaturePointExtractorTest*>(userData);
     owner->callbackCalled=true;
     owner->cbResult = static_cast<ImageFeaturePoints*>(result);

     VVLOG("[ Callback ] Finish\n");
     owner->callbackFinished=true;
  }

  static void timeoutThreadRoutine(bool * testFinishedFlag)
  {
    VVLOG("[ TIMEOUT  ] Timer started. %ds remainig.\n",timeoutTime);
    sleep (timeoutTime);
    FinishTestMutex.lock();

    //if not testFinishedFlag tearDown not happened
    if ( !*testFinishedFlag )
    {
       //so we set it true and call FAIL to start tearDown
       VVLOG("[ TIMEOUT  ] Time is over\n");
       *testFinishedFlag = true;
       FinishTestMutex.unlock();
       FAIL();
    }
    //if tearDown happened already we can delete this variable
    else
    {
      delete testFinishedFlag;
      FinishTestMutex.unlock();
    }
  }

  // Declares the variables your tests want to use.
  Mat image;
  FeaturePointExtractor* fpe;

  thread *  timeoutThread;
  bool * testFinishedFlag;

  bool callbackCalled;
  bool callbackFinished;
  ImageFeaturePoints* cbResult;
};


/**************************TESTS SECTION***************************/
// Tests the default c'tor.
TEST_F(FeaturePointExtractorTest, defaultConstructor)
{
   ASSERT_TRUE(fpe);
}

TEST_F(FeaturePointExtractorTest, nullParameters)
{
   ASSERT_TRUE(fpe);
   VVResultCode ret;
   ret=fpe->startExtraction(NULL, callback, this);
   EXPECT_EQ(ret,vVWrongParams);
   ret=fpe->startExtraction(&image, NULL, this);
   EXPECT_EQ(ret,vVWrongParams);
   ret=fpe->startExtraction(NULL, NULL, this);
   EXPECT_EQ(ret,vVWrongParams);
   VVLOG("[          ] app finished without crash\n");
}

//test if callback is called
TEST_F(FeaturePointExtractorTest, callbackIsCalled)
{
   ASSERT_TRUE(fpe);
   ASSERT_TRUE( image.data );
   VVResultCode ret;
   ret=fpe->startExtraction(&image, callback, this);
   EXPECT_EQ(ret, vVSuccess);

   //if callback will be called before timeout finish with success
   while(!callbackCalled);
   SUCCEED();
}

//check if result is not NULL
TEST_F(FeaturePointExtractorTest, resultNotNull)
{
    VVResultCode ret;
    ret=fpe->startExtraction(&image, callback, this);
    EXPECT_EQ(ret, vVSuccess);
    VVLOG("[          ] Extractor launched\n");
    while(!callbackFinished);
    ASSERT_TRUE(cbResult);
}

//test if blank image gives no features
TEST_F(FeaturePointExtractorTest, blankImageCase)
{
    VVResultCode ret;
    ret=fpe->startExtraction(&image, callback, this);
    VVLOG("[          ] Extractor launched\n");
    EXPECT_EQ(ret, vVSuccess);
    while(!callbackFinished);
    //check if result is not NULL
    ASSERT_TRUE(cbResult);
    unsigned keypointCount;
    unsigned descriptorCount;
    keypointCount=cbResult->keypoints.size();
    descriptorCount=cbResult->descriptors.size().height;
    VVLOG("[          ] Check if blank image gives no features\n");
    EXPECT_EQ(keypointCount,0);
    EXPECT_EQ(descriptorCount,0);
}

//test if it finds any feature points
TEST_F(FeaturePointExtractorTest, lenaImageCase)
{
    image.release();//release default one
    image = imread( "lena-gray.png", 1 );
    VVResultCode ret;
    ret=fpe->startExtraction(&image, callback, this);
    EXPECT_EQ(ret, vVSuccess);
    VVLOG("[          ] Extractor launched\n");
    while(!callbackFinished);
    //check if result is not NULL
    ASSERT_TRUE(cbResult);

    unsigned keypointCount;
    unsigned descriptorCount;
    keypointCount=cbResult->keypoints.size();
    descriptorCount=cbResult->descriptors.size().height;
    VVLOG("[          ] Check if lena image gives some features\n");
    EXPECT_GT(keypointCount,10);
    EXPECT_GT(descriptorCount,10);
    EXPECT_EQ(descriptorCount,keypointCount);
}

