#pragma once
#include "ICarType.h"
#include "IEngine.h"
#include "IBrakeSystem.h"
#include "ISteeringSystem.h"

struct CarConfig {
    const ICarType*        carType        = nullptr;
    const IEngine*         engine         = nullptr;
    const IBrakeSystem*    brakeSystem    = nullptr;
    const ISteeringSystem* steeringSystem = nullptr;
};
