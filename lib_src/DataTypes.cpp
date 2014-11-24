#include <assert.h>
#include "DataTypes.hpp"
#include "common.hpp"

using namespace std;
using namespace cv;
using namespace VV;

bool  DataPoint2dVector::sanityCheck()
{
  //Check if m_isProper flag is set.
  if(!m_isProper)
  {
     VVLOG_SOURCE_INFO();
     VVLOG("WARNING! m_isProper flag is not set!\n");
     return false;
  }

  //Check if point and descritor lists contain any data.
  if(!m_descriptors.data || m_points.size() == 0 )
  {
    VVLOG_SOURCE_INFO();
    VVLOG("WARNING! Point and descritor lists contain no data!\n");
    return m_isProper = false;
  }

  //Check if point and descritor lists have the same size.
  if(m_descriptors.rows != m_points.size())
  {
     VVLOG_SOURCE_INFO();
     VVLOG("WARNING! Point and descritor lists have different size!\n"
           "         descriptor count == %d points count == %lu \n"
           "         Sanity check failed!\n",
           m_descriptors.rows, m_points.size());
     return m_isProper = false;
  }

  return true;
}

DataPoint2dVector DataPoint2dVector::getDataSubset(unsigned firstIndex,
                                                   unsigned lastIndex)
{
  if (firstIndex > lastIndex ||  lastIndex >= m_descriptors.rows )
  {
    //This constructor creates empty vector with m_isProper flag set to false.
    return DataPoint2dVector();
  }

  DataPoint2dVector subset;
  subset.setDescriptors(m_descriptors.rowRange(firstIndex, lastIndex));

  vector<Point2d> pointsSubset(m_points.begin()+firstIndex,
                               m_points.begin()+lastIndex);

  subset.setPoints(pointsSubset);
  subset.m_isProper=true;
  return subset;
}
