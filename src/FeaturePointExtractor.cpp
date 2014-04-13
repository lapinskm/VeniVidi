#include "FeaturePointExtractor.hpp"
#include "FeaturePoint.hpp"

//constuctor
FeaturePointExtractor::FeaturePointExtractor()
{
   detector=FeatureDetector::create("SURF");
}

//this function extracts points and call the callback
void FeaturePointExtractor::extractorThreadRoutine(void*  data)
{
   struct threadRoutineData* routineData;
   routineData = static_cast<struct threadRoutineData*>(data);
   list<FeaturePoint> featurePoints;
   //TODO: use opencv extractor

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

