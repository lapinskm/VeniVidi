#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include "common.hpp"

class MathModule
{
  public:
    /*Elementary operations*/
    static void orthogonalize(cv::Mat& M, unsigned startIndex=1);
    static cv::Mat vectorProjection(const cv::Mat& a, const cv::Mat& b);

    /*Middle abstraction level minimization algorithms*/
    static cv::Mat yellowAlgorithm(const cv::Mat& A);
};

#endif /*MATH_MODULE_H*/
