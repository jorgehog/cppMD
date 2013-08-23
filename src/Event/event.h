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

    Event(std::string type, std::string unit);

    virtual void apply(int i) = 0;

    void setEnsemble(Ensemble* ensemble){
        this->ensemble = ensemble;
    }

    void setMeshField(MeshField* meshField){
        this->meshField = meshField;
    }


};

#endif // EVENT_H
