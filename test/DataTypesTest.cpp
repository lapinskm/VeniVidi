#include "VVTestBase.hpp"
#include "DataTypes.hpp"
#include "common.hpp"

using namespace VV;
using namespace cv;

/*************************FIXTURE SECTION**************************/
class dataTypesTest :public VVTestBase
{
  protected:
    vector<Point2d> createTestPoint2dVector(int StartValue, int size)
    {
      std::vector<Point2d> points;
      for(int i = 0; i < size; i++)
      {
        points.push_back(Point2d(StartValue+i, StartValue+i));
      }
      return points;
    }

    DataPoint2dVector createTestDataPoint2dVector(int VectorSize)
    {
       Mat descriptor = Mat::ones(1, 5, numericDataType);
       Mat descriptors = Mat::zeros(1, 5, numericDataType);

       std::vector<Point2d> points = createTestPoint2dVector(0,VectorSize);

       for(int i = 1; i < VectorSize; i++)
       {
         descriptor+=descriptor;
         descriptors.push_back(descriptor);
       }
       return DataPoint2dVector(points, descriptors);
    }

    void displayPoint2dVector(vector<Point2d> points)
    {
      for (int i=0; i < points.size(); i++)
      {
         VVLOG("[x=%f y=%f]\n",points[i].x,points[i].y);
      }
     VVLOG("size=%lu\n",points.size());
    }
};

/**************************TESTS SECTION***************************/
TEST_F(dataTypesTest, DataPoint2dVector_sanityCheck_positiveCase)
{
  DataPoint2dVector vector  = createTestDataPoint2dVector(5);
  EXPECT_TRUE(vector.sanityCheck());
}

TEST_F(dataTypesTest, DataPoint2dVector_sanityCheck_negativeCase)
{
  DataPoint2dVector properVector1 = createTestDataPoint2dVector(4);
  DataPoint2dVector properVector2 = createTestDataPoint2dVector(5);

  DataPoint2dVector badVector  = DataPoint2dVector();
  EXPECT_FALSE(badVector.sanityCheck());

  badVector = DataPoint2dVector(properVector1.getPoints(),Mat());
  EXPECT_FALSE(badVector.sanityCheck());

  badVector = DataPoint2dVector(std::vector<Point2d>(),
                                properVector1.getDescriptors());
  EXPECT_FALSE(badVector.sanityCheck());

  badVector = DataPoint2dVector(properVector2.getPoints(),
                                properVector1.getDescriptors());
  EXPECT_FALSE(badVector.sanityCheck());

}

TEST_F(dataTypesTest, DataPoint2dVector_getPoints)
{
  DataPoint2dVector vect  = createTestDataPoint2dVector(5);
  std::vector<Point2d> points = createTestPoint2dVector(0, 5);
  EXPECT_TRUE(vect.getPoints() == points);
}

TEST_F(dataTypesTest, DataPoint2dVector_getDescriptors)
{
  Mat desriptors = Mat::eye(5, 5, numericDataType);
  DataPoint2dVector vect = DataPoint2dVector(std::vector<Point2d>(),
                                             desriptors);

  EXPECT_TRUE(matsEqual(desriptors, vect.getDescriptors(), 0));
}

TEST_F(dataTypesTest, DataPoint2dVector_getSize)
{
  DataPoint2dVector vect=createTestDataPoint2dVector(5);
  EXPECT_EQ(5, vect.getSize());
}

TEST_F(dataTypesTest, DataPoint2dVector_getDataSubset_properParams)
{
  DataPoint2dVector vector  = createTestDataPoint2dVector(10);
  displayPoint2dVector(vector.getPoints());

  ASSERT_TRUE(vector.sanityCheck());

  DataPoint2dVector subVector1 = vector.getDataSubset(0, 5);

  ASSERT_TRUE(subVector1.sanityCheck());

  VVLOG("[ Test Body] Following Matrices should be the same:\n");
  VVLOG_MAT(vector.getDescriptors().rowRange(0, 5));
  VVLOG_MAT( subVector1.getDescriptors());

  EXPECT_TRUE( matsEqual( vector.getDescriptors().rowRange(0, 5),
                          subVector1.getDescriptors(), 0));

  std::vector<Point2d> points = createTestPoint2dVector(0, 5);

  VVLOG("[ Test Body] Following point vectors should be the same:\n");

  displayPoint2dVector(points);
  VVLOG("\n");

  displayPoint2dVector(subVector1.getPoints());
  VVLOG("\n");

  EXPECT_TRUE(subVector1.getPoints() == points);
}

TEST_F(dataTypesTest, DataPoint2dVector_getDataSubset_badParams)
{
  DataPoint2dVector vector  = createTestDataPoint2dVector(5);
  DataPoint2dVector badSubVector = vector.getDataSubset(3, 2);
  EXPECT_FALSE(badSubVector.sanityCheck());

  badSubVector = vector.getDataSubset(0, 5);
  EXPECT_FALSE(badSubVector.sanityCheck());

  badSubVector = vector.getDataSubset(0, 20);
  EXPECT_FALSE(badSubVector.sanityCheck());

}

