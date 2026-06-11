#pragma once
#include "../IEngine.h"

class GMEngine : public IEngine {
public:
    const char* getName() const override { return "GM"; }
    int         getId()  const override { return 1; }
    bool        isSelected() const override { return true; }
};
