#include <opencv2/opencv.hpp>

#include "SceneGenerator.hpp"

using namespace VV;
void SceneGenerator::processImage(const char* path)
{
  std::shared_ptr<cv::Mat> image;
  image = std::shared_ptr<cv::Mat>(new cv::Mat);
  *image.get() = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
}

void SceneGenerator::FeaturePointExtractorFinishCb(void* inputData,
                                                   void* outputData)
{

}

void SceneGenerator::FeaturePointMatcherFinishCb(void* inputData,
                                                 void* outputData)
{
}

