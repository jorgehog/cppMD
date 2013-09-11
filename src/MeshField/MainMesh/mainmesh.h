#ifndef MAINMESH_H
#define MAINMESH_H

#include "../meshfield.h"

class SolverEvent;

class MainMesh : public MeshField
{

public:
    MainMesh(const mat & topology, Ensemble &ensemble);

    int getPopulation() const {
        return ENS_N;
    }

    void eventLoop(int N);

private:

    void updateContainments();


};

#endif // MAINMESH_H
