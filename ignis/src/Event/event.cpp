#include "event.h"

#include <sstream>
#include <iomanip>

#define UNSET_PRIORITY 999999999

Event::Event(std::string type, std::string unit, bool doOutput, bool toFile):
    priority(UNSET_PRIORITY),
    value(new double(0)),
    type(type),
    unit(unit),
    valueInitialized(false),
    doOutput(doOutput),
    toFile(toFile)
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

/*
   Static member variables:
*/

const uint * Event::loopCycle;

std::vector<std::string> Event::outputTypes;

mat Event::observables;

uint Event::N = 0;

uint Event::counter = 0;

uint Event::priorityCounter = 0;
