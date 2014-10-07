#include "JobManager.hpp"

#include "common.hpp"
#include "utils.hpp"

#include <opencv2/opencv.hpp>
#include <queue>
#include <string>

using namespace VV;
using cv::Mat;
using std::string;
using std::queue;

JobManager::JobManager(DataManager* data_manager)
  :m_DatMgr(data_manager)
{
}

ResultCode JobManager::processImage(const string& path)
{
  ResultCode returnVal = startExtractor(path);
  if (postponed == returnVal)
  {
     //add data to job queue
     extractorDataQueue.push(path);
  }
  else if ( success == returnVal )
  {
    m_extrJobCount++;
  }
  else
  {
    VV_LOG_RESULT_CODE(returnVal);
  }
  return returnVal;
}

ResultCode JobManager::startExtractor(const string path)
{
  //TODO: Add mutex or other mechanism to prevent exceed maximum job count
  //If maximum job count is reached, do nothing.
  if (m_extrJobCount >= m_extrJobCountMax )
  {
    return postponed;
  }

  Mat image;
  ResultCode returnVal = loadImage(path, image);
  if ( returnVal != success )
  {
    return returnVal;
  }
  return m_fpext.startExtraction(image, onExtractorFinished, this);
}

void JobManager::onExtractorFinished(DataPoint2dVector* keypoints, void* data)
{
  JobManager* owner = static_cast<JobManager*>(data);

  //job just finished, decrement counter
  owner->m_extrJobCount--;
  //if there someting left in queue, start extractor again
  if(! owner->extractorDataQueue.empty())
  {
    ResultCode returnVal;
    returnVal = owner->startExtractor(owner->extractorDataQueue.front());
    switch (returnVal)
    {
      case success:
        //job started succesfully. Remove data from queue,
        owner->extractorDataQueue.pop();
        //and increment Job counter
        //TODO: Add mutex or other mechanism to prevent exceed maximum job count
        owner->m_extrJobCount++;
        owner->startMatcher(keypoints);
        break;

      case postponed:
        //max job count reached, do nothing
        break;

      default:
        //Something gone horribly wrong. Inform about it.
        VV_LOG_RESULT_CODE(returnVal);
        break;
    }
  }
}

ResultCode JobManager::startMatcher(DataPoint2dVector* newKeypoints)
{
  //TODO: Implement this
  return success;
}
