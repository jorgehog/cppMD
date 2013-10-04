#include "event.h"

#include "../MeshField/meshfield.h"

#include <sstream>
#include <iomanip>

Event::Event(std::string type, std::string unit, bool doOutput, bool toFile):
    value(new double(0)),
    type(type),
    unit(unit),
    valueInitialized(false),
    doOutput(doOutput),
    toFile(toFile)
{
    if (toFile) {
        id = counter++;
    }
}


std::string Event::dumpString()
{
    std::stringstream s;

    s << "<"

      << type << "@"
      << meshField->description;
    if (valueInitialized){
        s << ";\t" << "value: " << std::setprecision(3) << getMeasurement() << " " << unit;
        valueInitialized = false;
    }

    s << " >";


    return s.str();
}

int Event::counter = 0;

