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

    bool m_isMainMesh;


    MeshField* parent;

    MainMesh* mainMesh;


    Ensemble *ensemble;

    std::vector<uint> atoms;

    std::vector<Event*> events;

    std::vector<MeshField*> subFields;


    virtual void sendToTop(Event & event);


    //These should all be executes from the MainMesh,
    //As they recursively call all subMeshes.
    void initializeEvents();

    void executeEvents();

    void dumpEvents();

    void storeActiveEvents();

    void resetEvents();


    bool append(uint i);

    bool checkSubFields(uint i);


    bool notCompatible(MeshField & subField);


    void resetSubFields();

    void resetContents(){
        atoms.clear();
    }


    std::exception * incorrectSubMeshPlacementException;


public:

    MeshField(const mat & topology, Ensemble &ensemble,
              const std::string description = "meshField");


    const double volume;

    const mat::fixed<ENS_DIM, 2> topology;

    const vec::fixed<ENS_DIM> shape;

    void setTopology(const mat & topology, bool recursive=true);


    const std::string description;


    bool isMainMesh () {
        return m_isMainMesh;
    }

    void setMainMesh(MainMesh* mainMesh) {
        this->mainMesh = mainMesh;
    }


    void setParent(MeshField* parent) {
        this->parent = parent;
    }

    MeshField* getParent () {
        return parent;
    }


    virtual bool isWithinThis(uint i);

    void addEvent(Event & event);

    void removeEvent(uint i);

    void addSubField(MeshField &subField);

    void removeSubField(uint i){
        subFields.erase(subFields.begin() + i);
    }


    void stretchField(double l, uint xyz);

    void scaleField(const vec & oldShape, const mat &oldTopology, const mat &newTopology);


    virtual uint getPopulation() const {
        return atoms.size();
    }

    const std::vector<uint> & getAtoms() const {
        return atoms;
    }

    const Ensemble * getEnsemble () const {
        return ensemble;
    }

    const std::vector<MeshField*> & getSubfields() const {
        return subFields;
    }

    const std::vector<Event*> & getEvents() const {
        return events;
    }


    friend class MainMesh;
    friend class ContractMesh;
    friend class ExpandMesh;

};


#endif // MD_MESHFIELD_H
