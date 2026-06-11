#pragma once

class ICarPart {
public:
    virtual ~ICarPart() = default;
    virtual const char* getName() const = 0;
    virtual int         getId()  const = 0;
};
