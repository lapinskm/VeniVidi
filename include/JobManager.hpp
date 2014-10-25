#ifndef JOB_MANAGER_HPP
#define JOB_MANAGER_HPP

#include "common.hpp"
#include "DataManager.hpp"
#include "FeaturePointMatcher.hpp"
#include "FeaturePointExtractor.hpp"

#include <opencv2/opencv.hpp>
#include <queue>
#include <string>

namespace VV
{

class JobManager: public FeaturePointExtractorUser
                , public FeaturePointMatcherUser
{
  public:
    JobManager(DataManager* data_manager);
    ResultCode processImage(const std::string& path);

  private:
    ResultCode startExtractor(const std::string path);
    //
    void onExtractionFinished(DataPoint2dVector* result);
    void onExtractionFailed();

    ResultCode startMatcher(DataPoint2dVector* newKeypoints);

    void onMatchingFinished(std::vector<cv::DMatch>* result){};
    void onMatchingFailed(){};

    std::queue<std::string> extractorDataQueue;
    void* matcherDataQueue;

    //Maximum and current extractor jobs
    unsigned m_extrJobCountMax;
    unsigned m_extrJobCount;

    //Amount of threads used to matching keypoints
    unsigned m_matcherThreadCount;

    //pointer to Data  manager. Object is owned by SceneGenerator
    DataManager* m_DatMgr;
};

}/*namespace VV*/
#endif /*JOB_MANAGER_HPP*/
