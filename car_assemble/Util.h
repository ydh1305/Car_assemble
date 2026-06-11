#pragma once

enum QuestionType { CarType_Q, Engine_Q, brakeSystem_Q, SteeringSystem_Q, Run_Test };
enum ValidationResult { VALID, FAIL_SEDAN_CONTINENTAL, FAIL_SUV_TOYOTA, FAIL_TRUCK_WIA, FAIL_TRUCK_MANDO, FAIL_BOSCH_MISMATCH };

static const int VALID_RANGE[][2] = { {1,3},{0,4},{0,3},{0,2},{0,2} };
inline bool isInRange(int step, int answer) {
    return answer >= VALID_RANGE[step][0] && answer <= VALID_RANGE[step][1];
}
