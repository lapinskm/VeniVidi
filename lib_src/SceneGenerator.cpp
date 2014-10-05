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
  jobMgr.processImage(path);
}


