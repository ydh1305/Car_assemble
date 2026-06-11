#pragma once
#include "ICarType.h"
#include "IEngine.h"
#include "IBrakeSystem.h"
#include "ISteeringSystem.h"

class PartRegistry {
public:
    PartRegistry();
    ~PartRegistry();

    const ICarType*        getCarType(int id) const;
    const IEngine*         getEngine(int id) const;
    const IBrakeSystem*    getBrakeSystem(int id) const;
    const ISteeringSystem* getSteeringSystem(int id) const;

private:
    ICarType*        m_carTypes[4];
    IEngine*         m_engines[6];
    IBrakeSystem*    m_brakeSystems[4];
    ISteeringSystem* m_steeringSystems[3];
};
