#ifndef VV_COMMON_H
#define VV_COMMON_H

//thread-safe logging macro
#include <mutex>
extern std::mutex VVLogMtx;
#define VVLOG(format, ...)\
 VVLogMtx.lock(); fprintf (stderr, format, ## __VA_ARGS__); VVLogMtx.unlock()

/******************Types declarations****************/

typedef void (*finishCallback)(void* inputData,void* outputData);

typedef enum
{
   vVSuccess=0,
   vVWrongParams,
   vVExecutionFailure,
   vVFailure
}
VVResultCode;

#include <opencv2/opencv.hpp>

typedef struct
{
  cv::Mat distCoeffs;
  cv::Mat cameraMatrix;
} CalibData;


#endif /*VV_COMMON_H*/
