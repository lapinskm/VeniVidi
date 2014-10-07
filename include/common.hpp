#ifndef VV_COMMON_H
#define VV_COMMON_H

#include <opencv2/opencv.hpp>
#include <mutex>

/***********************Macros**********************/
//thread-safe logging macros
#define VVLOG(format, ...)\
 VVLogMtx.lock(); fprintf (stderr, format, ## __VA_ARGS__); VVLogMtx.unlock()

#define VV_PRINT_MAT(mat)\
 VVLogMtx.lock(); printf("%s = \n", #mat); displayMatrix(mat); VVLogMtx.unlock()

#define VV_LOG_RESULT_CODE(code)\
   VVLogMtx.lock(); fprintf(stderr, "%s:%d %s=%s\n", __FILE__, __LINE__, #code,\
                            resultCodeToString(code)); VVLogMtx.unlock()

//common opencv numeric calculation data type
#define numericDataType CV_64FC1
#define VVEpsilon 1.0e-8

/************Global variables declarations***********/
extern std::mutex VVLogMtx;

/******************Types declarations****************/
namespace VV
{
typedef void (*finishCallback)(void* inputData,void* outputData);

typedef enum
{
   success=0,
   wrongParams,
   executionFailure,
   nothingToDo,
   postponed,
   failure
}
ResultCode;

typedef struct
{
  cv::Mat distCoeffs;
  cv::Mat cameraMatrix;
} CalibData;

/***************Function Declarations****************/
void displayMatrix(cv::Mat);
const char* resultCodeToString(ResultCode);

}/*namespace VV*/

#endif /*VV_COMMON_H*/
