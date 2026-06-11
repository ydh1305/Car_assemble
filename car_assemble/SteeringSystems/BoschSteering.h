#pragma once
#include "../ISteeringSystem.h"

class BoschSteering : public ISteeringSystem {
public:
    const char* getName() const override { return "Bosch"; }
    int         getId()  const override { return 1; }
};
