#ifndef SOLVEREVENT_H
#define SOLVEREVENT_H

#include "../event.h"
#include "../../MeshField/MainMesh/mainmesh.h"

class SolverEvent : public Event
{
protected:
    MainMesh * meshField;


public:

    SolverEvent();

    bool isSolverEvent(){
        return true;
    }

    void execute(){
        meshField->resetEvents();
        meshField->updateContainments();
        meshField->executeEvents();
        meshField->dumpEvents();
    }

};

#endif // SOLVEREVENT_H
