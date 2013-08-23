#include "event.h"

#include "../MeshField/meshfield.h"

#include <sstream>

Event::Event(std::string type, std::string unit):
    type(type),
    unit(unit)
{
    value = 0;
    reset();
}


std::string Event::dumpString()
{
    std::stringstream s;

    s << "<"

      << type << ", "
      << meshField->description << ", "
      << "value: " << getMeasurement() << " "
      << unit

      << " >";

    return s.str();
}
