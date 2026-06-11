#pragma once
#include "../IBrakeSystem.h"

class ContinentalBrake : public IBrakeSystem {
public:
    const char* getName() const override { return "Continental"; }
    int         getId()  const override { return 2; }
};
