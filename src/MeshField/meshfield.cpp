#include "meshfield.h"

#include <sstream>
#include "../Ensemble/ensemble.h"
#include "../Event/event.h"

MeshField::MeshField(const mat &topology, Ensemble  & ensemble, const std::string description):
    nEvents(0),
    topology(topology),
    description(description)
{
    this->ensemble = &ensemble;

    //Evil haxx for changing const values
    vec * vecPtr;
    vecPtr = (vec*)(&shape);
    *vecPtr = topology.col(1) - topology.col(0);;

    //Calculate the volume
    volume = 1;
    for (int i = 0; i < ENS_DIM; ++i) {
        volume *= shape(i);
    }


    //Necessary?
    parent = NULL;
    depth = 1;

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


void MeshField::resetSubFields()
{
    for (MeshField* subField : subFields){
        subField->resetSubFields();
    }

    resetContents();
}

void MeshField::initializeEvents()
{
    for (MeshField* subfield : subFields){
        subfield->initializeEvents();
    }

    for (Event* event : events){
        event->initialize();
    }
}

void MeshField::dumpEvents()
{
    for (Event* event : events){
        if (event->doOutput) std::cout << event->dumpString() << std::endl;
    }

    for (MeshField* subfield : subFields){
        subfield->dumpEvents();
    }
}

void MeshField::executeEvents()
{
    for (Event* event : events){
        event->execute();
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

bool MeshField::checkSubFields(int i){

    bool matchedInSubLevel = false;
    bool matchInSubField;

    bool verbose = false;

    for (MeshField* subField : subFields){

        matchInSubField = subField->checkSubFields(i);
        matchedInSubLevel = matchedInSubLevel || matchInSubField;

    }

    if (matchedInSubLevel) {
        atoms.push_back(i);
    } else {
        matchedInSubLevel = append(i);
    }


    if (verbose) std::cout << i << " checked by " << description << "\t:  " << matchedInSubLevel << std::endl;
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
#endif

    return outsideMesh || equalMesh;

}


void MeshField::addEvent(Event & event)
{
    event.setMeshField(this);
    event.setEnsemble(ensemble);
    events.push_back(&event);

    nEvents++;
}

void MeshField::addSubField(MeshField  & subField)
{

    if (notCompatible(subField)) {
        std::cout << "subfield " << subField.description << " not compatible on " << description << std::endl;
        std::cout << "CONFLICT:\nsubField\n" << subField.topology << " is out of bounds or similar to parent\n" << topology << std::endl;
        mat issue = (-topology + subField.topology);
        issue.col(1)*=-1;

        std::cout << "Issue at negative region:\n" << issue << std::endl;
        exit(1);
        return;
    }

//    std::cout << "added subfield" << std::endl;

    subField.setParent(this);
    subFields.push_back(&subField);

    nSubFields++;

}
