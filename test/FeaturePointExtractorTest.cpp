#include <iostream>
#include <thread>
#include <mutex>
#include <gtest/gtest.h>

#include "common.hpp"
#include "FeaturePointExtractor.hpp"
#include "ImageFeaturePoints.hpp"

using namespace cv;

static mutex FinishTestMutex;

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
     cout<<"[ SetUp    ]\n";
     image = Mat::zeros(Size(100,100), CV_8U);
     fpe=new FeaturePointExtractor();
     callbackCalled=false;
     callbackFinished=false;

     testFinished=new bool(false);
     timeoutThread= new thread(&timeoutThreadRoutine,this);
     timeoutThread->detach();

     cout<<"[ Test body] \n";
  }

  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  virtual void TearDown()
  {
     FinishTestMutex.lock();
     cout<<"[ TearDown ]\n";
     image.release();
     delete fpe;
     delete timeoutThread;
     timeoutThread=NULL;
     callbackCalled=false;
     callbackFinished=false;

     if (!*testFinished)
       *testFinished=true;
     else
       delete testFinished;

     FinishTestMutex.unlock();
  }

  static void callback(void* result,void* userData)
  {
     cout<<"[          ] InsideCallback\n";
     FeaturePointExtractorTest* owner;
     owner=static_cast<FeaturePointExtractorTest*>(userData);
     owner->callbackCalled=true;

     owner->cbResult = static_cast<ImageFeaturePoints*>(result);

     owner->callbackFinished=true;
  }

  static void timeoutThreadRoutine(void* data)
  {
    FeaturePointExtractorTest* owner;
    owner = static_cast < FeaturePointExtractorTest* >(data);
    bool * testFinished = owner -> testFinished;

    sleep (owner->timeoutTime);
    FinishTestMutex.lock();

    if ( !*testFinished )
    {
       *testFinished = true;
       FAIL();
    }
    else
    {
      delete testFinished;
    }
    FinishTestMutex.unlock();
  }

  // Declares the variables your tests want to use.
  Mat image;
  FeaturePointExtractor* fpe;

  int timeoutTime = 1;
  thread *  timeoutThread;
  bool * testFinished;

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
   cout<<"[          ] app finished without crash\n";
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
    cout<<"[          ] Extractor launched\n";
    while(!callbackCalled);
    ASSERT_TRUE(cbResult);
}

//test if blank image gives no features
TEST_F(FeaturePointExtractorTest, blankImageCase)
{
    VVResultCode ret;
    ret=fpe->startExtraction(&image, callback, this);
    cout<<"[          ] Extractor launched\n";
    EXPECT_EQ(ret, vVSuccess);
    while(!callbackCalled);
    //check if result is not NULL
    ASSERT_TRUE(cbResult);
    unsigned keypointCount;
    unsigned descriptorCount;
    keypointCount=cbResult->keypoints.size();
    descriptorCount=cbResult->descriptors.size().height;
    cout<<"[          ] Check if blank image gives no features\n";
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
    cout<<"[          ] Extractor launched\n";
    while(!callbackCalled);
    //check if result is not NULL
    ASSERT_TRUE(cbResult);

    unsigned keypointCount;
    unsigned descriptorCount;
    keypointCount=cbResult->keypoints.size();
    descriptorCount=cbResult->descriptors.size().height;
    cout<<"[          ] Check if lena image gives some features\n";
    EXPECT_GT(keypointCount,10);
    EXPECT_GT(descriptorCount,10);
    EXPECT_EQ(descriptorCount,keypointCount);
}

