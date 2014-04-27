#include <stdio.h>
#include <gtest/gtest.h>
#include "common.hpp"
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
     delete fpe;
     callbackCalled=false;
  }

  static void callback(void* result,void* userData)
  {
     cout<<"[          ] InsideCallback\n";
     FeaturePointExtractorTest* owner;
     owner=static_cast<FeaturePointExtractorTest*>(userData);
     owner->cbResult = static_cast<ImageFeaturePoints*>(result);
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
   cout<<"[          ] Extractor launched\n";
   /*FIXME: Find more realible method than waiting some amount
   of time before checking if callback was called*/
   cout<<"[          ] waiting 2 sec for finish extraction\n";
   sleep(2);
   EXPECT_TRUE(callbackCalled);
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

