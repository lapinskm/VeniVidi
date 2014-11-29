#include "utils.hpp"
#include "common.hpp"
#include "VVTestBase.hpp"

using namespace cv;
using namespace VV;

/*************************FIXTURE SECTION**************************/
class utilsTest :public VVTestBase
{
  protected:
    Mat image1;
    Mat image2;
};


/**************************TESTS SECTION***************************/

TEST_F(utilsTest, imagesAlmostSame_emptyImages)
{
  EXPECT_FALSE( imagesAlmostSame(image1, image2));
}

TEST_F(utilsTest, imagesAlmostSame_sizeNotMatch)
{
  image1 = Mat(200, 100, numericDataType);
  image2 = Mat(100, 100, numericDataType);
  EXPECT_FALSE( imagesAlmostSame(image1, image2));

  image1 = Mat(200, 100, numericDataType);
  image2 = Mat(200, 200, numericDataType);
  EXPECT_FALSE( imagesAlmostSame(image1, image2));
}

TEST_F(utilsTest,  imagesAlmostSame_negativeCase)
{
  image1 = imread("lena-gray.png"       , CV_LOAD_IMAGE_GRAYSCALE);
  image2 = imread("lena-gray-120deg.png", CV_LOAD_IMAGE_GRAYSCALE);
  EXPECT_FALSE( imagesAlmostSame(image1, image2));
}

TEST_F(utilsTest,  imagesAlmostSame_PositiveCase_blank)
{
  image1 = Mat::zeros(200, 100, numericDataType);
  image2 = Mat::zeros(200, 100, numericDataType);
  EXPECT_TRUE( imagesAlmostSame(image1, image2));
}

TEST_F(utilsTest,  imagesAlmostSame_PositiveCase_sameImg)
{
  image1 = imread("lena-gray.png"          , CV_LOAD_IMAGE_GRAYSCALE);
  image2 = imread("lena-gray.png"          , CV_LOAD_IMAGE_GRAYSCALE);

  ASSERT_TRUE(image1.data);
  ASSERT_TRUE(image2.data);

  EXPECT_TRUE( imagesAlmostSame(image1, image2));
}


TEST_F(utilsTest,  imagesAlmostSame_PositiveCase_lena)
{
  image1 = imread("lena-gray.png"          , CV_LOAD_IMAGE_GRAYSCALE);
  image2 = imread("lena-gray-0_09noise.png", CV_LOAD_IMAGE_GRAYSCALE);

  ASSERT_TRUE(image1.data);
  ASSERT_TRUE(image2.data);

  EXPECT_TRUE( imagesAlmostSame(image1, image2));
}

TEST_F(utilsTest, segmentate_dataSmallerThanJobPool)
{
  segmentation segm = segmentate(5, 3);
  ASSERT_EQ(3, segm.size());

  EXPECT_EQ(0, segm[0].first );
  EXPECT_EQ(0, segm[0].second);

  EXPECT_EQ(1, segm[1].first );
  EXPECT_EQ(1, segm[1].second);

  EXPECT_EQ(2, segm[2].first );
  EXPECT_EQ(2, segm[2].second);
}


TEST_F(utilsTest, segmentate_dataSizeEqualJobPool)
{
  segmentation segm = segmentate(3, 3);
  ASSERT_EQ(3, segm.size());

  EXPECT_EQ(0, segm[0].first );
  EXPECT_EQ(0, segm[0].second);

  EXPECT_EQ(1, segm[1].first );
  EXPECT_EQ(1, segm[1].second);

  EXPECT_EQ(2, segm[2].first );
  EXPECT_EQ(2, segm[2].second);
}


TEST_F(utilsTest, segmentate_dataDividesEqually)
{
  segmentation segm = segmentate(3, 12);
  ASSERT_EQ(3, segm.size());

  EXPECT_EQ(0, segm[0].first );
  EXPECT_EQ(3, segm[0].second);

  EXPECT_EQ(4, segm[1].first );
  EXPECT_EQ(7, segm[1].second);

  EXPECT_EQ(8,  segm[2].first );
  EXPECT_EQ(11, segm[2].second);

}

TEST_F(utilsTest, segmentate_dataDividesNotEqually)
{
  segmentation segm = segmentate(3, 17);
  ASSERT_EQ(3, segm.size());

  EXPECT_EQ(0, segm[0].first );
  EXPECT_EQ(5, segm[0].second);

  EXPECT_EQ(6,  segm[1].first );
  EXPECT_EQ(11, segm[1].second);

  EXPECT_EQ(12, segm[2].first );
  EXPECT_EQ(16, segm[2].second);
}

TEST_F(utilsTest, segmentate_noThreads)
{
  segmentation segm = segmentate(0, 6);
  ASSERT_EQ(0, segm.size());
}

TEST_F(utilsTest, segmentate_noData)
{
  segmentation segm = segmentate(5, 0);
  ASSERT_EQ(0, segm.size());
}

