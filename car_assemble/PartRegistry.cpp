#include "PartRegistry.h"
#include "CarTypes/Sedan.h"
#include "CarTypes/SUV.h"
#include "CarTypes/Truck.h"
#include "Engines/GMEngine.h"
#include "Engines/ToyotaEngine.h"
#include "Engines/WIAEngine.h"
#include "Engines/NoEngine.h"
#include "BrakeSystems/MandoBrake.h"
#include "BrakeSystems/ContinentalBrake.h"
#include "BrakeSystems/BoschBrake.h"
#include "SteeringSystems/BoschSteering.h"
#include "SteeringSystems/MobisSteering.h"
#include <cstring>

PartRegistry::PartRegistry()
{
    memset(m_carTypes, 0, sizeof(m_carTypes));
    memset(m_engines, 0, sizeof(m_engines));
    memset(m_brakeSystems, 0, sizeof(m_brakeSystems));
    memset(m_steeringSystems, 0, sizeof(m_steeringSystems));

    m_carTypes[1] = new Sedan();
    m_carTypes[2] = new SUV();
    m_carTypes[3] = new Truck();

    m_engines[1] = new GMEngine();
    m_engines[2] = new ToyotaEngine();
    m_engines[3] = new WIAEngine();
    m_engines[4] = new NoEngine();

    m_brakeSystems[1] = new MandoBrake();
    m_brakeSystems[2] = new ContinentalBrake();
    m_brakeSystems[3] = new BoschBrake();

    m_steeringSystems[1] = new BoschSteering();
    m_steeringSystems[2] = new MobisSteering();
}

PartRegistry::~PartRegistry()
{
    for (int i = 0; i < 4; ++i) delete m_carTypes[i];
    for (int i = 0; i < 6; ++i) delete m_engines[i];
    for (int i = 0; i < 4; ++i) delete m_brakeSystems[i];
    for (int i = 0; i < 3; ++i) delete m_steeringSystems[i];
}

const ICarType* PartRegistry::getCarType(int id) const
{
    if (id < 1 || id > 3) return nullptr;
    return m_carTypes[id];
}

const IEngine* PartRegistry::getEngine(int id) const
{
    if (id < 1 || id > 4) return nullptr;
    return m_engines[id];
}

const IBrakeSystem* PartRegistry::getBrakeSystem(int id) const
{
    if (id < 1 || id > 3) return nullptr;
    return m_brakeSystems[id];
}

const ISteeringSystem* PartRegistry::getSteeringSystem(int id) const
{
    if (id < 1 || id > 2) return nullptr;
    return m_steeringSystems[id];
}
