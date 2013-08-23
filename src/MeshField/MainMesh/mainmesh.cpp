#include "mainmesh.h"

#include "../../Event/SolverEvent/solverevent.h"
#include "../../Ensemble/ensemble.h"

#include <stdlib.h>
#include <time.h>

MainMesh::MainMesh(const mat &topology, Ensemble  & ensemble, SolverEvent  & solver):
    MeshField(topology, ensemble, "MainMesh")
{
    this->solver = &solver;
    solver.mainMesh = this;

//    srand(time(NULL));

    //placeHolder
    ensemble.pos.randu();
    for (int i = 0; i < MD_DIM; ++i) {
        ensemble.pos.col(i) *= shape(i);
    }


}


void MainMesh::updateContainments()
{

    resetSubFields();

    for (int i = 0; i < MD_N; ++i) {

        for (MeshField* subField : subFields){
            (void)subField->checkSubFields(i);
        }

    }

}

