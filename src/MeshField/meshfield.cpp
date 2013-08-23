#include "meshfield.h"

#include <sstream>
#include "../Ensemble/ensemble.h"
#include "../Event/event.h"

MeshField::MeshField(const mat &topology, Ensemble *ensemble, const std::string description):
    topology(topology),
    description(description),
    nEvents(0)
{
    this->ensemble = ensemble;
    parent = NULL;
    depth = 1;

}


bool MeshField::isWithinThis(int i) {

    for (int j = 0; j < MD_DIM; ++j) {
        if (ensemble->pos(i, j) < topology(j, 0)){
            return false;
        } else if (ensemble->pos(i, j) > topology(j, 1)) {
            return false;
        }
    }

    return true;

}


void MeshField::updateContainments()
{
    if (depth != 1) {
        std::cout << "updateContainments must be used on the main mesh only." << std::endl;
    }

    resetSubFields();

    for (int i = 0; i < MD_N; ++i) {
        (void)checkSubFields(i);
    }


}

void MeshField::resetSubFields()
{
    for (MeshField* subField : subFields){
        subField->resetSubFields();
    }

    reset();
}

bool MeshField::checkSubFields(int i){

    bool matchedInSubLevel = false;


    for (MeshField* subField : subFields){
        matchedInSubLevel = matchedInSubLevel || subField->checkSubFields(i);
    }

    if (matchedInSubLevel) {
        atoms.push_back(i);
    } else {
        matchedInSubLevel = append(i);
    }

    return matchedInSubLevel;

}



bool MeshField::notCompatible(MeshField *subField)
{

    if (subField == this) return true;

    const mat & sft = subField->topology;
    const mat & tft = this->topology;

#if MD_DIM == 2
    bool outsideMesh =  (sft(0, 0) < tft(0, 0)) ||
                        (sft(0, 1) > tft(0, 1)) ||
                        (sft(1, 0) < tft(1, 0)) ||
                        (sft(1, 1) > tft(1, 1));

    bool equalMesh   =  (sft(0, 0) == tft(0, 0)) &&
                        (sft(0, 1) == tft(0, 1)) &&
                        (sft(1, 0) == tft(1, 0)) &&
                        (sft(1, 1) == tft(1, 1));
#elif MD_DIM == 3
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


void MeshField::addEvent(Event *event)
{
    event->setMeshField(this);
    event->setEnsemble(ensemble);
    events.push_back(event);

    nEvents++;
}

void MeshField::addSubField(MeshField *subField)
{

    if (notCompatible(subField)) return;

    std::cout << "added subfield" << std::endl;

    subField->setParent(this);
    subFields.push_back(subField);

    nSubFields++;

}
