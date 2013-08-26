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

