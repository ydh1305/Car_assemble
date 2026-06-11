#pragma once
#include "../IEngine.h"

class WIAEngine : public IEngine {
public:
    const char* getName() const override { return "WIA"; }
    int         getId()  const override { return 3; }
    bool        isSelected() const override { return true; }
};
