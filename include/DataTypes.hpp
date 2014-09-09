#ifndef VV_DATA_TYPES_HPP
#define VV_DATA_TYPES_HPP

#include <opencv2/opencv.hpp>

namespace VV
{

/* Class used to store data of 3D scene points.
 * It cannot be classical vector due to way which opencv uses list
 * of descriptors (as a cv::Mat object)
 */
class DataPoint3dVector
{
  public:
    std::shared_ptr<cv::Mat> getDescriptors() {return descriptors;}
    std::vector<cv::Point3d> getPoints() {return points;}
    std::vector<int> getImageIds(int i) {return imageIds[i];}

  private:
    //descriptor for each data point
    std::shared_ptr<cv::Mat> descriptors;

    /* Coordinates for each data point.
     * Values makes only sense if corresponding imageIds vector
     * is longer than 2, so triangulation was possible.*/
    std::vector<cv::Point3d> points;

    //vector of ImageIds for each data point
    std::vector<std::vector<int>> imageIds;

};

class DataPoint2dVector
{
  public:
    //probably we won't need store descriptors here
    //std::shared_ptr<cv:Mat> getDescriptors(){return descriptors;}

    std::vector<cv::Point2d> getPoints() {return points;}

  private:
    //std::shared_ptr<cv:Mat>  descriptors;
    std::vector<cv::Point2d> points;
};

/* Class containing data needed to be stored for each image
 */
class ImageData
{
  public:
    DataPoint2dVector getDataPoints(){return dataPoints;}
    cv::Mat           getCameraData(){return cameraData;}

  private:
    DataPoint2dVector dataPoints;
    cv::Mat cameraData;
};


}//namespace VV

#endif /*VV_DATA_TYPES_HPP*/
