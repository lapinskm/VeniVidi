#include <thread>
#include <opencv2/opencv.hpp>

#include "FeaturePointMatcher.hpp"
#include "common.hpp"

using namespace cv;

void FeaturePointMatcher::matcherThreadRoutine(void* data)
{
   struct threadRoutineData* routineData;
   routineData = static_cast<struct threadRoutineData*>(data);

   FlannBasedMatcher matcher;

   std::vector< DMatch > *  matches;
   matches= new std::vector< DMatch >;

   matcher.match(*(routineData->descriptors1), 
                 *(routineData->descriptors2),
                 *matches);

   routineData->cb(matches, routineData->userData);
}


//this function launches feature point matching in new thread
VVResultCode FeaturePointMatcher::startMatching(Mat* descriptors1,
                                                Mat* descriptors2,
                                                finishCallback cb,
                                                void* userData)
{
   //check if parameters are not NULL (user data could be)
   if( NULL == descriptors1 || NULL == descriptors2 || NULL == cb)
   {
      return vVWrongParams;
   }
   //set up routine data structure
   struct threadRoutineData* data = new struct threadRoutineData ;
   data->descriptors1=descriptors1;
   data->descriptors2=descriptors2;
   data->cb=cb;
   data->userData=userData;
   //launch extractor thread
   thread t(&matcherThreadRoutine,data);
   t.detach();
   return vVSuccess;
}

