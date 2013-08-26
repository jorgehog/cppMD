#ifndef MAINMESH_H
#define MAINMESH_H

#include "../meshfield.h"

class SolverEvent;

class MainMesh : public MeshField
{


public:
    MainMesh(const mat & topology, Ensemble &ensemble, SolverEvent &solver);

    void leastDistance(vec & leastRel, double &leastRel2, int i, int j);

    void updateContainments();

    int getPopulation() const {
        return ENS_N;
    }


    friend class SolverEvent;

private:
    SolverEvent * solver;
    ivec::fixed<3> ldi; //least distance iterator


};

#endif // MAINMESH_H
