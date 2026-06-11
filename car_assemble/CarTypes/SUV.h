#pragma once
#include "../ICarType.h"

class SUV : public ICarType {
public:
    const char* getName() const override { return "SUV"; }
    int         getId()  const override { return 2; }
};
