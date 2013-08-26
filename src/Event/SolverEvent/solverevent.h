#ifndef SOLVEREVENT_H
#define SOLVEREVENT_H

#include "../event.h"
#include "../../MeshField/MainMesh/mainmesh.h"

class SolverEvent : public Event
{
protected:
    MainMesh * mainMesh;


public:

    SolverEvent();

    bool isSolverEvent(){
        return true;
    }

    void execute(){
        mainMesh->resetEvents();
        mainMesh->updateContainments();
        mainMesh->executeEvents();
        mainMesh->dumpEvents();
    }

    friend MainMesh::MainMesh(const mat &, Ensemble &, SolverEvent &);

};

#endif // SOLVEREVENT_H
