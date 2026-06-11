#pragma once
#include "../ICompatibilityRule.h"

class TruckMandoRule : public ICompatibilityRule {
public:
    bool isFailed(const CarConfig& c) const override {
        return c.carType && c.brakeSystem &&
               c.carType->getId() == 3 && c.brakeSystem->getId() == 1;
    }
    const char* getFailReason() const override {
        return "Truck에서 Mando제동장치 조합 불가";
    }
};
