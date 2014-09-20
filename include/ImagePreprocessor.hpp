 #ifndef VV_IMAGEPREPRCESSOR_H
 #define VV_IMAGEPREPRCESSOR_H

#include "common.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
namespace VV
{

class ImagePreprocessor
{
  public:
    ImagePreprocessor(){};
    ~ImagePreprocessor(){};

    ResultCode convertToGrayscale(Mat* colorMat, Mat* greyMat);

    ResultCode removeDistortion(Mat* InputMat, Mat* OutputMat,
                                CalibData calData );

  private:
};

}/*namespace VV*/
#endif /*VV_IMAGEPREPRCESSOR_H*/
