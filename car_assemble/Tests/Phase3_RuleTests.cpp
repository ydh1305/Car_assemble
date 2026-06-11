#ifdef _DEBUG
#include "gmock/gmock.h"
#include "../CarConfig.h"
#include "../CarTypes/Sedan.h"
#include "../CarTypes/SUV.h"
#include "../CarTypes/Truck.h"
#include "../Engines/GMEngine.h"
#include "../Engines/ToyotaEngine.h"
#include "../Engines/WIAEngine.h"
#include "../BrakeSystems/MandoBrake.h"
#include "../BrakeSystems/ContinentalBrake.h"
#include "../BrakeSystems/BoschBrake.h"
#include "../SteeringSystems/BoschSteering.h"
#include "../SteeringSystems/MobisSteering.h"
#include "../Rules/SedanContinentalRule.h"
#include "../Rules/SuvToyotaRule.h"
#include "../Rules/TruckWIARule.h"
#include "../Rules/TruckMandoRule.h"
#include "../Rules/BoschMismatchRule.h"

TEST(SedanContinentalRuleTest, Fails_When_Sedan_Continental) {
    Sedan sedan; ContinentalBrake brake;
    CarConfig c; c.carType = &sedan; c.brakeSystem = &brake;
    EXPECT_TRUE(SedanContinentalRule().isFailed(c));
}
TEST(SedanContinentalRuleTest, Pass_When_SUV_Continental) {
    SUV suv; ContinentalBrake brake;
    CarConfig c; c.carType = &suv; c.brakeSystem = &brake;
    EXPECT_FALSE(SedanContinentalRule().isFailed(c));
}
TEST(SedanContinentalRuleTest, Pass_When_Sedan_Mando) {
    Sedan sedan; MandoBrake brake;
    CarConfig c; c.carType = &sedan; c.brakeSystem = &brake;
    EXPECT_FALSE(SedanContinentalRule().isFailed(c));
}

TEST(SuvToyotaRuleTest, Fails_When_SUV_Toyota) {
    SUV suv; ToyotaEngine eng;
    CarConfig c; c.carType = &suv; c.engine = &eng;
    EXPECT_TRUE(SuvToyotaRule().isFailed(c));
}
TEST(SuvToyotaRuleTest, Pass_When_Sedan_Toyota) {
    Sedan sedan; ToyotaEngine eng;
    CarConfig c; c.carType = &sedan; c.engine = &eng;
    EXPECT_FALSE(SuvToyotaRule().isFailed(c));
}
TEST(SuvToyotaRuleTest, Pass_When_SUV_GM) {
    SUV suv; GMEngine eng;
    CarConfig c; c.carType = &suv; c.engine = &eng;
    EXPECT_FALSE(SuvToyotaRule().isFailed(c));
}

TEST(TruckWIARuleTest, Fails_When_Truck_WIA) {
    Truck truck; WIAEngine eng;
    CarConfig c; c.carType = &truck; c.engine = &eng;
    EXPECT_TRUE(TruckWIARule().isFailed(c));
}
TEST(TruckWIARuleTest, Pass_When_Sedan_WIA) {
    Sedan sedan; WIAEngine eng;
    CarConfig c; c.carType = &sedan; c.engine = &eng;
    EXPECT_FALSE(TruckWIARule().isFailed(c));
}

TEST(TruckMandoRuleTest, Fails_When_Truck_Mando) {
    Truck truck; MandoBrake brake;
    CarConfig c; c.carType = &truck; c.brakeSystem = &brake;
    EXPECT_TRUE(TruckMandoRule().isFailed(c));
}
TEST(TruckMandoRuleTest, Pass_When_SUV_Mando) {
    SUV suv; MandoBrake brake;
    CarConfig c; c.carType = &suv; c.brakeSystem = &brake;
    EXPECT_FALSE(TruckMandoRule().isFailed(c));
}

TEST(BoschMismatchRuleTest, Fails_When_BoschBrake_MobisSteering) {
    BoschBrake brake; MobisSteering steering;
    CarConfig c; c.brakeSystem = &brake; c.steeringSystem = &steering;
    EXPECT_TRUE(BoschMismatchRule().isFailed(c));
}
TEST(BoschMismatchRuleTest, Pass_When_BoschBrake_BoschSteering) {
    BoschBrake brake; BoschSteering steering;
    CarConfig c; c.brakeSystem = &brake; c.steeringSystem = &steering;
    EXPECT_FALSE(BoschMismatchRule().isFailed(c));
}
TEST(BoschMismatchRuleTest, Pass_When_MandoBrake_MobisSteering) {
    MandoBrake brake; MobisSteering steering;
    CarConfig c; c.brakeSystem = &brake; c.steeringSystem = &steering;
    EXPECT_FALSE(BoschMismatchRule().isFailed(c));
}

TEST(AllRulesTest, FailReasonNotEmpty) {
    EXPECT_STRNE("", SedanContinentalRule().getFailReason());
    EXPECT_STRNE("", SuvToyotaRule().getFailReason());
    EXPECT_STRNE("", TruckWIARule().getFailReason());
    EXPECT_STRNE("", TruckMandoRule().getFailReason());
    EXPECT_STRNE("", BoschMismatchRule().getFailReason());
}
#endif
