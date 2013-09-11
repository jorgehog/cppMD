#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <iostream>
#include <assert.h>

class MeshField;
struct Ensemble;

class Event
{
protected:

    int N;
    int * loopCycle;

    double* value;

    MeshField* meshField;
    Ensemble* ensemble;

    std::string type;
    std::string unit;

    bool valueInitialized;

    bool doOutput;

    virtual void execute() = 0;


public:

    Event(std::string type = "Event", std::string unit = "", bool doOutput=false);


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

    void setLoopCyclePtr(int* loopCycle){
        this->loopCycle = loopCycle;
    }

    std::string dumpString();

};

class TriggerEvent : public Event {
public:

    TriggerEvent(std::string type = "TriggerEvent", std::string unit = "", bool doOutput=false) :
        Event(type, unit, doOutput) {}

    void setTrigger(int t){
        assert(t >= 0);
        trigger = t;
    }

    void executeEvent() {

        assert(trigger != -1);

        doOutput = false;

        if (*loopCycle == trigger){
            execute();
            doOutput = true;
        }
    }

protected:

    int trigger = -1;

};


class OnsetEvent : public Event {
public:

    OnsetEvent(std::string type = "OnsetEvent", std::string unit = "", bool doOutput=false) :
        Event(type, unit, false),
        doOutputOrig(doOutput) {}

    void setOnsetTime(int onsetTime){
        assert(onsetTime >= 0);

        this->onsetTime = onsetTime;
    }

    void setOffsetTime(int offTime) {
        assert(offTime >= 0);

        offsetTime = offTime;
    }

    void executeEvent() {

        assert(onsetTime != -1);

        if (*loopCycle == offsetTime) {
            shouldExecute = false;
            doOutput = false;
        }

        if ((*loopCycle > onsetTime) && (shouldExecute)) {
            execute();
            doOutput = doOutputOrig;
        }

    }

protected:

    int onsetTime = -1;
    int offsetTime = -1;

    bool shouldExecute = true;
    bool doOutputOrig;

};

#endif // EVENT_H
