#include <thread>
#include "FeaturePointMatcher.hpp"

using namespace cv;
using namespace VV;
using std::shared_ptr;

void FeaturePointMatcher::matcherThreadRoutine(shared_ptr<Mat> descriptors1,
                                               shared_ptr<Mat> descriptors2,
                                               finishCallback cb,
                                               void* userData)
{
  FlannBasedMatcher matcher;

  std::vector<DMatch>* matches;
  matches= new std::vector<DMatch>;

  matcher.match(*descriptors1.get(),
                *descriptors2.get(),
                *matches);

  removePoorMatches(matches);

  //Call the callback with reasult
  cb(matches, userData);
}


//this function launches feature point matching in new thread
VVResultCode FeaturePointMatcher::startMatching(shared_ptr<Mat> descriptors1,
                                                shared_ptr<Mat> descriptors2,
                                                finishCallback cb,
                                                void* userData)
{
  //check if parameters are not NULL (user data could be)
  if( NULL == descriptors1.get() || NULL == descriptors2.get() || NULL == cb)
  {
    return vVWrongParams;
  }
  //check if any of desciptors is empty
  if ( descriptors1.get()->empty() || descriptors2.get()->empty() )
  {
    return vVFailure;
  }
  //check if descriptors are in right type. if not convert
  if (CV_32F != descriptors1.get()->type() )
    descriptors1.get()->convertTo(*descriptors1.get(),CV_32F);

  if(CV_32F != descriptors2.get()->type() )
     descriptors2.get()->convertTo(*descriptors2.get(),CV_32F);

  //launch extractor thread
  std::thread t(&matcherThreadRoutine, descriptors1, descriptors2, cb, userData);
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

  int n = matches->size();
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

