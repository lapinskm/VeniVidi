#include <thread>
#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "DataTypes.hpp"
#include "FeaturePointExtractor.hpp"

using std::shared_ptr;

using namespace cv;
using namespace VV;

/*########################## FeaturePointExtractor ##########################*/
//static FeaturePointExtractor fields
const char* FeaturePointExtractor::detectStr="FAST";
const char* FeaturePointExtractor::extractStr="FREAK";

//this function extracts points and call the callback with result
void FeaturePointExtractor::extractorRoutine(const Mat& image,
                                             FeaturePointExtractorUser* user)
{
   //extractorThreadRoutine begins;
   Ptr<FeatureDetector> detector = FeatureDetector::create(detectStr);
   Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create(extractStr);

   //launching OpenCV detectors;
   if (detector && extractor)
   {
      std::vector<KeyPoint> keypoints;

      std::vector<cv::Point2d> points;
      Mat descriptors;

      detector  -> detect  (image, keypoints);
      extractor -> compute (image, keypoints, descriptors);

      //fill coordinates of dataPoints from the reasult of detector
      for(int i = 0; i < keypoints.size(); i++)
      {
        points.push_back(keypoints[i].pt);
      }

      DataPoint2dVector* result = new DataPoint2dVector(points, descriptors);

      //launch callback when finish
      user->onExtactionFinished(result);
   }
   else
   {
      VVLOG("Extractors init failed.\n");
      user->onExtactionFailed();
   }
}

//this function launches feature point extraction in new thread
ResultCode FeaturePointExtractor::startExtraction(const Mat& image)
{
   //check if m_user is not NULL,
   if( NULL == m_user )
   {
     return failure;
   }
   if(! image.data)
   {
     return wrongParams;
   }
   //launch extractor thread
   std::thread t(&extractorRoutine, image, m_user);
   t.detach();
   return success;
}

/*######################## FeaturePointExtractorUser ########################*/
FeaturePointExtractorUser::FeaturePointExtractorUser()
:m_extractor(FeaturePointExtractor(this))
{
}

ResultCode FeaturePointExtractorUser::startExtractor(const cv::Mat&  image)
{
  return m_extractor.startExtraction(image);
}

void FeaturePointExtractorUser::onExtactionFinished(DataPoint2dVector* result)
{
  //nobody overriden this function. Delete result to prevent leak.
  delete result;
}

void FeaturePointExtractorUser::onExtactionFailed()
{
}
