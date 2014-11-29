#include "utils.hpp"
#include "common.hpp"

#include <opencv2/opencv.hpp>
#include <string>

using namespace VV;
using namespace cv;
using std::string;
using std::pair;


bool VV::imagesAlmostSame(Mat& image1, Mat& image2)
{
  //there is no need to check anything when images have different size or
  //any of images have no data
  if ( image1.rows != image2.rows ||
       image1.cols != image2.cols ||
      !image1.data || !image2.data )
  {
    return false;
  }

  //Maximum mean of absolute difference between images, Magic number.
  const double deltaMax = 30;

  Mat diff;
  absdiff(image1, image2, diff);
  //Checked only one channel, works for grayscale only
  if (mean(diff).val[0] > deltaMax)
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool imageIsProcessworthy(Mat& image, Mat& previousImage)
{
  if( !image.data )
  {
     return false;
  }

  if( image.rows == 0 || image.cols == 0 )
  {
    return false;
  }

  //Check if this is image is not too similar to previous one
  //(e.g. camera stayed in the same place between images)
  if( imagesAlmostSame(image, previousImage) )
  {
    return false;
  }

  return true;
}

ResultCode loadImage(const string& path, Mat& image)
{
  if ( path.empty() )
  {
    return wrongParams;
  }
  image = imread(path, CV_LOAD_IMAGE_GRAYSCALE);
  return(image.data ? success : failure);
}

//returns vector of pairs of indexes determining optimal segmentation
segmentation VV::segmentate(unsigned threadCount,
                        unsigned dataSize)
{
  segmentation segm;

  if( 0 == dataSize || 0 == threadCount )
  { //return empty vector
    return segm;
  }

  if(dataSize <= threadCount)
  {
    for (int i=0; i < dataSize; i++)
    {
      segm.push_back(pair<unsigned, unsigned>(i,i));
    }
    return segm;
  }

  unsigned partSize = dataSize / threadCount;

  unsigned partStart = 0;

  // + 1 is to remove factoral element in division
  // - 1 is due to indexing is starting on 0
  unsigned partEnd = partSize + 1 - 1;

  unsigned unSegmentedDataSize = dataSize;

  while(0 != unSegmentedDataSize % partSize)
  {
   segm.push_back(pair<unsigned, unsigned>(partStart, partEnd));
   //calculating next indexes
   partStart = partEnd + 1;
   partEnd = partStart + partSize;
   unSegmentedDataSize -= (partSize + 1);
  }

  partEnd--;//rest parts is partSize (no partSize+1), so should be 1 smaller.
  //now we can segmentate rest without spare data.
  while(0 != unSegmentedDataSize)
  {
    segm.push_back(pair<unsigned, unsigned>(partStart, partEnd));
    //calculating next indexes
    partStart = partEnd + 1;
    partEnd = partStart + partSize - 1;
    unSegmentedDataSize -= partSize;
  }

  return segm;
}

