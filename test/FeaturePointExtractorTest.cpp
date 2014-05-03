#include "common.hpp"
#include "VVTestBase.hpp"
#include "FeaturePointExtractor.hpp"
#include "ImageFeaturePoints.hpp"

using namespace cv;

/*************************FIXTURE SECTION**************************/
class FeaturePointExtractorTest : public VVTestBase
{
 protected:

  virtual void postSetUp()
  {
     image = Mat::zeros(Size(100,100), CV_8U);
     fpe=new FeaturePointExtractor();
     callbackCalled=false;
     callbackFinished=false;
  }

  virtual void preTearDown()
  {
     image.release();
     delete fpe;
     callbackCalled=false;
     callbackFinished=false;
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

  Mat image;
  FeaturePointExtractor* fpe;
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

