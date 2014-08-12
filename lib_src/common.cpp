#include "common.hpp"

std::mutex VVLogMtx;

void displayMatrix(cv::Mat mat)
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

