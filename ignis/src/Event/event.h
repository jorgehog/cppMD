#ifndef EVENT_H
#define EVENT_H

#define UNSET_EVENT_TIME 999999999

#include "../MeshField/meshfield.h"

#include <iostream>
#include <iomanip>

#include <assert.h>


struct Ensemble;

class Event
{
protected:

    static uint N;

    static const uint *loopCycle;

    static mat observables;
    static std::vector<std::string> outputTypes;

    static uint counter;
    uint id;

    static uint priorityCounter;
    uint priority;

    uint address; //! This event's index in meshfield's event array

    double* value;

    MeshField* meshField;
    Ensemble* ensemble;

    std::string type;
    std::string unit;

    bool valueInitialized;

    bool doOutput;
    bool toFile;

    virtual void execute() = 0;

    std::vector<int> myAtoms () {
        return meshField->getAtoms();
    }

    uint onsetTime = UNSET_EVENT_TIME;

    uint offsetTime = UNSET_EVENT_TIME;

public:

    Event(std::string type = "Event", std::string unit = "", bool doOutput=false, bool toFile=false);


    uint getId() {
        return id;
    }

    static uint getPriorityCounter() {
        return priorityCounter;
    }

    std::string getType(){
        return type;
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

    uint getPriority () const {
        return priority;
    }


    bool notSilent(){
        return doOutput;
    }

    void storeEvent();

    virtual void executeEvent() {
        execute();
    }

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

    virtual void initialize(){}

    virtual void reset(){}

    virtual double getMeasurement(){
        return *value;
    }

    bool shouldToFile(){
        return toFile;
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

    void setPriority();

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

        assert(onsetTime != UNSET_EVENT_TIME);

        doOutput = false;

        if (*loopCycle == onsetTime){
            execute();
            doOutput = true;
        } else if (*loopCycle == onsetTime + 1) {
            meshField->removeEvent(this->address);
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

        if (onsetTime == UNSET_EVENT_TIME) return;

        this->onsetTime = onsetTime;

    }

    void setOffsetTime(uint offTime) {

        if (offTime == UNSET_EVENT_TIME) return;

        assert((onsetTime != UNSET_EVENT_TIME) && ("onTime must be set before offTime."));
        assert(offTime > onsetTime && "Event must initialize before the shutdown.");

        offsetTime = offTime;

        eventLength = offTime - onsetTime;

    }

    void executeEvent() {

        assert(onsetTime != UNSET_EVENT_TIME);

        if (*loopCycle == offsetTime) {
            meshField->removeEvent(this->address);
        }

        if (*loopCycle == onsetTime) {
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

    int eventLength = UNSET_EVENT_TIME;

    int nTimesExecuted;
};
#endif // EVENT_H
