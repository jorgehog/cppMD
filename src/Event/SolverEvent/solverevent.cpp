#include "solverevent.h"
#include "../../Ensemble/ensemble.h"

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

void SolverEvent::execute()
{

    initialize();

    vec test;
    double a;
    mainMesh->leastDistance(test,a , 0, 1);

    for (int i = 0; i < N; ++i, T+=dt) {

        mainMesh->updateContainments();

        mainMesh->executeEvents();
        mainMesh->dumpEvents();

        mainMesh->resetEvents();

        std::cout << mean(ensemble->pos, 1) << std::endl << mean(ensemble->vel, 1) << std::endl << mean(ensemble->forces, 1) << std::endl;
        std::cout << "t = " << T << " / " << (N-1)*dt << std::endl;

    }

}
