#include <opencv2/opencv.hpp>

#include "DataManager.hpp"
#include "common.hpp"

using namespace VV;
using namespace cv;

void DataManager::addImageData(ImageData data)
{
  ImagesData.push_back(data);
}

ResultCode DataManager::setCameraDataForImg(int ID, Mat data)
{
   if(ID >= ImagesData.size())
   {
     return wrongParams;
   }
   ImagesData[ID].setCameraData(data);
   return success;
}
