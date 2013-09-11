#ifndef MAINMESH_H
#define MAINMESH_H

#include "../meshfield.h"

class SolverEvent;

class MainMesh : public MeshField
{

public:
    MainMesh(const mat & topology, Ensemble &ensemble);

    void updateContainments();

    int getPopulation() const {
        return ENS_N;
    }

    void addSolverEvent(SolverEvent & solver);

    void eventLoop();

private:

    SolverEvent * solver;

};

#endif // MAINMESH_H
