#include <stdio.h>
#include <gtest/gtest.h>
#include "common.hpp"
#include "FeaturePointMatcher.hpp"

using namespace cv;


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
     cout<<"[ SetUp    ]\n";
     fpm=new FeaturePointMatcher();
     callbackCalled=false;
     callbackFinished=false;
     cout<<"[ Test body] \n";
  }

  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  virtual void TearDown()
  {
     cout<<"[ TearDown ]\n";
     image.release();
     delete fpm;
     callbackCalled=false;
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

  // Declares the variables your tests want to use.
  Mat image;
  FeaturePointMatcher* fpm;
  bool callbackCalled;
  bool callbackFinished;
  std::vector< DMatch >* cbResult;
  //int foo;
  //int bar;
};

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

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
   ret=fpm->startMatching(NULL, &descr, callback, this);
   EXPECT_EQ(ret,vVWrongParams);
   ret=fpm->startMatching(&descr, NULL, callback, this);
   EXPECT_EQ(ret,vVWrongParams);
   ret=fpm->startMatching(NULL, NULL, callback, this);
   EXPECT_EQ(ret,vVWrongParams);
   ret=fpm->startMatching(NULL, NULL, NULL, this);
   EXPECT_EQ(ret,vVWrongParams);
   ret=fpm->startMatching(&descr, &descr, NULL, this);
   EXPECT_EQ(ret,vVWrongParams);
}

//test if callback is called
TEST_F(FeaturePointMatcherTest, callbackIsCalled)
{
}

//check if result is not NULL
TEST_F(FeaturePointMatcherTest, resultNotNull)
{
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

