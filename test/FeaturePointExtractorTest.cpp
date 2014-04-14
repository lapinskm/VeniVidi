#include "gtest/gtest.h"
#include "FeaturePointExtractor.hpp"

// To use a test fixture, derive a class from testing::Test.
class FeaturePointExtractorTest : public testing::Test
{
 protected:  // You should make the members protected s.t. they can be
             // accessed from sub-classes.

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  //virtual void SetUp() {
  //}

  // virtual void TearDown() will be called after each test is run.
  // You should define it if there is cleanup work to do.  Otherwise,
  // you don't have to provide it.
  //
  //virtual void TearDown() {
  //}

  // A helper function
  //void helper(void* param){
  //}

  // Declares the variables your tests want to use.
  Mat image;
  //int foo;
  //int bar;
};

// When you have a test fixture, you define a test using TEST_F
// instead of TEST.

// Tests the default c'tor.
TEST_F(FeaturePointExtractorTest, DefaultConstructor) 
{
  // You can access data in the test fixture here.
}


TEST_F(FeaturePointExtractorTest, CallbackIsCalled)
{

}

TEST_F(FeaturePointExtractorTest, LenaImageCase)
{

}

TEST_F(FeaturePointExtractorTest, BlankImageCase)
{

}



