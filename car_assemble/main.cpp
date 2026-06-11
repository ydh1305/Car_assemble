#include "Util.h"

#ifdef _DEBUG

#include "gmock/gmock.h"

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define CLEAR_SCREEN "\033[H\033[2J"

int stack[10];

void selectCarType(int answer);
void selectEngine(int answer);
void selectbrakeSystem(int answer);
void selectSteeringSystem(int answer);
void runProducedCar();
void testProducedCar();

int main()
{
    char buf[100];
    int step = CarType_Q;

    while (1)
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

        printf("INPUT > ");
        fgets(buf, sizeof(buf), stdin);

        // 개행 문자열 제거
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!strcmp(buf, "exit"))
        {
            printf("안녕히가세요\n");
            break;
        }

        // 숫자로 된 입력인지 확인
        char* checkNumber;
        int answer = strtol(buf, &checkNumber, 10);

        // 입력받은 값이 숫자가 아니라면
        if (*checkNumber != '\0')
        {
            printf("ERROR :: 숫자를 입력 바람\n");
            Sleep(800);
            continue;
        }

        if (!isInRange(step, answer))
        {
            switch (step)
            {
            case CarType_Q:
                printf("ERROR :: 차량 타입은 1 ~ 3 번만을 선택 바람\n");
                break;
            case Engine_Q:
                printf("ERROR :: 엔진은 1 ~ 4 번만을 선택 바람\n");
                break;
            case brakeSystem_Q:
                printf("ERROR :: 제동장치는 1 ~ 3 번만을 선택 바람\n");
                break;
            case SteeringSystem_Q:
                printf("ERROR :: 조향장치는 1 ~ 2 번만을 선택 바람\n");
                break;
            case Run_Test:
                printf("ERROR :: Run 또는 Test 중 하나를 선택 필요\n");
                break;
            }
            Sleep(800);
            continue;
        }

        // 처음화면으로 돌아가기
        if (answer == 0 && step == Run_Test)
        {
            step = CarType_Q;
            continue;
        }

        // 이전으로 돌아가기
        if (answer == 0 && step >= 1)
        {
            step -= 1;
            continue;
        }

        switch (step)
        {
        case CarType_Q:
            selectCarType(answer);
            Sleep(800);
            step = Engine_Q;
            break;
        case Engine_Q:
            selectEngine(answer);
            Sleep(800);
            step = brakeSystem_Q;
            break;
        case brakeSystem_Q:
            selectbrakeSystem(answer);
            Sleep(800);
            step = SteeringSystem_Q;
            break;
        case SteeringSystem_Q:
            selectSteeringSystem(answer);
            Sleep(800);
            step = Run_Test;
            break;
        case Run_Test:
            if (answer == 1)
            {
                runProducedCar();
                Sleep(2000);
            }
            else if (answer == 2)
            {
                printf("Test...\n");
                Sleep(1500);
                testProducedCar();
                Sleep(2000);
            }
            break;
        }
    }
}

void selectCarType(int answer)
{
    stack[CarType_Q] = answer;
    switch (answer)
    {
    case SEDAN: printf("차량 타입으로 Sedan을 선택하셨습니다.\n"); break;
    case SUV:   printf("차량 타입으로 SUV를 선택하셨습니다.\n");   break;
    case TRUCK: printf("차량 타입으로 Truck을 선택하셨습니다.\n"); break;
    }
}

void selectEngine(int answer)
{
    stack[Engine_Q] = answer;
    switch (answer)
    {
    case GM:     printf("GM 엔진을 선택하셨습니다.\n");     break;
    case TOYOTA: printf("TOYOTA 엔진을 선택하셨습니다.\n"); break;
    case WIA:    printf("WIA 엔진을 선택하셨습니다.\n");    break;
    }
}

void selectbrakeSystem(int answer)
{
    stack[brakeSystem_Q] = answer;
    switch (answer)
    {
    case MANDO:       printf("MANDO 제동장치를 선택하셨습니다.\n");       break;
    case CONTINENTAL: printf("CONTINENTAL 제동장치를 선택하셨습니다.\n"); break;
    case BOSCH_B:     printf("BOSCH 제동장치를 선택하셨습니다.\n");       break;
    }
}

void selectSteeringSystem(int answer)
{
    stack[SteeringSystem_Q] = answer;
    switch (answer)
    {
    case BOSCH_S: printf("BOSCH 조향장치를 선택하셨습니다.\n"); break;
    case MOBIS:   printf("MOBIS 조향장치를 선택하셨습니다.\n"); break;
    }
}

ValidationResult isValidCheck()
{
    if (stack[CarType_Q] == SEDAN && stack[brakeSystem_Q] == CONTINENTAL)
        return FAIL_SEDAN_CONTINENTAL;
    if (stack[CarType_Q] == SUV && stack[Engine_Q] == TOYOTA)
        return FAIL_SUV_TOYOTA;
    if (stack[CarType_Q] == TRUCK && stack[Engine_Q] == WIA)
        return FAIL_TRUCK_WIA;
    if (stack[CarType_Q] == TRUCK && stack[brakeSystem_Q] == MANDO)
        return FAIL_TRUCK_MANDO;
    if (stack[brakeSystem_Q] == BOSCH_B && stack[SteeringSystem_Q] != BOSCH_S)
        return FAIL_BOSCH_MISMATCH;
    return VALID;
}

void runProducedCar()
{
    ValidationResult result = isValidCheck();
    if (result != VALID)
    {
        printf("차량이 동작하지 않습니다\n");
        return;
    }

    if (stack[Engine_Q] == 4)
    {
        printf("엔진이 없어나졌습니다.\n");
        printf("차량이 동작하지 않습니다.\n");
        return;
    }

    switch (stack[CarType_Q])
    {
    case SEDAN: printf("Car Type : Sedan\n"); break;
    case SUV:   printf("Car Type : SUV\n");   break;
    case TRUCK: printf("Car Type : Truck\n"); break;
    }

    switch (stack[Engine_Q])
    {
    case GM:     printf("Engine : GM\n");     break;
    case TOYOTA: printf("Engine : TOYOTA\n"); break;
    case WIA:    printf("Engine : WIA\n");    break;
    }

    switch (stack[brakeSystem_Q])
    {
    case MANDO:       printf("Brake System : Mando\n");       break;
    case CONTINENTAL: printf("Brake System : Continental\n"); break;
    case BOSCH_B:     printf("Brake System : Bosch\n");       break;
    }

    switch (stack[SteeringSystem_Q])
    {
    case BOSCH_S: printf("SteeringSystem : Bosch\n"); break;
    case MOBIS:   printf("SteeringSystem : Mobis\n"); break;
    }

    printf("차량이 동작됩니다.\n");
}

void testProducedCar()
{
    ValidationResult result = isValidCheck();
    if (result == VALID)
    {
        printf("차량의 부품 조합 테스트 결과 : PASS\n");
        return;
    }

    printf("차량의 부품 조합 테스트 결과 : FAIL\n");
    switch (result)
    {
    case FAIL_SEDAN_CONTINENTAL:
        printf("Sedan에서 Continental제동장치 조합 불가\n");
        break;
    case FAIL_SUV_TOYOTA:
        printf("SUV에서 TOYOTA엔진 조합 불가\n");
        break;
    case FAIL_TRUCK_WIA:
        printf("Truck에서 WIA엔진 조합 불가\n");
        break;
    case FAIL_TRUCK_MANDO:
        printf("Truck에서 Mando제동장치 조합 불가\n");
        break;
    case FAIL_BOSCH_MISMATCH:
        printf("Bosch제동장치에서 Bosch조향장치 이외 조합 불가\n");
        break;
    default:
        break;
    }
}

#endif
