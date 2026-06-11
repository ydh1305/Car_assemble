#pragma once
#include "../IBrakeSystem.h"

class MandoBrake : public IBrakeSystem {
public:
    const char* getName() const override { return "Mando"; }
    int         getId()  const override { return 1; }
};
