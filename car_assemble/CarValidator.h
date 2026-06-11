#pragma once
#include <vector>
#include "Util.h"
#include "CarConfig.h"
#include "ICompatibilityRule.h"

class CarValidator {
public:
    CarValidator();
    explicit CarValidator(std::vector<ICompatibilityRule*> rules);
    ~CarValidator();

    virtual ValidationResult validate(const CarConfig& config) const;

private:
    std::vector<ICompatibilityRule*> m_rules;
    bool m_ownsRules;
};
