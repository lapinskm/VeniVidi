#ifndef VV_SCENE_GENERATOR_HPP
#define VV_SCENE_GENERATOR_HPP

#include <opencv2/opencv.hpp>

#include "FeaturePointMatcher.hpp"
#include "FeaturePointExtractor.hpp"
#include "ImagePreprocessor.hpp"
#include "DataManager.hpp"

namespace VV{

class SceneGenerator
{
  public:
    SceneGenerator(){};
    ~SceneGenerator(){};

    void processImage(const char* path);
    DataManager dataManagerGet() {return dmgr;}

  private:
    /*Finish callbacks for async modules*/
    static void FeaturePointExtractorFinishCb(void* inputData,
                                              void* outputData);

    static void FeaturePointMatcherFinishCb(void* inputData,
                                            void* outputData);

  private:
    /*submodules used to process image data*/

    //image monochromization and distortion removal
    ImagePreprocessor iprep;
    //self expalnatory
    FeaturePointExtractor fpext;
    FeaturePointMatcher fpmtch;

    /*Not implemeted features*/
    //module increasing feature point amount by search in existing
    // point neighbourhood
    //FeaturePointDensifier fpdnsf;

    /*Data storing module*/
    DataManager dmgr;
};
struct FeaturePointExtractorUserData
{
  SceneGenerator* owner;
  Mat* image;
};

}/*namespace VV*/

#endif /*VV_SCENE_GENERATOR_HPP*/
