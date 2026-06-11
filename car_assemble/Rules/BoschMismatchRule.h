#pragma once
#include "../ICompatibilityRule.h"

class BoschMismatchRule : public ICompatibilityRule {
public:
    bool isFailed(const CarConfig& c) const override {
        return c.brakeSystem && c.steeringSystem &&
               c.brakeSystem->getId() == 3 && c.steeringSystem->getId() != 1;
    }
    const char* getFailReason() const override {
        return "Bosch제동장치에서 Bosch조향장치 이외 조합 불가";
    }
};
