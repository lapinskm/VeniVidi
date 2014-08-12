#ifndef MATH_MODULE_H
#define MATH_MODULE_H

#include "common.hpp"

class MathModule
{
  public:
    /*Elementary operations*/
    static void orthogonalize(cv::Mat& M, unsigned startIndex=1);
    static cv::Mat vectorProjection(const cv::Mat& a, const cv::Mat& b);
    static void addTo(cv::Mat& M1, const cv::Mat& M2, bool isPositive = true);
    static void divByScalar(cv::Mat& M, const double k);

    /*Middle abstraction level minimization algorithms*/
    static cv::Mat yellowAlgorithm(const cv::Mat& A);


    /*An iterational SVD algorithm*/
    static bool updateSVD(cv::Mat& U,
                          cv::Mat& S,
                          cv::Mat& V,
                          const cv::Mat& C);


};

#endif /*MATH_MODULE_H*/
