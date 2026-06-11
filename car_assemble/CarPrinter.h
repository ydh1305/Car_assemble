#pragma once
#include "ICarPrinter.h"

class CarPrinter : public ICarPrinter {
public:
    void printMenu(QuestionType step, const PartRegistry& reg) const override;
    void printSelected(const ICarPart* part) const override;
    void printRunResult(ValidationResult r, const CarConfig& c) const override;
    void printTestResult(ValidationResult r, const ICompatibilityRule* failedRule) const override;
};
