#ifndef EVENT_H
#define EVENT_H

#include "../MeshField/meshfield.h"

#include <iostream>
#include <assert.h>


struct Ensemble;

class Event
{
protected:

    int N;
    const int * loopCycle;

    static int counter;
    int id;

    int address; //! This event's index in meshfield's event array

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


public:

    Event(std::string type = "Event", std::string unit = "", bool doOutput=false, bool toFile=false);


    int getId() {
        return id;
    }

    std::string getType(){
        return type;
    }

    std::string getUnit() {
        return unit;
    }

    static int getCounter(){
        return counter;
    }

    bool notSilent(){
        return doOutput;
    }

    virtual void executeEvent() {
        execute();
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

    void setN(int N){
        this->N = N;
    }

    void setLoopCyclePtr(const int* loopCycle){
        this->loopCycle = loopCycle;
    }

    void setAddress(int i) {
        address = i;
    }

    std::string dumpString();

};


#define UNSET_EVENT_TIME -1

class TriggerEvent : public Event {
public:

    TriggerEvent(std::string type = "TriggerEvent", std::string unit = "", bool doOutput=false) :
        Event(type, unit, doOutput) {}

    void setTrigger(int t){
        assert(t >= 0);
        trigger = t;
    }

    void executeEvent() {

        assert(trigger != UNSET_EVENT_TIME);

        doOutput = false;

        if (*loopCycle == trigger){
            execute();
            doOutput = true;
        } else if (*loopCycle == trigger + 1) {
            meshField->removeEvent(this->address);
        }

    }

protected:

    int trigger = UNSET_EVENT_TIME;

};



class OnsetEvent : public Event {
public:

    OnsetEvent(std::string type = "OnsetEvent", std::string unit = "", bool doOutput=false, bool toFile=false) :
        Event(type, unit, false, toFile),
        doOutputOrig(doOutput),
        nTimesExecuted(0)
    {

    }

    void setOnsetTime(int onsetTime){

        if (onsetTime == UNSET_EVENT_TIME) return;

        assert(onsetTime >= 0);

        this->onsetTime = onsetTime;

    }

    void setOffsetTime(int offTime) {

        if (offTime == UNSET_EVENT_TIME) return;

        assert((onsetTime != UNSET_EVENT_TIME) && ("onTime must be set before offTime."));
        assert(offTime > onsetTime && "Event must initialize before the shutdown.");
        assert(offTime >= 0 && "event shutdown must be a positive value");

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

    int getOnsetTime() {
        return onsetTime;
    }

    int getOffsetTime() {
        return offsetTime;
    }

protected:

    int onsetTime = UNSET_EVENT_TIME;
    int offsetTime = UNSET_EVENT_TIME;

    bool doOutputOrig;

    int eventLength = UNSET_EVENT_TIME;

    int nTimesExecuted;
};
#endif // EVENT_H
