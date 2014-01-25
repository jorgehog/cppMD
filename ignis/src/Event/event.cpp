#include "event.h"

#include <sstream>
#include <iomanip>

#define UNSET_PRIORITY -2

Event::Event(std::string type, std::string unit, bool doOutput, bool toFile):
    priority(UNSET_PRIORITY),
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

void Event::setPriority()
{
    assert(priority == UNSET_PRIORITY && "Priority of event set twice.");
    priority = priorityCounter++;

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
int Event::priorityCounter = 0;
