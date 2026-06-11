#pragma once
#include "../ICompatibilityRule.h"

class TruckWIARule : public ICompatibilityRule {
public:
    bool isFailed(const CarConfig& c) const override {
        return c.carType && c.engine &&
               c.carType->getId() == 3 && c.engine->getId() == 3;
    }
    const char* getFailReason() const override {
        return "Truck에서 WIA엔진 조합 불가";
    }
};
