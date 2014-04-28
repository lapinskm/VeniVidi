#ifndef VV_FEATUREPOINTMATCHER_H
#define VV_FEATUREPOINTMATCHER_H

#include <opencv2/opencv.hpp>
#include "common.hpp"

using namespace cv;
using namespace std;

class FeaturePointMatcher
{
  public:
    FeaturePointMatcher(){};
    ~FeaturePointMatcher(){};
    static VVResultCode startMatching(Mat* descriptors1,
                                      Mat* descriptors2,
                                      finishCallback cb,
                                      void* userData);

 //static VVResultCode mergeToMatchTable(MatchTable& mt,
 //                                      vector<DMatch> matches);
  private:
    struct threadRoutineData
    {
      Mat* descriptors1;
      Mat* descriptors2;
      finishCallback cb;
      void* userData;
    };

    static VVResultCode removePoorMatches(vector<DMatch>* matches);
    static void matcherThreadRoutine(void* data);
};

#endif /*VV_FEATUREPOINTMATCHER_H*/

