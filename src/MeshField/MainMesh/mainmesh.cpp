#include "mainmesh.h"

#include <assert.h>

#include "../../Ensemble/ensemble.h"


MainMesh::MainMesh(const mat &topology, Ensemble  & ensemble):
    MeshField(topology, ensemble, "MainMesh")
{

    for (int i = 0; i < ENS_N; ++i) {
        atoms.push_back(i);
    }

}


void MainMesh::updateContainments()
{

    for (MeshField* subField : subFields){
        subField->resetSubFields();
    }

    for (int i = 0; i < ENS_N; ++i) {

        for (MeshField* subField : subFields){
            (void)subField->checkSubFields(i);
        }

    }

}

void MainMesh::eventLoop(int N)
{

    int *i = new int(0);

    initializeEvents(i, N);

    while (*i < N) {

        updateContainments();       //1. Find which atoms are in which meshes

        executeEvents();            //2. Let each mesh execute their events on these atoms
        dumpEvents();               //3. Let each event dump their output

        resetEvents();              //4. Reset.

        *i = *i + 1;
    }
}


