#ifndef VV_FEATURE_POINT_EXTRACTOR_HPP
#define VV_FEATURE_POINT_EXTRACTOR_HPP

#include <memory>
#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "ImageFeaturePoints.hpp"

namespace VV
{
class FeaturePointExtractor
{
  public:
    FeaturePointExtractor(){};
    ~FeaturePointExtractor(){};
    static VVResultCode startExtraction(std::shared_ptr<cv::Mat>  image,
                                        finishCallback cb,
                                        void* userData);
  private:

    static void extractorThreadRoutine(std::shared_ptr<cv::Mat> image,
                                       finishCallback cb,
                                       void* userData);
    static const char* detectStr;
    static const char* extractStr;

};

}/*namespace VV*/
#endif /*VV_FEATUREPOINTEXTRACTOR_HPP*/

