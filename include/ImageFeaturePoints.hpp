#ifndef VV_IMAGE_FEATURE_POINTS_HPP
#define VV_IMAGE_FEATURE_POINTS_HPP

#include <opencv2/opencv.hpp>
class ImageFeaturePoints
{
   public:
     int imageID;
     std::vector<cv::KeyPoint> keypoints;
     cv::Mat descriptors;
};
#endif /*VV_IMAGE_FEATURE_POINTS_HPP*/
