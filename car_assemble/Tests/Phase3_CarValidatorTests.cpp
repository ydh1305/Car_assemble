#ifdef _DEBUG
#include "gmock/gmock.h"
#include "../CarConfig.h"
#include "../CarValidator.h"
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

using ::testing::_;
using ::testing::Return;

TEST(CarValidatorTest, Valid_Sedan_GM_Mando_Bosch) {
    Sedan s; GMEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(VALID, CarValidator().validate(c));
}
TEST(CarValidatorTest, Valid_Truck_GM_Bosch_Bosch) {
    Truck t; GMEngine e; BoschBrake b; BoschSteering st;
    CarConfig c{&t, &e, &b, &st};
    EXPECT_EQ(VALID, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_Sedan_Continental) {
    Sedan s; GMEngine e; ContinentalBrake b; BoschSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(FAIL_SEDAN_CONTINENTAL, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_SUV_Toyota) {
    SUV s; ToyotaEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(FAIL_SUV_TOYOTA, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_Truck_WIA) {
    Truck t; WIAEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&t, &e, &b, &st};
    EXPECT_EQ(FAIL_TRUCK_WIA, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_Truck_Mando) {
    Truck t; GMEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&t, &e, &b, &st};
    EXPECT_EQ(FAIL_TRUCK_MANDO, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_Bosch_Mismatch) {
    Sedan s; GMEngine e; BoschBrake b; MobisSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(FAIL_BOSCH_MISMATCH, CarValidator().validate(c));
}

class MockCompatibilityRule : public ICompatibilityRule {
public:
    MOCK_METHOD(bool,        isFailed,      (const CarConfig&), (const, override));
    MOCK_METHOD(const char*, getFailReason, (),                 (const, override));
};

TEST(CarValidatorTest, CallsAllRulesOnValidConfig) {
    MockCompatibilityRule rule1, rule2;
    EXPECT_CALL(rule1, isFailed(_)).WillOnce(Return(false));
    EXPECT_CALL(rule2, isFailed(_)).WillOnce(Return(false));

    CarValidator validator({&rule1, &rule2});
    CarConfig c;
    EXPECT_EQ(VALID, validator.validate(c));
}

TEST(CarValidatorTest, StopsAtFirstFailedRule) {
    MockCompatibilityRule rule1, rule2;
    EXPECT_CALL(rule1, isFailed(_)).WillOnce(Return(true));
    EXPECT_CALL(rule2, isFailed(_)).Times(0);

    CarValidator validator({&rule1, &rule2});
    CarConfig c;
    EXPECT_NE(VALID, validator.validate(c));
}
#endif
