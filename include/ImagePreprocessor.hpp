 #ifndef VV_IMAGEPREPRCESSOR_H
 #define VV_IMAGEPREPRCESSOR_H

#include "common.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;

class ImagePreprocessor
{
  public:
    ImagePreprocessor(){};
    ~ImagePreprocessor(){};

    VVResultCode convertToGrayscale(Mat* colorMat, Mat* greyMat);

    VVResultCode removeDistortion(Mat* InputMat, Mat* OutputMat,
                                  CalibData calData );

  private:

};
#endif /*VV_IMAGEPREPRCESSOR_H*/
