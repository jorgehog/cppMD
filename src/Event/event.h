#ifndef EVENT_H
#define EVENT_H

#include <string>

class MeshField;
class Ensemble;

class Event
{

private:

    MeshField* meshField;
    Ensemble* ensemble;

    std::string type;
    std::string unit;


public:

    virtual bool isSolverEvent(){
        return false;
    }

    Event(std::string type = "Event", std::string unit = "");

    virtual void apply(int i) = 0;

    virtual double getMeasurement(){
        return 0.0;
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
