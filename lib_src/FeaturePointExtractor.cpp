#include <stdio.h>
#include <unistd.h>
#include "FeaturePointExtractor.hpp"
#include "ImageFeaturePoints.hpp"

using namespace std;

const char * FeaturePointExtractor::detectStr="FAST";
const char * FeaturePointExtractor::extractStr="FREAK";

//this function extracts points and call the callback with result
void FeaturePointExtractor::extractorThreadRoutine(void*  data)
{
   //cout<<"extractorThreadRoutine begins\n";
   struct threadRoutineData* routineData;
   routineData = static_cast<struct threadRoutineData*>(data);
   ImageFeaturePoints* featurePoints = new ImageFeaturePoints();
   Ptr<FeatureDetector>     detector=FeatureDetector::create(detectStr);
   Ptr<DescriptorExtractor> extractor=DescriptorExtractor::create(extractStr);

   //cout<<"launching OpenCV detectors\n";
   if (detector && extractor)
   {
      detector  -> detect  (*(routineData->image), featurePoints->keypoints);
      extractor -> compute (*(routineData->image), featurePoints->keypoints,
                            featurePoints->descriptors);
       //cout<<"done\n";
       //launch callback when finish
       routineData->cb(featurePoints, routineData->userData);
   }
   else cout<<"extractors init failed\n";

   //free routine data
   delete routineData;
}

//this function launches feature point extraction in new thread
VVResultCode FeaturePointExtractor::startExtraction(Mat* image,
                                                    finishCallback cb,
                                                    void* userData)
{
   //check if parameters are not NULL (user data could be)
   if( NULL == image || NULL == cb )
   {
      return vVWrongParams;
   }
   //set up routine data structure
   struct threadRoutineData* data = new struct threadRoutineData ;
   data->image=image;
   data->cb=cb;
   data->userData=userData;
   //launch extractor thread
   thread t(&extractorThreadRoutine,data);
   t.detach();
   return vVSuccess;
}

