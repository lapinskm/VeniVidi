#include <thread>
#include <opencv2/opencv.hpp>

#include "common.hpp"
#include "FeaturePointExtractor.hpp"
#include "ImageFeaturePoints.hpp"

using std::shared_ptr;

using namespace cv;
using namespace VV;

const char* FeaturePointExtractor::detectStr="FAST";
const char* FeaturePointExtractor::extractStr="FREAK";

//this function extracts points and call the callback with result
void FeaturePointExtractor::extractorThreadRoutine(shared_ptr<Mat> image,
                                                  finishCallback cb,
                                                  void* userData)
{
   //cout<<"extractorThreadRoutine begins\n";
   ImageFeaturePoints* featurePoints = new ImageFeaturePoints();
   Ptr<FeatureDetector>     detector=FeatureDetector::create(detectStr);
   Ptr<DescriptorExtractor> extractor=DescriptorExtractor::create(extractStr);

   //cout<<"launching OpenCV detectors\n";
   if (detector && extractor)
   {
      detector  -> detect  (*image.get(), featurePoints->keypoints);
      extractor -> compute (*image.get(), featurePoints->keypoints,
                            featurePoints->descriptors);
       //cout<<"done\n";
       //launch callback when finish
       cb(featurePoints, userData);
   }
   else VVLOG("extractors init failed\n");
}

//this function launches feature point extraction in new thread
VVResultCode FeaturePointExtractor::startExtraction(shared_ptr<Mat> image,
                                                    finishCallback cb,
                                                    void* userData)
{
   //check if parameters are not NULL (user data could be)
   if( NULL == image || NULL == cb )
   {
      return vVWrongParams;
   }
   //launch extractor thread
   std::thread t(&extractorThreadRoutine, image, cb, userData);
   t.detach();
   return vVSuccess;
}

