#include <thread>
#include "FeaturePointMatcher.hpp"

using namespace cv;

void FeaturePointMatcher::matcherThreadRoutine(void* data)
{
  struct threadRoutineData* routineData;
  routineData = static_cast<struct threadRoutineData*>(data);

  FlannBasedMatcher matcher;

  std::vector<DMatch>* matches;
  matches= new std::vector<DMatch>;

  matcher.match(*(routineData->descriptors1),
                *(routineData->descriptors2),
                *matches);

  removePoorMatches(matches);

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

//method from opencv example
VVResultCode FeaturePointMatcher::removePoorMatches(vector<DMatch>* matches)
{
  if( NULL == matches )
  {
    return vVWrongParams;
  }

  int    n       = matches->size();
  double minDist = 100;

  for( int i = 0; i < n; i++ )
  {
    double dist = (*matches)[i].distance;
    if( dist < minDist ) minDist = dist;
  }

  /* Leave only "good" matches (i.e. whose distance is less than 2*min_dist,
  or a small arbitary value ( 0.02 ) in the event that min_dist is very small)
  PS. radiusMatch can also be used here.*/
  std::vector< DMatch > goodMatches;

  for( int i = 0; i < n; i++ )
  { if( (*matches)[i].distance <= max(2*minDist, 0.02) )
    {
      goodMatches.push_back( (*matches)[i] );
    }
  }
  *matches=goodMatches;

  return vVSuccess;
}

