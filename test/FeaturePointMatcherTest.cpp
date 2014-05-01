#include <iostream>
#include <thread>
#include <mutex>
#include <gtest/gtest.h>

#include "common.hpp"
#include "FeaturePointMatcher.hpp"

using namespace cv;

mutex FinishTestMutex;

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
     initTestData();
     cout<<"[ SetUp    ]\n";
     fpm=new FeaturePointMatcher();
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
     delete fpm;
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
     FeaturePointMatcherTest* owner;
     owner=static_cast<FeaturePointMatcherTest*>(userData);
     owner->cbResult = static_cast<std::vector< DMatch >*>(result);
     owner->callbackCalled=true;
     owner->callbackFinished=true;
  }

  void findDescriptors(const char* filename, Mat* descriptors)
  {
    Mat image;
    Ptr<FeatureDetector>     detector=FeatureDetector::create("FAST");
    Ptr<DescriptorExtractor> extractor=DescriptorExtractor::create("FREAK");
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

  static void timeoutThreadRoutine(void* data)
  {
    FeaturePointMatcherTest* owner;
    owner = static_cast < FeaturePointMatcherTest* >(data);
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

  Mat empty_descr;
  Mat lena_descr;
  Mat lena_full_descr;
  Mat lena_120deg_descr;
  Mat lena_scalled_descr;

  FeaturePointMatcher* fpm;

  int timeoutTime = 1;
  thread *  timeoutThread;
  bool * testFinished;

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

TEST_F(FeaturePointMatcherTest, lenaRotatedCase)
{
}

TEST_F(FeaturePointMatcherTest, lenaResizedCase)
{
}

TEST_F(FeaturePointMatcherTest, lenaResizedRotatedCase)
{
}

