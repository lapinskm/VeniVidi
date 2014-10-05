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

JobManager::JobManager(DataManager* data_manager) :m_DatMgr(data_manager)
{
}

ResultCode JobManager::processImage(const string& path)
{
  //if maximum job count not reached
  if (m_extrJobCount < m_extrJobCountMax )
  {
    //TODO: Add mutex or other mechanism to prevent exceed maximum job count
    m_extrJobCount++;
    //start extractor
    return startExtractor(path);
  }
  else
  {
    //add to job queue
    extractorDataQueue.push(path);
    return postponed;
  }
}

ResultCode JobManager::startExtractor(const string& path)
{
  ResultCode returnVal;
  Mat image;
  returnVal = loadImage(path, image);
  if ( returnVal != success )
  {
    return returnVal;
  }
  return m_fpext.startExtraction(image, onExtractorFinished, this);
}

void JobManager::onExtractorFinished(DataPoint2dVector* result, void* userData)
{
  //TODO: Implement this
}

