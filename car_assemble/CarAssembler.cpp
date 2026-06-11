#include "CarAssembler.h"
#include "CarPrinter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

CarAssembler::CarAssembler()
    : m_step(CarType_Q), m_ownsObjects(true)
{
    m_registry  = new PartRegistry();
    m_validator = new CarValidator();
    m_printer   = new CarPrinter();
}

CarAssembler::CarAssembler(ICarPrinter* printer, CarValidator* validator, PartRegistry* registry)
    : m_printer(printer), m_validator(validator), m_registry(registry),
      m_step(CarType_Q), m_ownsObjects(false)
{
}

CarAssembler::~CarAssembler()
{
    if (m_ownsObjects)
    {
        delete m_registry;
        delete m_validator;
        delete m_printer;
    }
}

void CarAssembler::selectPart(QuestionType step, int value)
{
    if (!m_registry) return;
    switch (step)
    {
    case CarType_Q:
        m_config.carType = m_registry->getCarType(value);
        if (m_printer) m_printer->printSelected(m_config.carType);
        m_step = Engine_Q;
        break;
    case Engine_Q:
        m_config.engine = m_registry->getEngine(value);
        if (m_printer) m_printer->printSelected(m_config.engine);
        m_step = brakeSystem_Q;
        break;
    case brakeSystem_Q:
        m_config.brakeSystem = m_registry->getBrakeSystem(value);
        if (m_printer) m_printer->printSelected(m_config.brakeSystem);
        m_step = SteeringSystem_Q;
        break;
    case SteeringSystem_Q:
        m_config.steeringSystem = m_registry->getSteeringSystem(value);
        if (m_printer) m_printer->printSelected(m_config.steeringSystem);
        m_step = Run_Test;
        break;
    default:
        break;
    }
}

void CarAssembler::goBack()
{
    if (m_step > CarType_Q)
        m_step = static_cast<QuestionType>(m_step - 1);
}

void CarAssembler::triggerTest()
{
    if (!m_validator || !m_printer) return;
    ValidationResult r = m_validator->validate(m_config);
    // Find the failed rule for printing
    m_printer->printTestResult(r, nullptr);
}

void CarAssembler::triggerRun()
{
    if (!m_validator || !m_printer) return;
    ValidationResult r = m_validator->validate(m_config);
    m_printer->printRunResult(r, m_config);
}

void CarAssembler::processSelection(int answer)
{
    // Unused: logic is in run()
}

void CarAssembler::run()
{
    char buf[100];

    while (1)
    {
        if (m_printer && m_registry)
            m_printer->printMenu(m_step, *m_registry);

        printf("INPUT > ");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!strcmp(buf, "exit"))
        {
            printf("안녕히가세요\n");
            break;
        }

        char* checkNumber;
        int answer = strtol(buf, &checkNumber, 10);

        if (*checkNumber != '\0')
        {
            printf("ERROR :: 숫자를 입력 바람\n");
            Sleep(800);
            continue;
        }

        if (!isInRange(m_step, answer))
        {
            printf("ERROR :: 올바른 번호를 입력 바람\n");
            Sleep(800);
            continue;
        }

        if (answer == 0 && m_step == Run_Test)
        {
            m_step = CarType_Q;
            m_config = CarConfig{};
            continue;
        }

        if (answer == 0 && m_step >= 1)
        {
            goBack();
            continue;
        }

        if (m_step == Run_Test)
        {
            if (answer == 1)
            {
                triggerRun();
                Sleep(2000);
            }
            else if (answer == 2)
            {
                printf("Test...\n");
                Sleep(1500);
                triggerTest();
                Sleep(2000);
            }
        }
        else
        {
            selectPart(m_step, answer);
            Sleep(800);
        }
    }
}
