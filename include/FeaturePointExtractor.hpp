#ifndef VV_FEATURE_POINTi_EXTRACTOR_HPP
#define VV_FEATURE_POINT_EXTRACTOR_HPP

#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "DataTypes.hpp"

namespace VV
{
//callback type declaration
typedef void (*FeaturePointExtractorCb)
             (DataPoint2dVector* result, void* userData);

class FeaturePointExtractor
{
  public:
    FeaturePointExtractor(){};
    ~FeaturePointExtractor(){};

    // starts asynchonous feature point extracion
    // Afer finish cb is called. Passes ownership of result to callback.
    static ResultCode startExtraction(cv::Mat&  image,
                                      FeaturePointExtractorCb cb,
                                      void* userData);

  private:
   //Function which actually does feature point extaction.
    static void extractorThreadRoutine(cv::Mat* image,
                                       FeaturePointExtractorCb cb,
                                       void* userData);

  //strings used to setting up extractors openvc implementation
    static const char* detectStr;
    static const char* extractStr;

};

}/*namespace VV*/
#endif /*VV_FEATUREPOINTEXTRACTOR_HPP*/

