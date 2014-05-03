#include <iostream>
#include <thread>
#include <mutex>
#include <gtest/gtest.h>

#include "common.hpp"
#include "FeaturePointMatcher.hpp"

using namespace cv;

static mutex FinishTestMutex;
static const int timeoutTime = 3; //value in seconds

// To use a test fixture, derive a class from testing::Test.
class FeaturePointMatcherTest : public testing::Test
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
     initTestData();
     FinishTestMutex.unlock();
     fpm=new FeaturePointMatcher();
     callbackCalled=false;
     callbackFinished=false;

     testFinishedLFlag=new bool(false);
     timeoutThread= new thread(&timeoutThreadRoutine,testFinishedLFlag);
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
     delete fpm;
     delete timeoutThread;
     timeoutThread=NULL;
     callbackCalled=false;
     callbackFinished=false;

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

  static void callback(void* result,void* userData)
  {
     VVLOG("[ Callback ] Start\n");
     FeaturePointMatcherTest* owner;
     owner=static_cast<FeaturePointMatcherTest*>(userData);
     owner->callbackCalled=true;
     owner->cbResult = static_cast<std::vector< DMatch >*>(result);

     VVLOG("[ Callback ] Finish\n");
     owner->callbackFinished=true;
  }

  void findDescriptors(const char* filename, Mat* descriptors)
  {
    Mat image;

    Ptr<FeatureDetector>     detector=FeatureDetector::create("ORB");
    Ptr<DescriptorExtractor> extractor=DescriptorExtractor::create("ORB");
    vector<KeyPoint> keypoints;

    image = imread( filename, 1 );
    detector  -> detect  (image, keypoints);
    extractor -> compute (image, keypoints,*descriptors);
  }

  void initTestData()
  {
    findDescriptors("lena-gray.png"        , & lena_descr);
    findDescriptors("lena-gray-120deg.png" , & lena_120deg_descr);
    empty_descr=Mat::zeros(1, 1, CV_32F);
  }

  static void timeoutThreadRoutine(bool *testFinishedLFlag)
  {
    VVLOG("[ TIMEOUT  ] Timer started. %ds remainig.\n",timeoutTime);

    sleep (timeoutTime);
    FinishTestMutex.lock();

    //if not testFinishedLFlag tearDown not happened
    if ( !*testFinishedLFlag )
    {
       //so we set it true and call FAIL to start tearDown
       VVLOG("[ TIMEOUT  ] Time is over\n");
       *testFinishedLFlag = true;
       FinishTestMutex.unlock();
       FAIL();
    }
    //if tearDown happened already we can delete this variable
    else
    {
      delete testFinishedLFlag;
      FinishTestMutex.unlock();
    }
  }

  // Declares the variables your tests want to use.

  Mat empty_descr;
  Mat lena_descr;
  Mat lena_full_descr;
  Mat lena_120deg_descr;
  Mat lena_scalled_descr;

  FeaturePointMatcher* fpm;

  thread *  timeoutThread;

  bool * testFinishedLFlag;

  bool callbackCalled;
  bool callbackFinished;
  std::vector< DMatch >* cbResult;
};

/**************************TESTS SECTION***************************/
// Tests the default c'tor.
TEST_F(FeaturePointMatcherTest, defaultConstructor)
{
  EXPECT_TRUE(fpm);
}

TEST_F(FeaturePointMatcherTest, nullParameters)
{
  ASSERT_TRUE(fpm);
  VVResultCode ret;
  Mat descr=Mat::zeros(1, 1, CV_32F);
  ret=fpm->startMatching(NULL, &empty_descr, callback, this);
  EXPECT_EQ(ret,vVWrongParams);
  ret=fpm->startMatching(&empty_descr, NULL, callback, this);
  EXPECT_EQ(ret,vVWrongParams);
  ret=fpm->startMatching(NULL, NULL, callback, this);
  EXPECT_EQ(ret,vVWrongParams);
  ret=fpm->startMatching(NULL, NULL, NULL, this);
  EXPECT_EQ(ret,vVWrongParams);
  ret=fpm->startMatching(&empty_descr, &empty_descr, NULL, this);
  EXPECT_EQ(ret,vVWrongParams);
}

//test if callback is called
TEST_F(FeaturePointMatcherTest, callbackIsCalled)
{
  ASSERT_TRUE(fpm);
  VVResultCode ret;
  ret=fpm->startMatching(&empty_descr, &empty_descr, callback, this);
  EXPECT_EQ(ret,vVSuccess);
  //if callback will be called before timeout finish with success
  while (!callbackCalled);

  SUCCEED();
}

//check if result is not NULL
TEST_F(FeaturePointMatcherTest, resultNotNull)
{
  ASSERT_TRUE(fpm);
  VVResultCode ret;
  ret=fpm->startMatching(&empty_descr, &empty_descr, callback, this);
  EXPECT_EQ(ret,vVSuccess);
  while (!callbackFinished);
  EXPECT_TRUE(cbResult);
}

TEST_F(FeaturePointMatcherTest, sameDescriptors)
{
  ASSERT_TRUE(fpm);
  VVResultCode ret;
  ret=fpm->startMatching(&lena_descr, &lena_descr, callback, this);
  EXPECT_EQ(ret,vVSuccess);
  while (!callbackFinished);
  //faill when null
  ASSERT_TRUE(cbResult);
  VVLOG("[ result   ] result size is %ld\n",cbResult->size());
  //amount of matches should be the same as descriptors
  EXPECT_EQ(lena_descr.size().height ,cbResult->size());
}

TEST_F(FeaturePointMatcherTest, 120Rot)
{
   ASSERT_TRUE(fpm);
   VVResultCode ret;
   ret=fpm->startMatching(&lena_descr, &lena_120deg_descr, callback, this);
   EXPECT_EQ(ret,vVSuccess);
   while (!callbackFinished);
   //faill when null
   ASSERT_TRUE(cbResult);
   VVLOG("[ result   ] result size is %ld\n",cbResult->size());
   //amount of matches should be the same as descriptors
   EXPECT_GT( cbResult->size(), 10);
}

