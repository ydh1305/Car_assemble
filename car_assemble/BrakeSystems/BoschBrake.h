#pragma once
#include "../IBrakeSystem.h"

class BoschBrake : public IBrakeSystem {
public:
    const char* getName() const override { return "Bosch"; }
    int         getId()  const override { return 3; }
};
