#ifndef VV_DATA_MANAGER_HPP
#define VV_DATA_MANAGER_HPP

#include <opencv2/opencv.hpp>

#include "DataTypes.hpp"
#include "common.hpp"

namespace VV
{

/*Class storing data used to generate and update 3D scene*/
class DataManager
{
  public:
    void addImageData(ImageData);
    ResultCode setCameraDataForImg(int ID, cv::Mat data);

    std::vector<ImageData> getImgData(std::vector<int> IDs);
    DataPoint3dVector* getSceneData() {return &sceneDataPoints3D;}

  private:
    DataPoint3dVector sceneDataPoints3D;
    std::vector<ImageData> ImagesData;
};

}/*namespace VV*/

#endif /*VV_DATA_MANAGER_HPP*/
