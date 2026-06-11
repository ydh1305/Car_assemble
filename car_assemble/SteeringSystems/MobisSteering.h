#pragma once
#include "../ISteeringSystem.h"

class MobisSteering : public ISteeringSystem {
public:
    const char* getName() const override { return "Mobis"; }
    int         getId()  const override { return 2; }
};
