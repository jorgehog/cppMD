#ifndef MD_MESHFIELD_H
#define MD_MESHFIELD_H

#include "../defines.h"

#include <armadillo>
using namespace arma;

#include <string>
#include <vector>

class Ensemble;
class Event;

class MeshField
{

protected:

    int depth;

    int nSubFields;
    int nEvents;

    double volume;

    Ensemble *ensemble;
    MeshField* parent;

    std::vector<int> atoms;
    std::vector<Event*> events;
    std::vector<MeshField*> subFields;

    bool checkSubFields(int i);

    void resetSubFields();
    void resetEvents();
    void executeEvents();
    void dumpEvents();

    bool append(int i) {

        if (isWithinThis(i)){
            atoms.push_back(i);
            return true;
        }

        return false;
    }


    bool notCompatible(MeshField & subField);

    void resetContents(){
        atoms.clear();
    }

    void setParent(MeshField* parent){
        this->parent = parent;
        depth = parent->depth + 1;
    }

public:

    MeshField(const mat & topology, Ensemble &ensemble,
              const std::string description = "meshField");

    const mat::fixed<ENS_DIM, 2> topology;
    const vec::fixed<ENS_DIM> shape;

    const std::string description;

    void addEvent(Event & event);

    void addSubField(MeshField &subField);

    virtual bool isWithinThis(int i);

    const vec & getShape() const {
        return shape;
    }

    const double & getVolume() const {
        return volume;
    }

    virtual int getPopulation() const {
        return atoms.size();
    }

    friend class MainMesh;


};




#endif // MD_MESHFIELD_H
