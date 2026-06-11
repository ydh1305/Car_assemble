#ifdef _DEBUG
#include "gmock/gmock.h"
#include "../CarTypes/Sedan.h"
#include "../CarTypes/SUV.h"
#include "../CarTypes/Truck.h"
#include "../Engines/GMEngine.h"
#include "../Engines/ToyotaEngine.h"
#include "../Engines/WIAEngine.h"
#include "../Engines/NoEngine.h"
#include "../BrakeSystems/MandoBrake.h"
#include "../BrakeSystems/ContinentalBrake.h"
#include "../BrakeSystems/BoschBrake.h"
#include "../SteeringSystems/BoschSteering.h"
#include "../SteeringSystems/MobisSteering.h"

TEST(CarTypeTest, Sedan_Name)  { Sedan s; EXPECT_STREQ("Sedan", s.getName()); }
TEST(CarTypeTest, Sedan_Id)    { Sedan s; EXPECT_EQ(1, s.getId()); }
TEST(CarTypeTest, SUV_Name)    { SUV s;   EXPECT_STREQ("SUV",   s.getName()); }
TEST(CarTypeTest, SUV_Id)      { SUV s;   EXPECT_EQ(2, s.getId()); }
TEST(CarTypeTest, Truck_Name)  { Truck t; EXPECT_STREQ("Truck", t.getName()); }
TEST(CarTypeTest, Truck_Id)    { Truck t; EXPECT_EQ(3, t.getId()); }

TEST(EngineTest, GM_IsSelected)        { GMEngine e;     EXPECT_TRUE(e.isSelected()); }
TEST(EngineTest, Toyota_IsSelected)    { ToyotaEngine e; EXPECT_TRUE(e.isSelected()); }
TEST(EngineTest, WIA_IsSelected)       { WIAEngine e;    EXPECT_TRUE(e.isSelected()); }
TEST(EngineTest, NoEngine_NotSelected) { NoEngine e;     EXPECT_FALSE(e.isSelected()); }
TEST(EngineTest, NoEngine_Id)          { NoEngine e;     EXPECT_EQ(4, e.getId()); }
TEST(EngineTest, GM_Id)                { GMEngine e;     EXPECT_EQ(1, e.getId()); }
TEST(EngineTest, Toyota_Id)            { ToyotaEngine e; EXPECT_EQ(2, e.getId()); }
TEST(EngineTest, WIA_Id)               { WIAEngine e;    EXPECT_EQ(3, e.getId()); }

TEST(BrakeSystemTest, Mando_Name)       { MandoBrake b;       EXPECT_STREQ("Mando",       b.getName()); }
TEST(BrakeSystemTest, Continental_Name) { ContinentalBrake b; EXPECT_STREQ("Continental", b.getName()); }
TEST(BrakeSystemTest, Bosch_Name)       { BoschBrake b;       EXPECT_STREQ("Bosch",       b.getName()); }
TEST(BrakeSystemTest, Mando_Id)         { MandoBrake b;       EXPECT_EQ(1, b.getId()); }
TEST(BrakeSystemTest, Continental_Id)   { ContinentalBrake b; EXPECT_EQ(2, b.getId()); }
TEST(BrakeSystemTest, Bosch_Id)         { BoschBrake b;       EXPECT_EQ(3, b.getId()); }

TEST(SteeringTest, Bosch_Name)  { BoschSteering s;  EXPECT_STREQ("Bosch", s.getName()); }
TEST(SteeringTest, Mobis_Name)  { MobisSteering s;  EXPECT_STREQ("Mobis", s.getName()); }
TEST(SteeringTest, Bosch_Id)    { BoschSteering s;  EXPECT_EQ(1, s.getId()); }
TEST(SteeringTest, Mobis_Id)    { MobisSteering s;  EXPECT_EQ(2, s.getId()); }
#endif
