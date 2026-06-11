#pragma once
#include "../IEngine.h"

class NoEngine : public IEngine {
public:
    const char* getName() const override { return "NoEngine"; }
    int         getId()  const override { return 4; }
    bool        isSelected() const override { return false; }
};
