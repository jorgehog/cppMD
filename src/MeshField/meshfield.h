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

    const mat::fixed<MD_DIM, 2> topology;
    const vec::fixed<MD_DIM> shape;

    std::vector<int> atoms;
    std::vector<Event*> events;
    std::vector<MeshField*> subFields;

    bool checkSubFields(int i);

public:

    MeshField(const mat & topology, Ensemble &ensemble,
              const std::string description = "meshField");

    const std::string description;

    virtual bool isWithinThis(int i);

    void resetSubFields();

    bool notCompatible(MeshField & subField);

    void addEvent(Event & event);

    void addSubField(MeshField &subField);

    void setParent(MeshField* parent){
        this->parent = parent;
        depth = parent->depth + 1;
    }

    bool append(int i) {

        if (isWithinThis(i)){
            atoms.push_back(i);
            return true;
        }

        return false;
    }

    void reset(){
        atoms.clear();
    }

    const vec & getShape() const {
        return shape;
    }

    const double & getVolume() const {
        return volume;
    }

    friend class MainMesh;


};



/*
    INLINE FUNCTION IMPLEMENTATIONS
*/





#endif // MD_MESHFIELD_H
