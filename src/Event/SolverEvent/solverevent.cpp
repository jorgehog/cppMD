#include "solverevent.h"

SolverEvent::SolverEvent(double dt, int N):
    Event("mainSolver"),
    dt(dt),
    N(N),
    T(0),
    mainMesh(NULL)
{

}

void SolverEvent::setMainMesh(MainMesh *mainMesh)
{

    if (this->mainMesh != NULL) {
        std::cout << "main mesh already set in solver" << std::endl;
        return;
    }

    this->mainMesh = mainMesh;

    for (Event* mainMeshEvent : solverSpecificEvents){
        mainMesh->addEvent(*mainMeshEvent);
    }

    solverSpecificEvents.clear();

}

#include "../../Ensemble/ensemble.h"
void SolverEvent::execute()
{

    initialize();

    for (int i = 0; i < N; ++i, T+=dt) {

        std::cout << mean(ensemble->pos) << std::endl;
        mainMesh->updateContainments();

        mainMesh->executeEvents();
        mainMesh->dumpEvents();

        mainMesh->resetEvents();

        std::cout << "t = " << T << " / " << (N-1)*dt << std::endl;

    }

}
