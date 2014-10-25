#ifndef VV_FEATUREPOINTMATCHER_HPP
#define VV_FEATUREPOINTMATCHER_HPP

#include <opencv2/opencv.hpp>
#include "common.hpp"

namespace VV
{
class FeaturePointMatcherUser;

/*########################### FeaturePointMatcher ###########################*/
class FeaturePointMatcher
{
  public:
    FeaturePointMatcher(FeaturePointMatcherUser* user)
      :m_user(user)
    {}
    ~FeaturePointMatcher()
    {}

    //Starts asynchronous matching process. When matching is finished,
    //one functions of interface FeaturePointMatcherUser is called.
    //Depending on if matching succeded, it would be onMatchingFinished or
    // onMatchingFailed.
    ResultCode startMatching(cv::Mat& descriptors1,
                             cv::Mat& descriptors2);

  private:
    //Removes matches which descriptors are to distant to each other.
    static ResultCode removePoorMatches(std::vector<cv::DMatch>* matches);

    //Performs actual matching
    static void matcherRoutine(const cv::Mat& descriptors1,
                               const cv::Mat& descriptors2,
                               FeaturePointMatcherUser* user);

    //Object informed about execution status.
    FeaturePointMatcherUser* m_user;
};

/*######################## FeaturePointMatcherUser ##########################*/
class FeaturePointMatcherUser
{
  protected:
    //Protected constructor makes class purely abstract
    //cannot be used as an object
    FeaturePointMatcherUser()
      :m_matcher(FeaturePointMatcher(this))
    {}
    ~FeaturePointMatcherUser()
    {}

    //This function is called if matching finished successfully.
    virtual void onMatchingFinished(std::vector<cv::DMatch>* result) = 0;

    //This function is called if matching failed.
    virtual void onMatchingFailed() = 0;

  public:
    ResultCode startMatcher(cv::Mat& descriptors1,
                            cv::Mat& descriptors2)
    {
      return m_matcher.startMatching(descriptors1, descriptors2);
    }

  private:
    FeaturePointMatcher m_matcher;
  friend class FeaturePointMatcher;
};

}/*namespace VV*/

#endif /*VV_FEATUREPOINTMATCHER_HPP*/

