#ifndef MD_MESHFIELD_H
#define MD_MESHFIELD_H

#include "../defines.h"

#include <armadillo>
using namespace arma;

#include <string>
#include <vector>

struct Ensemble;
class Event;

class MeshField
{

protected:

    double volume;

    Ensemble *ensemble;

    std::vector<int> atoms;
    std::vector<Event*> events;
    std::vector<MeshField*> subFields;

    bool checkSubFields(int i);
    void resetSubFields();

    //These should all be executes from the MainMesh,
    //As they recursively call all subMeshes.
    void initializeEvents();
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

public:

    MeshField(const mat & topology, Ensemble &ensemble,
              const std::string description = "meshField");

    const mat::fixed<ENS_DIM, 2> topology;
    const vec::fixed<ENS_DIM> shape;

    const std::string description;

    void addEvent(Event & event);

    void addSubField(MeshField &subField);

    virtual bool isWithinThis(int i);

    const double & getVolume() const {
        return volume;
    }

    virtual int getPopulation() const {
        return atoms.size();
    }

    const std::vector<int> & getAtoms() {
        return atoms;
    }

    friend class MainMesh;
    friend class fieldSampler;
};




#endif // MD_MESHFIELD_H
