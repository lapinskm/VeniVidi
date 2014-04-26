#include <stdio.h>
#include <gtest/gtest.h>
#include "FeaturePointMatcher.hpp"
#include "ImageFeaturePoints.hpp"

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
     owner->cbResult = static_cast<ImageFeaturePoints*>(result);
     owner->callbackCalled=true;
     owner->callbackFinished=true;
  }

  // Declares the variables your tests want to use.
  Mat image;
  FeaturePointMatcher* fpm;
  bool callbackCalled;
  bool callbackFinished;
  ImageFeaturePoints* cbResult;
  //int foo;
  //int bar;
};

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

// Tests the default c'tor.
TEST_F(FeaturePointMatcherTest, defaultConstructor)
{
}

TEST_F(FeaturePointMatcherTest, nullParameters)
{
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

