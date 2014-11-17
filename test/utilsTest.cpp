#include "utils.hpp"
#include "common.hpp"
#include "VVTestBase.hpp"

using namespace cv;
using namespace VV;

/*************************FIXTURE SECTION**************************/
class utilsTest : public VVTestBase
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


