#ifndef VV_UTILS_HPP
#define VV_UTILS_HPP

#include "common.hpp"

#include <opencv2/opencv.hpp>
#include <string>

namespace VV{

typedef std::vector<std::pair<unsigned, unsigned>> segmentation;

//returns vector of pairs of indexes determining optimal segmentation
segmentation segmentate(unsigned threadCount, unsigned dataSize);

bool imagesAlmostSame(cv::Mat& image1, cv::Mat& image2);
bool imageIsProcessworthy(cv::Mat& image, cv::Mat& previousImage);
ResultCode loadImage(const std::string& path, cv::Mat& image);

}/*namespace VV*/

#endif /*VV_UTILS_HPP*/
