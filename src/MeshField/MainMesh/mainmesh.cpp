#include "mainmesh.h"

#include "../../Event/SolverEvent/solverevent.h"
#include "../../Ensemble/ensemble.h"

MainMesh::MainMesh(const mat &topology, Ensemble  & ensemble, SolverEvent  & solver):
    MeshField(topology, ensemble, "MainMesh")
{
    this->solver = &solver;

    //Initialize the solver
    solver.setMeshField(NULL);
    solver.setMainMesh(this);
    solver.setEnsemble(&ensemble);

    ldi(0) = -1;
    ldi(1) = 0;
    ldi(2) = 1;

}

void MainMesh::leastDistance(vec &leastRel, double &leastRel2, int i, int j)
{

    //if we have no periodicity, we are done.
#if !defined (ENS_PERIODIC_X) && !defined (ENS_PERIODIC_Y) && !defined (ENS_PERIODIC_Z)

    leastRel = ensemble->pos.col(i) - ensemble->pos.col(j);
    leastRel2 = 0;
    for (int k = 0; k < ENS_DIM; ++k) {
        leastRel2 += leastRel(k)*leastRel(k);
    }

#else

    leastRel2 = 1E23;
    vec orig(2);

    for (int c0 = 0; c0 < 3; ++c0) {
        for (int c1 = 0; c1 < 3; ++c1) {
            orig(0) = ensemble->pos(0, i) + ldi(c0)*shape(0);
            orig(1) = ensemble->pos(1, i) + ldi(c1)*shape(1);

            vec leastRelTest = orig - ensemble->pos.col(j);
            double leastRel2Test = 0;
            for (int k = 0; k < ENS_DIM; ++k) {
                leastRel2Test += leastRelTest(k)*leastRelTest(k);
            }

            if (leastRel2Test < leastRel2) {
                leastRel2 = leastRel2Test;
                leastRel = leastRelTest;
            }

        }
    }

//Can be optimized alot

#endif

    //TODO PARTIAL PERIODICITY
}


void MainMesh::updateContainments()
{

    resetSubFields();

    for (int i = 0; i < ENS_N; ++i) {

        for (MeshField* subField : subFields){
            (void)subField->checkSubFields(i);
        }

    }

}


