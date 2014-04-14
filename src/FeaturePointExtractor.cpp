#include "FeaturePointExtractor.hpp"
#include "ImageFeaturePoints.hpp"

//this function extracts points and call the callback with result
void FeaturePointExtractor::extractorThreadRoutine(void*  data)
{
   struct threadRoutineData* routineData;
   routineData = static_cast<struct threadRoutineData*>(data);
   ImageFeaturePoints featurePoints;
   Ptr<FeatureDetector>     detector=FeatureDetector::create("SURF");
   Ptr<DescriptorExtractor> extractor=DescriptorExtractor::create("SURF");

   detector  -> detect  (routineData->image,featurePoints.keypoints);
   extractor -> compute (routineData->image,featurePoints.keypoints,
                         featurePoints.descriptors);

   //launch callback when finish
   routineData->cb(&featurePoints, routineData->userData);
   //free routine data
   delete routineData;
}

//this function launches feature point extraction in new thread
void FeaturePointExtractor::startExtraction(Mat image,
                                            finishCallback cb,
                                            void* userData)
{
   //set up routine data structure
   struct threadRoutineData* data = new struct threadRoutineData;
   data->image=image;
   data->cb=cb;
   data->userData=userData;   

   //launch extractor thread
   std::thread t1(&extractorThreadRoutine,data);
}

