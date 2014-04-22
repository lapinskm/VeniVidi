#include <stdio.h>

#include "gtest/gtest.h"
#include "FeaturePointExtractor.hpp"
#include "ImageFeaturePoints.hpp"


using namespace cv;

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
  }

  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  virtual void TearDown()
  {
     cout<<"[ TearDown ]\n";
     delete fpe;
     callbackCalled=false;
  }

  static void callback(void* result,void* userData)
  {
     cout<<"[          ] InsideCallback\n";
     FeaturePointExtractorTest* owner;
     owner=static_cast<FeaturePointExtractorTest*>(userData);
    // owner->cbResult = static_cast<ImageFeaturePoints*>(result);
     owner->callbackCalled=true;
     owner->callbackFinished=true;
  }

  // Declares the variables your tests want to use.
  Mat image;
  FeaturePointExtractor* fpe;
  bool callbackCalled;
  bool callbackFinished;
  ImageFeaturePoints* cbResult;
  //int foo;
  //int bar;
};

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

// Tests the default c'tor.
TEST_F(FeaturePointExtractorTest, DefaultConstructor) 
{
   ASSERT_TRUE(fpe);
}

//test if callback is called
TEST_F(FeaturePointExtractorTest, CallbackIsCalled)
{
   cout<<"[ Test body] \n";
   ASSERT_TRUE(fpe);
   ASSERT_TRUE( image.data );

   fpe->startExtraction(&image, callback, this);
   cout<<"[          ] Extraction launched\n";
   /*FIXME: Find more realible method than waiting some amount
   of time before checking if callback was called*/
   cout<<"[          ] waiting 2 sec for finish extraction\n";
   sleep(2);
   ASSERT_TRUE(callbackCalled);
}

//test if it finds any feature points
TEST_F(FeaturePointExtractorTest, LenaImageCase)
{
    image = imread( "lena-gray.png", 1 );
    fpe->startExtraction(&image, callback, this);
    while(!callbackCalled);
}

TEST_F(FeaturePointExtractorTest, BlankImageCase)
{

}



