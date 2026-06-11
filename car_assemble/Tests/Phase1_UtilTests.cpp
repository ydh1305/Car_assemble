#ifdef _DEBUG
#include "gmock/gmock.h"
#include "../Util.h"

TEST(IsInRangeTest, CarTypeQ_MinValid)  { EXPECT_TRUE(isInRange(CarType_Q, 1)); }
TEST(IsInRangeTest, CarTypeQ_MaxValid)  { EXPECT_TRUE(isInRange(CarType_Q, 3)); }
TEST(IsInRangeTest, CarTypeQ_BelowMin) { EXPECT_FALSE(isInRange(CarType_Q, 0)); }
TEST(IsInRangeTest, CarTypeQ_AboveMax) { EXPECT_FALSE(isInRange(CarType_Q, 4)); }
TEST(IsInRangeTest, EngineQ_BackValid) { EXPECT_TRUE(isInRange(Engine_Q, 0)); }
TEST(IsInRangeTest, EngineQ_NoEngine)  { EXPECT_TRUE(isInRange(Engine_Q, 4)); }
TEST(IsInRangeTest, EngineQ_AboveMax)  { EXPECT_FALSE(isInRange(Engine_Q, 5)); }
TEST(IsInRangeTest, BrakeQ_MinValid)   { EXPECT_TRUE(isInRange(brakeSystem_Q, 1)); }
TEST(IsInRangeTest, BrakeQ_MaxValid)   { EXPECT_TRUE(isInRange(brakeSystem_Q, 3)); }
TEST(IsInRangeTest, BrakeQ_AboveMax)   { EXPECT_FALSE(isInRange(brakeSystem_Q, 4)); }
TEST(IsInRangeTest, SteeringQ_MinValid){ EXPECT_TRUE(isInRange(SteeringSystem_Q, 1)); }
TEST(IsInRangeTest, SteeringQ_MaxValid){ EXPECT_TRUE(isInRange(SteeringSystem_Q, 2)); }
TEST(IsInRangeTest, SteeringQ_AboveMax){ EXPECT_FALSE(isInRange(SteeringSystem_Q, 3)); }
TEST(IsInRangeTest, RunTestQ_MinValid) { EXPECT_TRUE(isInRange(Run_Test, 0)); }
TEST(IsInRangeTest, RunTestQ_MaxValid) { EXPECT_TRUE(isInRange(Run_Test, 2)); }
TEST(IsInRangeTest, RunTestQ_AboveMax) { EXPECT_FALSE(isInRange(Run_Test, 3)); }
#endif
