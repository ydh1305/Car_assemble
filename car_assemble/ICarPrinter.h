#pragma once
#include "Util.h"
#include "CarConfig.h"
#include "ICompatibilityRule.h"

class PartRegistry;

class ICarPrinter {
public:
    virtual ~ICarPrinter() = default;
    virtual void printMenu(QuestionType step, const PartRegistry& reg) const = 0;
    virtual void printSelected(const ICarPart* part)                   const = 0;
    virtual void printRunResult(ValidationResult r, const CarConfig& c) const = 0;
    virtual void printTestResult(ValidationResult r,
                                 const ICompatibilityRule* failedRule) const = 0;
};
