#include "meshField.h"

#include <sstream>
#include "../region/region.h"

std::string meshField::dumpString()
{
    std::stringstream s;

    s << "<"

      << type << ", "
      << reg->description << ", "
      << "value: " << getMeasurement() << " "
      << unit

      << " >";

    return s.str();
}
