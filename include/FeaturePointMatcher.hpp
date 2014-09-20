#ifndef VV_FEATUREPOINTMATCHER_HPP
#define VV_FEATUREPOINTMATCHER_HPP

#include <memory>
#include <opencv2/opencv.hpp>
#include "common.hpp"

namespace VV
{

class FeaturePointMatcher
{
  public:
    FeaturePointMatcher(){};
    ~FeaturePointMatcher(){};
    static ResultCode startMatching(std::shared_ptr<cv::Mat> descriptors1,
                                      std::shared_ptr<cv::Mat> descriptors2,
                                      finishCallback cb,
                                      void* userData);

  private:
    static ResultCode removePoorMatches(std::vector<cv::DMatch>* matches);

    static void matcherThreadRoutine(std::shared_ptr<cv::Mat> descriptors1,
                                     std::shared_ptr<cv::Mat> descriptors2,
                                     finishCallback cb,
                                     void* userData);
};

}/*namespace VV*/

#endif /*VV_FEATUREPOINTMATCHER_HPP*/

