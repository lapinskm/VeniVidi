#include "ImagePreprocessor.hpp"
#include <opencv2/imgproc/imgproc.hpp>

using namespace VV;

ResultCode ImagePreprocessor::convertToGrayscale(Mat * colorMat,
                                                 Mat * greyMat)
{
  if ( NULL == colorMat || NULL == greyMat )
  {
    return wrongParams;
  }

  cv::cvtColor(*colorMat, *greyMat, CV_BGR2GRAY);
  return success;
}
ResultCode ImagePreprocessor::removeDistortion(Mat* inputMat,
                                               Mat* outputMat,
                                               CalibData calData )
{
  if ( NULL == inputMat || NULL == outputMat )
  {
    return wrongParams;
  }
  cv::undistort(*inputMat, *outputMat, calData.cameraMatrix, calData.distCoeffs);
  return success;
}


