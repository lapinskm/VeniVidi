#include <thread>
#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "DataTypes.hpp"
#include "FeaturePointExtractor.hpp"

using std::shared_ptr;

using namespace cv;
using namespace VV;

//static FeaturePointExtractor fields
const char* FeaturePointExtractor::detectStr="FAST";
const char* FeaturePointExtractor::extractStr="FREAK";

//this function extracts points and call the callback with result
void FeaturePointExtractor::extractorThreadRoutine(Mat* image,
                                                  FeaturePointExtractorCb cb,
                                                  void* userData)
{
   //extractorThreadRoutine begins;
   Ptr<FeatureDetector>     detector=FeatureDetector::create(detectStr);
   Ptr<DescriptorExtractor> extractor=DescriptorExtractor::create(extractStr);

   //launching OpenCV detectors;
   if (detector && extractor)
   {
      std::vector<KeyPoint> keypoints;

      std::vector<cv::Point2d> points;
      Mat descriptors;

      detector  -> detect  (*image, keypoints);
      extractor -> compute (*image, keypoints, descriptors);

      //fill coordinates of dataPoints from the reasult of detector
      for(int i = 0; i < keypoints.size(); i++)
      {
        points.push_back(keypoints[i].pt);
      }

      DataPoint2dVector* result = new DataPoint2dVector(points, descriptors);

      //launch callback when finish
      cb(result, userData);
   }
   else
   {
      VVLOG("extractors init failed\n");
   }
}

//this function launches feature point extraction in new thread
ResultCode FeaturePointExtractor::startExtraction(Mat& image,
                                                  FeaturePointExtractorCb cb,
                                                  void* userData)
{
   //check if parameters are not NULL (user data could be)
   if( NULL == cb )
   {
      return wrongParams;
   }
   //launch extractor thread
   std::thread t(&extractorThreadRoutine, &image, cb, userData);
   t.detach();
   return success;
}


