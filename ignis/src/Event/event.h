#ifndef EVENT_H
#define EVENT_H


#include "../defines.h"

#include "../MeshField/meshfield.h"

#include <iostream>
#include <iomanip>

#include <assert.h>


namespace ignis
{

struct Ensemble;

class Event
{
protected:

    static uint N;

    static const uint *loopCycle;


    std::string type;


    static std::vector<std::string> outputTypes;


    static uint priorityCounter;

    uint priority;

    uint address; //! This event's index in meshfield's event array


    static uint counter;

    uint id;

    double* value;

    bool valueInitialized;

    bool doOutput;

    bool toFile;

    std::string unit;

    static mat observables;


    MeshField* meshField;

    Ensemble* ensemble;


    virtual void execute() = 0;


    uint onsetTime = IGNIS_UNSET_UINT;

    uint offsetTime = IGNIS_UNSET_UINT;

public:

    Event(std::string type = "Event", std::string unit = "", bool doOutput=false, bool toFile=false);


    virtual void executeEvent() {
        execute();
    }

    virtual void initialize(){}

    virtual void reset(){}


    uint getId() {
        return id;
    }


    void setPriority();

    uint getPriority () const {
        return priority;
    }

    static uint getPriorityCounter() {
        return priorityCounter;
    }


    std::string getType(){
        return type;
    }

    bool shouldToFile(){
        return toFile;
    }

    bool notSilent(){
        return doOutput;
    }

    std::string getUnit() {
        return unit;
    }

    static uint getCounter(){
        return counter;
    }


    uint getOnsetTime() {
        return onsetTime;
    }

    uint getOffsetTime() {
        return offsetTime;
    }


    virtual double getMeasurement(){
        return *value;
    }


    void setValue(double value){
        valueInitialized = true;
        *(this->value) = value;
    }

    void setEnsemble(Ensemble* ensemble){
        this->ensemble = ensemble;
    }

    void setMeshField(MeshField* meshField){
        this->meshField = meshField;
    }

    static void setN(uint & N){
        Event::N = N;
    }

    static void setLoopCyclePtr(const uint* loopCycle){
        Event::loopCycle = loopCycle;
    }

    void setAddress(uint i) {
        address = i;
    }



    void storeEvent();

    void setOutputVariables();

    static void initializeEventMatrix() {
        assert(N!=0 && "Unset or empty number of cycles");

        observables.zeros(N, getCounter());
    }

    static void dumpEventMatrixData(uint k) {
        for (uint i = 0; i < getCounter(); ++i) {
            cout << std::setw(30) << std::left << outputTypes.at(i) << "  " << std::setw(10) << observables(k, i) << endl;
        }
    }

    static void saveEventMatrix(std::string path) {
        observables.save(path);
    }

    std::string dumpString();

};



class TriggerEvent : public Event {
public:

    TriggerEvent(std::string type = "TriggerEvent", std::string unit = "", bool doOutput=false) :
        Event(type, unit, doOutput) {}

    void setTrigger(uint t){

        onsetTime = t;
        offsetTime = t+1;

    }

    void executeEvent() {

        assert(onsetTime != IGNIS_UNSET_UINT);

        doOutput = false;

        if (*loopCycle == onsetTime){

            if (onsetTime != 0)
            {
                initialize();
            }

            execute();
            doOutput = true;
        }

    }
};



class OnsetEvent : public Event {
public:

    OnsetEvent(std::string type = "OnsetEvent", std::string unit = "", bool doOutput=false, bool toFile=false) :
        Event(type, unit, false, toFile),
        doOutputOrig(doOutput),
        nTimesExecuted(0)
    {

    }

    void setOnsetTime(uint onsetTime){

        if (onsetTime == IGNIS_UNSET_UINT) return;

        this->onsetTime = onsetTime;

    }

    void setOffsetTime(uint offTime) {

        if (offTime == IGNIS_UNSET_UINT) return;

        assert((onsetTime != IGNIS_UNSET_UINT) && ("onTime must be set before offTime."));
        assert(offTime > onsetTime && "Event must initialize before the shutdown.");

        offsetTime = offTime;

        eventLength = offTime - onsetTime;

    }

    void executeEvent() {

        assert(*loopCycle <= offsetTime && "Event was not properly removed.");

        if (*loopCycle == onsetTime && onsetTime != 0) {
            initialize();
        }

        if (*loopCycle >= onsetTime) {
            execute();
            doOutput = doOutputOrig;
            nTimesExecuted++;
        }


    }

protected:

    bool doOutputOrig;

    uint eventLength = IGNIS_UNSET_UINT;

    uint nTimesExecuted;
};
}

#endif // EVENT_H
