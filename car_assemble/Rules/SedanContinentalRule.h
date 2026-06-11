#pragma once
#include "../ICompatibilityRule.h"
#include "../CarTypes/Sedan.h"
#include "../BrakeSystems/ContinentalBrake.h"

class SedanContinentalRule : public ICompatibilityRule {
public:
    bool isFailed(const CarConfig& c) const override {
        return c.carType && c.brakeSystem &&
               c.carType->getId() == 1 && c.brakeSystem->getId() == 2;
    }
    const char* getFailReason() const override {
        return "Sedan에서 Continental제동장치 조합 불가";
    }
};
