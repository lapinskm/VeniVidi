#ifndef VV_FEATURE_POINTi_EXTRACTOR_HPP
#define VV_FEATURE_POINT_EXTRACTOR_HPP

#include <memory>
#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "DataTypes.hpp"

namespace VV
{
//callback type declaration
typedef void (*FeaturePointExtractorCb)
             (std::shared_ptr<DataPoint2dVector> result, void* userData);

class FeaturePointExtractor
{
  public:
    FeaturePointExtractor(){};
    ~FeaturePointExtractor(){};

    static VVResultCode startExtraction(std::shared_ptr<cv::Mat>  image,
                                        FeaturePointExtractorCb cb,
                                        void* userData);
  private:

    static void extractorThreadRoutine(std::shared_ptr<cv::Mat> image,
                                       FeaturePointExtractorCb cb,
                                       void* userData);
    static const char* detectStr;
    static const char* extractStr;

};

}/*namespace VV*/
#endif /*VV_FEATUREPOINTEXTRACTOR_HPP*/

