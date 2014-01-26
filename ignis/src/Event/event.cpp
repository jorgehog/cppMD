#include "event.h"

#include "../defines.h"

#include <sstream>
#include <iomanip>

using namespace ignis;

Event::Event(std::string type, std::string unit, bool doOutput, bool toFile):
    type(type),
    priority(IGNIS_UNSET_UINT),
    value(new double(0)),
    valueInitialized(false),
    doOutput(doOutput),
    toFile(toFile),
    unit(unit),
    onsetTime()
{

}

void Event::storeEvent()
{
    if (!toFile) {
        return;
    }

    observables(*loopCycle, id) = *value;

}

void Event::setOutputVariables()
{
    if (toFile) {
        id = counter++;
        cout << type << endl;
        outputTypes.push_back(type + ("@" + meshField->description));
    }
}

void Event::setPriority()
{
    assert(priority == IGNIS_UNSET_UINT && "Priority of event set twice.");
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

/*
   Static member variables:
*/

const uint * Event::loopCycle;

std::vector<std::string> Event::outputTypes;

mat Event::observables;

uint Event::N = 0;

uint Event::counter = 0;

uint Event::priorityCounter = 0;
