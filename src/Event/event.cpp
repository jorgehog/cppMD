#include "event.h"

#include "../MeshField/meshfield.h"

#include <sstream>
#include <iomanip>

Event::Event(std::string type, std::string unit):
    type(type),
    unit(unit),
    value(new double(0)),
    valueInitialized(false)
{
    initialize();
    reset();
}


std::string Event::dumpString()
{
    std::stringstream s;

    s << "<"

      << type << "@"
      << meshField->description;
    if (valueInitialized){
      s << ";\t" << "value: " << std::setprecision(3) << getMeasurement() << " " << unit;
    }

     s << " >";

     valueInitialized = false;

    return s.str();
}
