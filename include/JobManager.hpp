#ifndef JOB_MANAGER_HPP
#define JOB_MANAGER_HPP

#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "DataManager.hpp"
#include "FeaturePointMatcher.hpp"
#include "FeaturePointExtractor.hpp"

namespace VV
{

class JobManager
{
  public:
    JobManager(DataManager* data_manager);
    ResultCode processImage(cv::Mat image);

  private:
    void extractorFinished();
    void matcherFinished();

    void* extractorDataQueue;
    void* matcherDataQueue;

    //submodules used to process image data
    FeaturePointExtractor m_fpext;
    FeaturePointMatcher   m_fpmtch;

    //pointer to Data  manager. Object is owned by SceneGenerator
    DataManager* m_DatMgr;
};

}/*namespace VV*/
#endif /*JOB_MANAGER_HPP*/
