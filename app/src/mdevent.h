#pragma once

#include <ignis.h>

#include "mdpositionhandler.h"

using ignis::Event;

class MDEvent : public Event<double>
{

public:

    MDEvent(std::string type = "MDEvent", std::string unit = "", bool doOutput = false, bool toFile = false)
        : Event<double>(type, unit, doOutput, toFile)
    {

    }

protected:

    MDParticles & particles()
    {
        return *static_cast<MDParticles*>(&Event<double>::particles());
    }

    vec getTotalLinearMomentum();

    void cancelLinearMomentum();

    double getKineticEnergy();

    double getTemperature();

};
