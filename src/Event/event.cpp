#include "event.h"

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
    using namespace std;

    stringstream s, tail;

    s << left
      << "<" << setw(20) << type << " "
      << "@" << setw(30) << meshField->description;

    if (valueInitialized){
        tail << "value: " << setprecision(3) << getMeasurement() << " " << unit;
        valueInitialized = false;
    }

    tail << " >";

    s << right << tail.str();


    return s.str();
}

int Event::counter = 0;

