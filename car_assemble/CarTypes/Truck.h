#pragma once
#include "../ICarType.h"

class Truck : public ICarType {
public:
    const char* getName() const override { return "Truck"; }
    int         getId()  const override { return 3; }
};
