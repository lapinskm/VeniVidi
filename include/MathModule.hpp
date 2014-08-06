#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include "common.hpp"

class MathModule
{
  public:
    static void orthogonalize(cv::Mat& M, unsigned startIndex=1);
    static cv::Mat vectorProjection(const cv::Mat& a, const cv::Mat& b);
};

#endif /*MATH_MODULE_H*/
