#pragma once
#include "CarConfig.h"

class ICompatibilityRule {
public:
    virtual ~ICompatibilityRule() = default;
    virtual bool        isFailed(const CarConfig& config) const = 0;
    virtual const char* getFailReason() const = 0;
};
