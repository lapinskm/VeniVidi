#include "utils.hpp"
#include "common.hpp"
#include <opencv2/opencv.hpp>

using namespace VV;
using namespace cv;

bool VV::imagesAlmostSame(Mat& image1, Mat& image2)
{
  //there is no need to check anything when images have different size or
  //any of images have no data
  if ( image1.rows != image2.rows ||
       image1.cols != image2.cols ||
      !image1.data || !image2.data )
  {
    return false;
  }

  //Maximum mean of absolute difference between images, Magic number.
  const double deltaMax = 0.01;

  Mat diff;
  absdiff(image1, image2, diff);
  //Checked only one channel, works for grayscale only
  if (mean(diff).val[0] > deltaMax)
  {
    return false;
  }
  else
  {
    return true;
  }
}

