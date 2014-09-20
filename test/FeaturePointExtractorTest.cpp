#include "common.hpp"
#include "VVTestBase.hpp"
#include "FeaturePointExtractor.hpp"
#include "DataTypes.hpp"

using namespace cv;
using namespace VV;

using std::shared_ptr;

/*************************FIXTURE SECTION**************************/
class FeaturePointExtractorTest : public VVTestBase
{
 protected:

  virtual void postSetUp()
  {
     image = shared_ptr<Mat>(new Mat(Mat::zeros(Size(100, 100), CV_8U)));
     cbResult = shared_ptr<DataPoint2dVector>(NULL);
     fpe = new FeaturePointExtractor();
     callbackCalled = false;
     callbackFinished = false;
  }

  virtual void preTearDown()
  {
     delete fpe;
     callbackCalled = false;
     callbackFinished = false;
  }

  static void callback(shared_ptr<DataPoint2dVector> result,
                       void* userData)
  {
     VVLOG("[ Callback ] Start\n");
     FeaturePointExtractorTest* owner;
     owner=static_cast<FeaturePointExtractorTest*>(userData);
     owner->callbackCalled = true;
     owner->cbResult = result;

     VVLOG("[ Callback ] Finish\n");
     owner->callbackFinished = true;
  }

  shared_ptr<Mat> image;
  FeaturePointExtractor* fpe;
  bool callbackCalled;
  bool callbackFinished;
  shared_ptr<DataPoint2dVector> cbResult;
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
   ResultCode ret;
   ret=fpe->startExtraction(shared_ptr<Mat>(NULL), callback, this);
   EXPECT_EQ(ret,wrongParams);
   ret=fpe->startExtraction(image, NULL, this);
   EXPECT_EQ(ret,wrongParams);
   ret=fpe->startExtraction(shared_ptr<Mat>(NULL), NULL, this);
   EXPECT_EQ(ret,wrongParams);
   VVLOG("[          ] app finished without crash\n");
}

//test if callback is called
TEST_F(FeaturePointExtractorTest, callbackIsCalled)
{
   ASSERT_TRUE(fpe);
   ASSERT_TRUE( image->data );
   ResultCode ret;
   ret=fpe->startExtraction(image, callback, this);
   EXPECT_EQ(ret, success);

   //if callback will be called before timeout finish with success
   while(!callbackCalled);
   SUCCEED();
}

//check if result is not NULL
TEST_F(FeaturePointExtractorTest, resultNotNull)
{
    ResultCode ret;
    ret=fpe->startExtraction(image, callback, this);
    EXPECT_EQ(ret, success);
    VVLOG("[          ] Extractor launched\n");
    while(!callbackFinished);
    ASSERT_TRUE(cbResult.get());
}

//test if blank image gives no features
TEST_F(FeaturePointExtractorTest, blankImageCase)
{
    ResultCode ret;
    ret=fpe->startExtraction(image, callback, this);
    VVLOG("[          ] Extractor launched\n");
    EXPECT_EQ(ret, success);
    while(!callbackFinished);
    //check if result is not NULL
    ASSERT_TRUE(cbResult.get());
    unsigned keypointCount;
    unsigned descriptorCount;
    keypointCount =   cbResult->getPoints().size();
    descriptorCount = cbResult->getDescriptors().size().height;
    VVLOG("[          ] Check if blank image gives no features\n");
    EXPECT_EQ(keypointCount,0);
    EXPECT_EQ(descriptorCount,0);
}

//test if it finds any feature points
TEST_F(FeaturePointExtractorTest, lenaImageCase)
{
    image = shared_ptr<Mat>(new Mat(imread( "lena-gray.png", 1 )));
    ResultCode ret;
    ret=fpe->startExtraction(image, callback, this);
    EXPECT_EQ(ret, success);
    VVLOG("[          ] Extractor launched\n");
    while(!callbackFinished);
    //check if result is not NULL
    ASSERT_TRUE(cbResult.get());

    unsigned keypointCount;
    unsigned descriptorCount;
    keypointCount =   cbResult->getPoints().size();
    descriptorCount = cbResult->getDescriptors().size().height;
    VVLOG("[          ] Check if lena image gives some features\n");
    EXPECT_GT(keypointCount,10);
    EXPECT_GT(descriptorCount,10);
    EXPECT_EQ(descriptorCount,keypointCount);
}

