#ifndef MAINMESH_H
#define MAINMESH_H

#include "../meshfield.h"

class SolverEvent;

class MainMesh : public MeshField
{
private:
    SolverEvent * solver;

public:
    MainMesh(const mat & topology, Ensemble &ensemble, SolverEvent &solver);

    void updateContainments();

    int getPopulation() const {
        return ENS_N;
    }


    friend class SolverEvent;

};

#endif // MAINMESH_H
