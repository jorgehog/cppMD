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

        std::cout << "attempting reset" << std::endl;
        mainMesh->resetEvents();
        std::cout << "reset" << std::endl;
        mainMesh->updateContainments();
        std::cout << "updated" << std::endl;
        mainMesh->executeEvents();
        std::cout << "executed" << std::endl;
        mainMesh->dumpEvents();
        std::cout << "dumped" << std::endl;
    }

    friend MainMesh::MainMesh(const mat &, Ensemble &, SolverEvent &);

};

#endif // SOLVEREVENT_H
