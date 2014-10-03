#ifndef VV_SCENE_GENERATOR_HPP
#define VV_SCENE_GENERATOR_HPP

#include <opencv2/opencv.hpp>

#include "ImagePreprocessor.hpp"
#include "DataManager.hpp"
#include "JobManager.hpp"
#include "common.hpp"

namespace VV{

class SceneGenerator
{
  public:
    SceneGenerator();

    ~SceneGenerator(){};

    ResultCode processImage(const char* path);

  private:
    ResultCode loadImage(const char* path, Mat& image);
    bool imageIsProcessworthy(Mat& Image);

  private:
    //image kept to check if is the same as currently received
    Mat PreviousImage;

    /*Not implemeted features*/
    //module increasing feature point amount by search in existing
    // point neighbourhood
    //FeaturePointDensifier fpdnsf;

    /*Job sheduler*/
    JobManager  jobMgr;
    /*Data storing module*/
    DataManager DatMgr;
};

}/*namespace VV*/

#endif /*VV_SCENE_GENERATOR_HPP*/
