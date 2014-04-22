#include <thread>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

typedef void (*finishCallback)(void* inputData,void* outputData);

class FeaturePointExtractor
{
  public:
    FeaturePointExtractor(){};
    ~FeaturePointExtractor(){};
    static void startExtraction(Mat * image,
                               finishCallback cb,
                               void* userData);
  private:
    struct threadRoutineData
    {
       Mat * image;
       finishCallback cb;
       void* userData;
    };

    static void extractorThreadRoutine(void* data);
};

