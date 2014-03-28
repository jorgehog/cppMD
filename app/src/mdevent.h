#pragma once

#include <ignis.h>

#include "mdpositionhandler.h"

using ignis::Event;

class MDEvent : public Event<double, MDParticles<10, 10>>
{

public:

    MDEvent(std::string type = "MDEvent", std::string unit = "", bool doOutput = false, bool toFile = false)
        : Event(type, unit, doOutput, toFile)
    {

    }

protected:

    vec getTotalLinearMomentum();

    void cancelLinearMomentum();

    double getKineticEnergy();

    double getTemperature();

};
