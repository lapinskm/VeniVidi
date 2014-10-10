#include <chrono>
#include <thread>

#include "common.hpp"
#include "VVTestBase.hpp"
#include "FeaturePointExtractor.hpp"
#include "DataTypes.hpp"

using namespace cv;
using namespace VV;

/*************************FIXTURE SECTION**************************/
class FeaturePointExtractorTest :public FeaturePointExtractorUser
                                ,public VVTestBase
{
 protected:

  virtual void postSetUp()
  {
     dura = std::chrono::milliseconds (100);
     image = Mat::zeros(Size(100, 100), CV_8U);
     cbResult = NULL;
     onExtactionFinishedCalled = false;
     onExtactionFinishedEnded = false;
  }

  virtual void preTearDown()
  {
     if(cbResult) delete cbResult;
  }

  virtual void onExtactionFinished(DataPoint2dVector* result)
  {
     VVLOG("[ Callback ] Start\n");
     onExtactionFinishedCalled = true;
     cbResult = result;

     VVLOG("[ Callback ] Finish\n");
     onExtactionFinishedEnded = true;
  }

  virtual void onExtactionFailed()
  {}

  Mat image;
  bool onExtactionFinishedCalled;
  bool onExtactionFinishedEnded;
  DataPoint2dVector* cbResult;
  std::chrono::milliseconds dura;
};




/**************************TESTS SECTION***************************/

TEST_F(FeaturePointExtractorTest, emptyImage)
{
	image=Mat();
	EXPECT_EQ(startExtractor(image) , wrongParams);
   VVLOG("[          ] app finished without crash\n");
}

//test if callback is called
TEST_F(FeaturePointExtractorTest, onExtactionFinishedCalled)
{
   ASSERT_TRUE( image.data );
   ResultCode ret;
   EXPECT_EQ(startExtractor(image), success);

   //if callback will be called before timeout finish with success
   while( !onExtactionFinishedCalled )
   {
     std::this_thread::sleep_for( dura );
   }

   SUCCEED();
}

//check if result is not NULL
TEST_F(FeaturePointExtractorTest, resultNotNull)
{
    EXPECT_EQ(startExtractor(image),success);
    VVLOG("[          ] Extractor launched\n");

    while( !onExtactionFinishedEnded )
    {
       std::this_thread::sleep_for( dura );
    }
    ASSERT_TRUE(cbResult);
}

//test if blank image gives no features
TEST_F(FeaturePointExtractorTest, blankImageCase)
{
    EXPECT_EQ(startExtractor(image), success);
    VVLOG("[          ] Extractor launched\n");
    while( !onExtactionFinishedEnded )
    {
        std::this_thread::sleep_for( dura );
    }
    //check if result is not NULL
    ASSERT_TRUE(cbResult);
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
    image = imread( "lena-gray.png", 1 );
    ResultCode ret;
    EXPECT_EQ(startExtractor(image), success);
    VVLOG("[          ] Extractor launched\n");
    while( !onExtactionFinishedEnded )
    {
      std::this_thread::sleep_for( dura );
    }
    //check if result is not NULL
    ASSERT_TRUE(cbResult);

    unsigned keypointCount;
    unsigned descriptorCount;
    keypointCount =   cbResult->getPoints().size();
    descriptorCount = cbResult->getDescriptors().size().height;
    VVLOG("[          ] Check if lena image gives some features\n");
    EXPECT_GT(keypointCount,10);
    EXPECT_GT(descriptorCount,10);
    EXPECT_EQ(descriptorCount,keypointCount);
}

