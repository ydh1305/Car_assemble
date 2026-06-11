#pragma once
#include "../ICompatibilityRule.h"

class SuvToyotaRule : public ICompatibilityRule {
public:
    bool isFailed(const CarConfig& c) const override {
        return c.carType && c.engine &&
               c.carType->getId() == 2 && c.engine->getId() == 2;
    }
    const char* getFailReason() const override {
        return "SUV에서 TOYOTA엔진 조합 불가";
    }
};
