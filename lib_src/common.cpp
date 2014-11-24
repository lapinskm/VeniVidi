#include "common.hpp"

std::mutex VVLogMtx;

void VV::displayMatrix(cv::Mat mat)
{
   for(int i=0; i<mat.rows; i++)
   {
      printf("[");
      for (int j=0; j<mat.cols; j++ )
      {
         printf("%13.8f, ", mat.at<double>(i,j));
      }
      printf("]\n");
   }
   printf ("cols = %d rows = %d\n\n", mat.cols, mat.rows);
}

const char* VV::resultCodeToString(VV::ResultCode code)
{
  switch(code)
  {
    case success:
      return "success";

    case wrongParams:
      return "wrongParams";

    case executionFailure:
      return "executionFailure";

    case nothingToDo:
      return "nothingToDo";

    case postponed:
      return "postponed";

    case failure:
      return "failure";

    default:
      return "unknown";
  }
}
