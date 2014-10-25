#include <thread>
#include "FeaturePointMatcher.hpp"

using namespace cv;
using namespace VV;
using std::vector;

void FeaturePointMatcher::matcherRoutine(const Mat& descriptors1,
                                         const Mat& descriptors2,
                                         FeaturePointMatcherUser* user)
{
  FlannBasedMatcher matcher;

  vector<DMatch>* matches = new vector<DMatch>;

  matcher.match(descriptors1,
                descriptors2,
                *matches);

  if ( success != removePoorMatches(matches) )
  {
    if(matches)
    {
       delete matches;
    }
    user->onMatchingFailed();
    return;
  }

  if (!matches)
  {
    user->onMatchingFailed();
    return;
  }

  //Call the callback with result
  user->onMatchingFinished(matches);
}


//this function launches feature point matching in new thread
ResultCode FeaturePointMatcher::startMatching(Mat& descriptors1,
                                              Mat& descriptors2)
{
  if (!descriptors1.data || !descriptors2.data )
  {
    return wrongParams;
  }

  //check if any of desciptors is empty
  if ( descriptors1.empty() || descriptors2.empty() )
  {
    return failure;
  }

  //check if descriptors are in right type. if not convert
  if (CV_32F != descriptors1.type() )
  {
    descriptors1.convertTo(descriptors1, CV_32F);
  }
  if(CV_32F != descriptors2.type() )
     descriptors2.convertTo(descriptors2, CV_32F);

  //launch extractor thread
  std::thread t(&matcherRoutine, descriptors1, descriptors2, m_user);
  t.detach();
  return success;
}

//method from opencv example
ResultCode FeaturePointMatcher::removePoorMatches(vector<DMatch>* matches)
{
  if( NULL == matches )
  {
    return wrongParams;
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
  vector< DMatch > goodMatches;

  for( int i = 0; i < n; i++ )
  { if( (*matches)[i].distance <= max(2*minDist, 0.02) )
    {
      goodMatches.push_back( (*matches)[i] );
    }
  }
  *matches=goodMatches;

  return success;
}

