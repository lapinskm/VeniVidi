#include <opencv2/opencv.hpp>

#include "SceneGenerator.hpp"

#include "utils.hpp"

using namespace VV;
using namespace cv;


SceneGenerator::SceneGenerator()
  :jobMgr(JobManager(&DatMgr))
{
}

ResultCode SceneGenerator::processImage(const char* path)
{
  ResultCode ret;
  Mat image;

  ret=loadImage(path, image);
  if (ret != success)
  {
    return ret;
  }

  if ( imageIsProcessworthy(image) )
  {
    jobMgr.processImage(image);
    PreviousImage = image;
    return success;
  }
  else
  {
    return nothingToDo;
  }

}

ResultCode SceneGenerator::loadImage(const char* path, Mat& image)
{
  if ( !path )
  {
    return wrongParams;
  }
  image = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
  return(image.data ? success : failure);
}

bool SceneGenerator::imageIsProcessworthy(Mat& image)
{
  if( !image.data )
  {
     return false;
  }

  if( image.rows == 0 || image.cols == 0 )
  {
    return false;
  }

  //Check if this is image is not too similar to previous one
  //(e.g. camera stayed in the same place between images)
  if( imagesAlmostSame(image, PreviousImage) )
  {
    return false;
  }

  return true;
}

