#pragma once
#include "ICarPart.h"

class IEngine : public ICarPart {
public:
    virtual bool isSelected() const = 0;
};
