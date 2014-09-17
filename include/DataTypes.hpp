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

    void addPoint(cv::Point3d point, cv::Mat descriptor);
    void addImageToPoint(int imageId);

    cv::Mat getDescriptors() {return m_descriptors;}
    std::vector<cv::Point3d> getPoints() {return m_points;}
    std::vector<int> getImageIds(int i) {return m_imageIds[i];}

  private:
    //descriptor for each data point
    cv::Mat m_descriptors;

    /* Coordinates for each data point.
     * Values makes only sense if corresponding imageIds vector
     * is longer than 2, so triangulation was possible.*/
    std::vector<cv::Point3d> m_points;

    //vector of ImageIds for each data point
    std::vector<std::vector<int>> m_imageIds;

};

class DataPoint2dVector
{
  public:
    DataPoint2dVector(std::vector<cv::Point2d> points, cv::Mat descriptors)
    :m_points(points)
    ,m_descriptors(descriptors)
    {}
    cv::Mat getDescriptors(){return m_descriptors;}
    std::vector<cv::Point2d> getPoints() {return m_points;}

  private:
    DataPoint2dVector();
    cv::Mat  m_descriptors;
    std::vector<cv::Point2d> m_points;
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
