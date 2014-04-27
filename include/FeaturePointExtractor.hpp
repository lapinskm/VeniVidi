#ifndef VV_FEATUREPOINTEXTRACTOR_H
#define VV_FEATUREPOINTEXTRACTOR_H

#include <thread>
#include <opencv2/opencv.hpp>
#include "common.hpp"

using namespace cv;
using namespace std;

class FeaturePointExtractor
{
  public:
    FeaturePointExtractor(){};
    ~FeaturePointExtractor(){};
    static VVResultCode startExtraction(Mat * image,
                                       finishCallback cb,
                                       void* userData);
  private:
    struct threadRoutineData
    {
       Mat * image;
       finishCallback cb;
       void* userData;
    };

    static void extractorThreadRoutine(void* data);
    static const char* detectStr;
    static const char* extractStr;
};
#endif /*VV_FEATUREPOINTEXTRACTOR_H*/

