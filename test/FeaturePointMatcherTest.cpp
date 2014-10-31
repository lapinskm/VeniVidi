#include "common.hpp"
#include "VVTestBase.hpp"
#include "FeaturePointMatcher.hpp"


using namespace cv;
using namespace VV;


/*************************FIXTURE SECTION**************************/
class FeaturePointMatcherTest :public FeaturePointMatcherUser
                              ,public VVTestBase
{
 protected:
  virtual void postSetUp()
  {
     initTestData();
     lastId = -1;
     cbResult = NULL;
     onMatchingFinishedCalled = false;
     onMatchingFinishedEnded = false;
  }

  virtual void preTearDown()
  {
    if(cbResult)
    {
      delete cbResult;
    }
  }
 public:
  virtual void onMatchingFinished(std::vector<DMatch>* result, int id) override
  {
     VVLOG("[ onFinshed] Start\n");
     onMatchingFinishedCalled=true;
     cbResult = result;
     lastId = id;
     VVLOG("[ onFinshed] End\n");
     onMatchingFinishedEnded=true;
  }

  virtual void onMatchingFailed(int id) override
  {
    VVLOG("[ onFailed]\n");
    FAIL();
  }

  void findDescriptors(const char* filename, Mat& descriptors)
  {
    Ptr<FeatureDetector>     detector=FeatureDetector::create("ORB");
    Ptr<DescriptorExtractor> extractor=DescriptorExtractor::create("ORB");
    ASSERT_TRUE(detector);
    ASSERT_TRUE(extractor);
    vector<KeyPoint> keypoints;

    Mat image = imread(filename,
                       CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

    detector  -> detect  (image, keypoints);
    extractor -> compute (image, keypoints, descriptors);
  }

  void initTestData()
  {
    zero_descr = Mat::zeros(1, 1, CV_32F);
    findDescriptors("lena-gray.png", lena_descr);
    findDescriptors("lena-gray-120deg.png", lena_120deg_descr);
  }

  Mat dataless_descr;
  Mat zero_descr;
  Mat lena_descr;
  Mat lena_120deg_descr;

  int lastId;
  bool onMatchingFinishedCalled;
  bool onMatchingFinishedEnded;
  std::vector< DMatch >* cbResult;
};

/**************************TESTS SECTION***************************/
//using decriptors without Mat::data should result with wrongParams
TEST_F(FeaturePointMatcherTest, wrongParameters)
{
  ResultCode ret;
  int id = 0;

  EXPECT_EQ(wrongParams, startMatcher(dataless_descr, zero_descr, id));
  EXPECT_EQ(wrongParams, startMatcher(zero_descr,dataless_descr , id++));
  EXPECT_EQ(wrongParams, startMatcher(zero_descr,dataless_descr , id++));
}

//test if callback is called
TEST_F(FeaturePointMatcherTest, callbackIsCalled)
{

  ResultCode ret;
  ret=startMatcher(zero_descr, zero_descr, 0);
  EXPECT_EQ(ret, success);
  //if callback will be called before timeout finish with success
  waitUntil(onMatchingFinishedEnded);
  SUCCEED();
}

//check if result is not NULL
TEST_F(FeaturePointMatcherTest, resultNotNull)
{
  ResultCode ret;
  ret=startMatcher(zero_descr, zero_descr, 0);
  EXPECT_EQ(ret, success);
  waitUntil(onMatchingFinishedEnded);
  EXPECT_TRUE(cbResult);
}

TEST_F(FeaturePointMatcherTest, sameDescriptors)
{
  ResultCode ret;
  ret=startMatcher(lena_descr, lena_descr, 0);
  EXPECT_EQ(ret,success);
  waitUntil(onMatchingFinishedEnded);
  //faill when null
  ASSERT_TRUE(cbResult);
  VVLOG("[ result   ] result size is %ld\n",cbResult->size());
  //amount of matches should be the same as descriptors
  EXPECT_EQ(lena_descr.size().height, cbResult->size());
}

TEST_F(FeaturePointMatcherTest, 120Rot)
{
  ResultCode ret;
  ret=startMatcher(lena_descr, lena_120deg_descr, 0);
  EXPECT_EQ(ret, success);
  waitUntil(onMatchingFinishedEnded);
  //faill when null
  ASSERT_TRUE(cbResult);
  VVLOG("[ result   ] result size is %ld\n", cbResult->size());
  EXPECT_GT( cbResult->size(), 10);
}

//check if onMatchingFinished recieves the same Id as specified.
TEST_F(FeaturePointMatcherTest, checkId)
{
  ResultCode ret;
  int id = 69;// Just a number.
  ret=startMatcher(lena_descr, lena_descr, id);
  EXPECT_EQ(ret,success);
  waitUntil(onMatchingFinishedEnded);
  //faill when null
  ASSERT_TRUE(cbResult);
  EXPECT_EQ(id, lastId);
}

