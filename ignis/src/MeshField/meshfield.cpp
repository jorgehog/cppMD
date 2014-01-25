#include "meshfield.h"

#include <sstream>
#include "../Ensemble/ensemble.h"
#include "../Event/event.h"

MeshField::MeshField(const mat &topology, Ensemble  & ensemble, const std::string description):
    m_isMainMesh(false),
    description(description)
{
    this->ensemble = &ensemble;

    setTopology(topology, false);

}


bool MeshField::isWithinThis(int i) {

    for (int j = 0; j < ENS_DIM; ++j) {
        if (ensemble->pos(j, i) < topology(j, 0)){
            return false;
        } else if (ensemble->pos(j, i) > topology(j, 1)) {
            return false;
        }
    }

    return true;

}

void MeshField::removeEvent(int i)
{
    events.erase(events.begin() + i);

    for (uint j = i; j < events.size(); ++j) {
        events.at(j)->setAddress(j);
    }
}


void MeshField::resetSubFields()
{
    for (MeshField* subField : subFields){
        subField->resetSubFields();
    }

    resetContents();
}

void MeshField::initializeEvents(int *loopCycle, int N)
{
    for (MeshField* subfield : subFields){
        subfield->initializeEvents(loopCycle, N);
    }

    for (Event* event : events){

        sendToTop(*event);
        event->setN(N);
        event->setLoopCyclePtr(loopCycle);

        event->initialize();

    }
}

void MeshField::dumpEvents()
{
    for (Event* event : events){
        if (event->notSilent()) std::cout << event->dumpString() << std::endl;
    }

    for (MeshField* subfield : subFields){
        subfield->dumpEvents();
    }
}

void MeshField::executeEvents()
{
    for (Event* event : events){
        event->executeEvent();
    }

    for (MeshField* subfield : subFields){
        subfield->executeEvents();
    }
}

void MeshField::resetEvents()
{
    for (MeshField* subfield : subFields){
        subfield->resetEvents();
    }

    for (Event* event : events){
        event->reset();
    }
}

void MeshField::sendToTop(Event &event)
{
    parent->sendToTop(event);
}

void MeshField::storeActiveEvents()
{

    for (MeshField* subfield : subFields){
        subfield->storeActiveEvents();
    }

    for (Event* event : events){
        if (event->shouldToFile()) {
            observables(*loopCounter, event->getId()) = event->getMeasurement();
        }
    }

}

bool MeshField::checkSubFields(int i){

    bool matchInSubField;
    bool matchedInSubLevel = false;

    //bool verbose = false;

    for (MeshField* subField : subFields){

        matchInSubField = subField->checkSubFields(i);
        matchedInSubLevel = matchedInSubLevel || matchInSubField;

    }

    if (matchedInSubLevel) {
        atoms.push_back(i);
    } else {
        matchedInSubLevel = append(i);
    }


    //if (verbose) std::cout << i << " checked by " << description << "\t:  " << matchedInSubLevel << std::endl;
    return matchedInSubLevel;

}



bool MeshField::notCompatible(MeshField & subField)
{

    if (&subField == this) return true;

    const mat & sft = subField.topology;
    const mat & tft = this->topology;

#if ENS_DIM == 2
    bool outsideMesh =  (sft(0, 0) < tft(0, 0)) ||
            (sft(0, 1) > tft(0, 1)) ||
            (sft(1, 0) < tft(1, 0)) ||
            (sft(1, 1) > tft(1, 1));

    bool equalMesh   =  (sft(0, 0) == tft(0, 0)) &&
            (sft(0, 1) == tft(0, 1)) &&
            (sft(1, 0) == tft(1, 0)) &&
            (sft(1, 1) == tft(1, 1));
    bool inverted    =  (sft(0, 0) >= sft(0, 1)) ||
            (sft(1, 0) >= sft(1, 1));
#elif ENS_DIM == 3
    bool outsideMesh =  (sft(0, 0) < tft(0, 0)) ||
            (sft(0, 1) > tft(0, 1)) ||
            (sft(1, 0) < tft(1, 0)) ||
            (sft(1, 1) > tft(1, 1)) ||
            (sft(2, 0) < tft(2, 0)) ||
            (sft(2, 1) > tft(2, 1));

    bool equalMesh   =  (sft(0, 0) == tft(0, 0)) &&
            (sft(1, 0) == tft(1, 0)) &&
            (sft(0, 1) == tft(0, 1)) &&
            (sft(1, 1) == tft(1, 1)) &&
            (sft(0, 2) == tft(0, 2)) &&
            (sft(1, 2) == tft(1, 2));

    bool inverted    =  (sft(0, 0) >= sft(0, 1)) ||
            (sft(1, 0) >= sft(1, 1)) ||
            (sft(2, 0) >= sft(2, 1));
#endif

    return outsideMesh || equalMesh || inverted;

}

void MeshField::setTopology(const mat &topology, bool recursive)
{

    if (recursive) {
        for (MeshField * subField : subFields) {
            subField->scaleField(shape, this->topology, topology);
        }
    }

    //Evil haxx for changing const values

    mat * matPtr;
    matPtr = (mat*)(&this->topology);
    *matPtr = topology;

    vec * vecPtr;
    vecPtr = (vec*)(&shape);
    *vecPtr = topology.col(1) - topology.col(0);

    //Calculate the volume
    volume = 1;
    for (int i = 0; i < ENS_DIM; ++i) {
        volume *= shape(i);
    }

}


void MeshField::addEvent(Event & event)
{

    event.setPriority();

    event.setMeshField(this);

    event.setEnsemble(ensemble);

    events.push_back(&event);

    event.setAddress(events.size()-1);

}

void MeshField::addSubField(MeshField  & subField)
{

    if (notCompatible(subField)) {
        std::cout << "subfield " << subField.description << " not compatible on " << description << std::endl;
        std::cout << "CONFLICT:\nsubField\n" << subField.topology << " is out of bounds, similar to parent or inverted/empty\n" << topology << std::endl;
        mat issue = (-topology + subField.topology);
        issue.col(1)*=-1;

        std::cout << "Issue at negative or non-zero region:\n" << issue << std::endl;
        throw *incorrectSubMeshPlacementException;
        return;
    }

    subField.setParent(this);
    subFields.push_back(&subField);

}

void MeshField::stretchField(double deltaL, int xyz)
{

    assert((xyz >= 0) && (xyz < ENS_DIM) && "stretch direction out of bounds.");

    mat newTopology = topology;
    newTopology(xyz, 0) += deltaL/2;
    newTopology(xyz, 1) -= deltaL/2;

    setTopology(newTopology);

}

void MeshField::scaleField(const vec & oldShape, const mat & oldTopology, const mat & newTopology){

    mat newSubTopology(ENS_DIM, 2);

    double oldCOM, newCOM, shapeFac, newShape_i, newSubShape_i;

    for (int i = 0; i < ENS_DIM; ++i) {

        newShape_i = newTopology(i, 1) - newTopology(i, 0);
        shapeFac = newShape_i/oldShape(i);

        newSubShape_i = shapeFac*shape(i);

        oldCOM = topology(i, 1) - shape(i)/2 - oldTopology(i, 0);

        newCOM = shapeFac*oldCOM;

        newSubTopology(i, 0) = newTopology(i, 0) + newCOM - newSubShape_i/2;
        newSubTopology(i, 1) = newTopology(i, 0) + newCOM + newSubShape_i/2;

    }

    setTopology(newSubTopology);

    //    topology.save((std::string)"/home/jorgehog/tmp/" + (description + ".arma"));

}

mat MeshField::observables;
int * MeshField::loopCounter;


