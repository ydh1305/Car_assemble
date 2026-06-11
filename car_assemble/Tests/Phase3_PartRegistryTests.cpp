#ifdef _DEBUG
#include "gmock/gmock.h"
#include "../PartRegistry.h"
#include "../Engines/NoEngine.h"

TEST(PartRegistryTest, GetCarType_1_ReturnsSedanType) {
    PartRegistry reg;
    EXPECT_EQ(1, reg.getCarType(1)->getId());
    EXPECT_STREQ("Sedan", reg.getCarType(1)->getName());
}
TEST(PartRegistryTest, GetCarType_2_ReturnsSUV) {
    EXPECT_EQ(2, PartRegistry().getCarType(2)->getId());
}
TEST(PartRegistryTest, GetCarType_3_ReturnsTruck) {
    EXPECT_EQ(3, PartRegistry().getCarType(3)->getId());
}
TEST(PartRegistryTest, GetCarType_InvalidId_ReturnsNull) {
    EXPECT_EQ(nullptr, PartRegistry().getCarType(0));
    EXPECT_EQ(nullptr, PartRegistry().getCarType(4));
}

TEST(PartRegistryTest, GetEngine_4_ReturnsNoEngine) {
    NoEngine* ne = dynamic_cast<NoEngine*>(
        const_cast<IEngine*>(PartRegistry().getEngine(4)));
    EXPECT_NE(nullptr, ne);
}
TEST(PartRegistryTest, GetEngine_InvalidId_ReturnsNull) {
    EXPECT_EQ(nullptr, PartRegistry().getEngine(0));
    EXPECT_EQ(nullptr, PartRegistry().getEngine(5));
}

TEST(PartRegistryTest, GetBrakeSystem_AllValid) {
    PartRegistry reg;
    EXPECT_EQ(1, reg.getBrakeSystem(1)->getId());
    EXPECT_EQ(2, reg.getBrakeSystem(2)->getId());
    EXPECT_EQ(3, reg.getBrakeSystem(3)->getId());
}
TEST(PartRegistryTest, GetSteering_AllValid) {
    PartRegistry reg;
    EXPECT_EQ(1, reg.getSteeringSystem(1)->getId());
    EXPECT_EQ(2, reg.getSteeringSystem(2)->getId());
}
#endif
