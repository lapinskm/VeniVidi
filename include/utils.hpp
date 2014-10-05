#ifndef VV_UTILS_HPP
#define VV_UTILS_HPP

#include "common.hpp"

#include <opencv2/opencv.hpp>
#include <string>

namespace VV{

bool imagesAlmostSame(cv::Mat& image1, cv::Mat& image2);
bool imageIsProcessworthy(cv::Mat& image, cv::Mat& previousImage);
ResultCode loadImage(const std::string& path, cv::Mat& image);

}/*namespace VV*/

#endif /*VV_UTILS_HPP*/
