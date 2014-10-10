#ifndef VV_FEATURE_POINTi_EXTRACTOR_HPP
#define VV_FEATURE_POINT_EXTRACTOR_HPP

#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "DataTypes.hpp"

namespace VV
{
//Forward declaration. Needed by FeaturePointExtractorUser
//It stores User's pointer
class FeaturePointExtractorUser;

class FeaturePointExtractor
{
  public:
    FeaturePointExtractor(FeaturePointExtractorUser* user)
    :m_user(user)
{};
    ~FeaturePointExtractor(){};

  // starts asynchonous feature point extracion
  // Afer finish m_user->onExtactionFinished is called.
  // Passes ownership of result to m_user.
    ResultCode startExtraction(const cv::Mat&  image);

  private:
   //Function which actually does feature point extaction.
    static void extractorRoutine(const cv::Mat& image,
                                 FeaturePointExtractorUser*);

  //strings used to setting up extractors openvc implementation
    static const char* detectStr;
    static const char* extractStr;

  //The object which is informed about Extaction statuExtaction status.
    FeaturePointExtractorUser* m_user;
};

class FeaturePointExtractorUser
{
  protected:
    //It is purely abstract class so constructor is protected
    FeaturePointExtractorUser();
    ~FeaturePointExtractorUser(){};

    //Interface functions called by extractor should be overriden

    //is called when feature extractor is finish successfully
    //It return extraction result, which should be deleted after use with delete
    virtual void onExtactionFinished(DataPoint2dVector* result);

    //Informs about extraction process failure
    virtual void onExtactionFailed();

    //function which can be called by interface implementing class
    ResultCode startExtractor(const cv::Mat& image);

  private:
    FeaturePointExtractor m_extractor;

  //needed to extractor having access to onFinish/onFail functions
  friend class FeaturePointExtractor;
};

}/*namespace VV*/
#endif /*VV_FEATUREPOINTEXTRACTOR_HPP*/

