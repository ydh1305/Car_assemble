#include "CarPrinter.h"
#include "PartRegistry.h"
#include <stdio.h>

#define CLEAR_SCREEN "\033[H\033[2J"

void CarPrinter::printMenu(QuestionType step, const PartRegistry& reg) const
{
    switch (step)
    {
    case CarType_Q:
        printf(CLEAR_SCREEN);
        printf("        ______________\n");
        printf("       /|            | \n");
        printf("  ____/_|_____________|____\n");
        printf(" |                      O  |\n");
        printf(" '-(@)----------------(@)--'\n");
        printf("===============================\n");
        printf("어떤 차량 타입을 선택하시겠습니까?\n");
        printf("1. Sedan\n");
        printf("2. SUV\n");
        printf("3. Truck\n");
        break;
    case Engine_Q:
        printf(CLEAR_SCREEN);
        printf("어떤 엔진을 탑재하시겠습니까?\n");
        printf("0. 뒤로가기\n");
        printf("1. GM\n");
        printf("2. TOYOTA\n");
        printf("3. WIA\n");
        printf("4. 선택안 하셈\n");
        break;
    case brakeSystem_Q:
        printf(CLEAR_SCREEN);
        printf("어떤 제동장치를 선택하시겠습니까?\n");
        printf("0. 뒤로가기\n");
        printf("1. MANDO\n");
        printf("2. CONTINENTAL\n");
        printf("3. BOSCH\n");
        break;
    case SteeringSystem_Q:
        printf(CLEAR_SCREEN);
        printf("어떤 조향장치를 선택하시겠습니까?\n");
        printf("0. 뒤로가기\n");
        printf("1. BOSCH\n");
        printf("2. MOBIS\n");
        break;
    case Run_Test:
        printf(CLEAR_SCREEN);
        printf("차량 조립이 완성되었습니다.\n");
        printf("어떻게 하시겠습니까?\n");
        printf("0. 처음 화면으로 돌아가기\n");
        printf("1. RUN\n");
        printf("2. Test\n");
        break;
    }
    printf("===============================\n");
}

void CarPrinter::printSelected(const ICarPart* part) const
{
    if (part)
        printf("%s 을(를) 선택하셨습니다.\n", part->getName());
}

void CarPrinter::printRunResult(ValidationResult r, const CarConfig& c) const
{
    if (r != VALID)
    {
        printf("차량이 동작하지 않습니다\n");
        return;
    }
    if (c.engine && !c.engine->isSelected())
    {
        printf("엔진이 없어나졌습니다.\n");
        printf("차량이 동작하지 않습니다.\n");
        return;
    }
    if (c.carType)        printf("Car Type : %s\n",       c.carType->getName());
    if (c.engine)         printf("Engine : %s\n",          c.engine->getName());
    if (c.brakeSystem)    printf("Brake System : %s\n",    c.brakeSystem->getName());
    if (c.steeringSystem) printf("SteeringSystem : %s\n",  c.steeringSystem->getName());
    printf("차량이 동작됩니다.\n");
}

void CarPrinter::printTestResult(ValidationResult r, const ICompatibilityRule* failedRule) const
{
    if (r == VALID)
    {
        printf("차량의 부품 조합 테스트 결과 : PASS\n");
        return;
    }
    printf("차량의 부품 조합 테스트 결과 : FAIL\n");
    if (failedRule)
        printf("%s\n", failedRule->getFailReason());
}
