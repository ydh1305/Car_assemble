#pragma once
#include "../IEngine.h"

class ToyotaEngine : public IEngine {
public:
    const char* getName() const override { return "TOYOTA"; }
    int         getId()  const override { return 2; }
    bool        isSelected() const override { return true; }
};
