#pragma once
#include "Util.h"
#include "CarConfig.h"
#include "CarValidator.h"
#include "ICarPrinter.h"
#include "PartRegistry.h"

class CarAssembler {
public:
    CarAssembler();
    CarAssembler(ICarPrinter* printer, CarValidator* validator, PartRegistry* registry);
    ~CarAssembler();

    void run();

    // Test accessors
    QuestionType getCurrentStep() const { return m_step; }
    void selectPart(QuestionType step, int value);
    void goBack();
    void triggerTest();
    void triggerRun();

private:
    CarConfig     m_config;
    CarValidator* m_validator;
    ICarPrinter*  m_printer;
    PartRegistry* m_registry;
    QuestionType  m_step;
    bool          m_ownsObjects;

    void processSelection(int answer);
};
