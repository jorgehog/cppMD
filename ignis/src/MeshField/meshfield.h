#ifndef MD_MESHFIELD_H
#define MD_MESHFIELD_H

#include "../defines.h"

#include <armadillo>
using namespace arma;

#include <string>
#include <vector>
#include <exception>

struct Ensemble;
class Event;
class MainMesh;

class incorrectSubMeshPlacementException : public std::exception {

    virtual const char* what() const throw()
    {
        return "Illegal placement of SubMesh";
    }

};

class MeshField
{

protected:

    double volume;

    bool m_isMainMesh;

    Ensemble *ensemble;

    MeshField* parent;
    MainMesh* mainMesh;

    std::vector<int> atoms;
    std::vector<Event*> events;
    std::vector<MeshField*> subFields;

    static int * loopCycle;

    virtual void sendToTop(Event & event);

    void storeActiveEvents();
    bool checkSubFields(int i);
    void resetSubFields();

    //These should all be executes from the MainMesh,
    //As they recursively call all subMeshes.
    void initializeEvents();
    void resetEvents();
    void executeEvents();
    void dumpEvents();

    std::exception * incorrectSubMeshPlacementException;


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

    bool isMainMesh () {
        return m_isMainMesh;
    }

    void setParent(MeshField* parent) {
        this->parent = parent;
    }

    void setMainMesh(MainMesh* mainMesh) {
        this->mainMesh = mainMesh;
    }

    MeshField* getParent () {
        return parent;
    }

    void setTopology(const mat & topology, bool recursive=true);

    void addEvent(Event & event);

    void addSubField(MeshField &subField);

    void stretchField(double l, int xyz);

    void scaleField(const vec & oldShape, const mat &oldTopology, const mat &newTopology);

    virtual bool isWithinThis(int i);

    const double & getVolume() const {
        return volume;
    }

    virtual int getPopulation() const {
        return atoms.size();
    }

    const std::vector<int> & getAtoms() const {
        return atoms;
    }

    const Ensemble * getEnsemble () const {
        return ensemble;
    }

    std::vector<MeshField*> getSubfields(){
        return subFields;
    }

    std::vector<Event*> getEvents(){
        return events;
    }

    void removeEvent(int i);

    void removeSubMesh(int i){
        subFields.erase(subFields.begin() + i);
    }

    friend class MainMesh;
    friend class ContractMesh;
    friend class ExpandMesh;

};


#endif // MD_MESHFIELD_H
