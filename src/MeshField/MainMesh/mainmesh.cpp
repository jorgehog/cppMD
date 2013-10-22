#include "mainmesh.h"

#include <assert.h>

#include "../../Event/event.h"
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

void MainMesh::dumpEventsToFile()
{
    storeActiveEvents();

    observables.save("/home/jorgehog/tmp/mdEventsOut.arma");
}

void MainMesh::eventLoop(int N)
{

    loopCounter = new int(0);

    observables.zeros(N, Event::getCounter());

    initializeEvents(loopCounter, N);

    while (*loopCounter < N) {

        updateContainments();       //1. Find which atoms are in which meshes

        executeEvents();            //2. Let each mesh execute their events on these atoms
        dumpEvents();               //3. Let each event dump their output
        dumpEventsToFile();         //4. Dump the events to file if so is specified.

        resetEvents();              //4. Reset.

        *loopCounter = *loopCounter + 1;

    }
}


