#include "common.hpp"
#include "VVTestBase.hpp"
#include "FeaturePointMatcher.hpp"


using namespace cv;

/*************************FIXTURE SECTION**************************/
class FeaturePointMatcherTest : public VVTestBase
{
 protected:

  virtual void postSetUp()
  {
     initTestData();
     fpm=new FeaturePointMatcher();
     callbackCalled=false;
     callbackFinished=false;
  }

  virtual void preTearDown()
  {
     delete fpm;
     callbackCalled=false;
     callbackFinished=false;
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


  Mat empty_descr;
  Mat lena_descr;
  Mat lena_full_descr;
  Mat lena_120deg_descr;
  Mat lena_scalled_descr;

  FeaturePointMatcher* fpm;

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

