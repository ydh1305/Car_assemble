#ifdef _DEBUG
#include "gmock/gmock.h"
#include "../CarAssembler.h"
#include "../ICarPrinter.h"
#include "../CarValidator.h"

using ::testing::_;
using ::testing::AnyNumber;
using ::testing::Return;

class MockCarPrinter : public ICarPrinter {
public:
    MOCK_METHOD(void, printMenu,       (QuestionType, const PartRegistry&), (const, override));
    MOCK_METHOD(void, printSelected,   (const ICarPart*),                   (const, override));
    MOCK_METHOD(void, printRunResult,  (ValidationResult, const CarConfig&),(const, override));
    MOCK_METHOD(void, printTestResult, (ValidationResult, const ICompatibilityRule*), (const, override));
};

class MockCarValidator : public CarValidator {
public:
    MOCK_METHOD(ValidationResult, validate, (const CarConfig&), (const, override));
};

TEST(CarAssemblerTest, ProcessSelection_CarType_AdvancesToEngineStep) {
    MockCarPrinter printer;
    PartRegistry registry;
    EXPECT_CALL(printer, printSelected(_)).Times(AnyNumber());
    CarAssembler assembler(&printer, nullptr, &registry);

    assembler.selectPart(CarType_Q, 1);
    EXPECT_EQ(Engine_Q, assembler.getCurrentStep());
}

TEST(CarAssemblerTest, ProcessSelection_BackNavigation_DecrementsStep) {
    MockCarPrinter printer;
    PartRegistry registry;
    EXPECT_CALL(printer, printSelected(_)).Times(AnyNumber());
    CarAssembler assembler(&printer, nullptr, &registry);

    assembler.selectPart(CarType_Q, 1);
    assembler.goBack();
    EXPECT_EQ(CarType_Q, assembler.getCurrentStep());
}

TEST(CarAssemblerTest, RunTest_CallsPrintTestResult) {
    MockCarPrinter printer;
    MockCarValidator validator;
    EXPECT_CALL(validator, validate(_)).WillOnce(Return(VALID));
    EXPECT_CALL(printer, printTestResult(VALID, _)).Times(1);

    CarAssembler assembler(&printer, &validator, nullptr);
    assembler.triggerTest();
}

TEST(CarAssemblerTest, RunTest_FailCase_CallsPrintTestResultWithFail) {
    MockCarPrinter printer;
    MockCarValidator validator;
    EXPECT_CALL(validator, validate(_)).WillOnce(Return(FAIL_SEDAN_CONTINENTAL));
    EXPECT_CALL(printer, printTestResult(FAIL_SEDAN_CONTINENTAL, _)).Times(1);

    CarAssembler assembler(&printer, &validator, nullptr);
    assembler.triggerTest();
}
#endif
