#include "event.h"

#include "../MeshField/meshfield.h"

#include <sstream>
#include <iomanip>

Event::Event(std::string type, std::string unit):
    type(type),
    unit(unit)
{
    value = 0;
    initialize();
    reset();
}


std::string Event::dumpString()
{
    std::stringstream s;

    s << "<"

      << type << "@"
      << meshField->description << ";\t"
      << "value: " << std::setprecision(3) << getMeasurement() << " "
      << unit

      << " >";

    return s.str();
}
