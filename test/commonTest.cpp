#include "VVTestBase.hpp"
#include "common.hpp"

using namespace VV;


/*************************FIXTURE SECTION**************************/
class commonTest :public VVTestBase
{
};

/**************************TESTS SECTION***************************/

TEST_F(commonTest, resultCodeToString_properParams)
{
  ASSERT_STREQ("success"         , resultCodeToString(success));
  ASSERT_STREQ("wrongParams"     , resultCodeToString(wrongParams));
  ASSERT_STREQ("executionFailure", resultCodeToString(executionFailure));
  ASSERT_STREQ("nothingToDo"     , resultCodeToString(nothingToDo));
  ASSERT_STREQ("postponed"       , resultCodeToString(postponed));
  ASSERT_STREQ("failure"         , resultCodeToString(failure));
}

