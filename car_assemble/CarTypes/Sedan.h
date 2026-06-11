#pragma once
#include "../ICarType.h"

class Sedan : public ICarType {
public:
    const char* getName() const override { return "Sedan"; }
    int         getId()  const override { return 1; }
};
