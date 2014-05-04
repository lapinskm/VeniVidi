#include "ImagePreprocessor.hpp"
#include <opencv2/imgproc/imgproc.hpp>

VVResultCode ImagePreprocessor::convertToGrayscale(Mat * colorMat,
                                                   Mat * greyMat)
{
  if ( NULL == colorMat || NULL == greyMat )
  {
    return vVWrongParams;
  }

  cv::cvtColor(*colorMat, *greyMat, CV_BGR2GRAY);
  return vVSuccess;
}
VVResultCode ImagePreprocessor::removeDistortion(Mat* inputMat,
                                                 Mat* outputMat,
                                                 CalibData calData )
{
  if ( NULL == inputMat || NULL == outputMat )
  {
    return vVWrongParams;
  }
  cv::undistort(*inputMat, *outputMat, calData.cameraMatrix, calData.distCoeffs);
  return vVSuccess;
}

