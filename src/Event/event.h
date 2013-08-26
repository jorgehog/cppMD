#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <iostream>

class MeshField;
class Ensemble;

class Event
{
protected:

    double* value;

    MeshField* meshField;
    Ensemble* ensemble;

    std::string type;
    std::string unit;

    bool valueInitialized;

public:

    Event(std::string type = "Event", std::string unit = "");

    virtual void execute() = 0;

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


    std::string dumpString();

};

#endif // EVENT_H
