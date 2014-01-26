
#include "mainmesh.h"

#include <assert.h>

#include "../../Event/event.h"
#include "../../Ensemble/ensemble.h"

struct sortByPriority
{
    inline bool operator() (const Event* event1, const Event* event2)
    {
        return (event1->getPriority() < event2->getPriority());
    }
};


MainMesh::MainMesh(const mat &topology, Ensemble  & ensemble):
    MeshField(topology, ensemble, "MainMesh")
{

    m_isMainMesh = true;

    setOutputPath("/tmp/");

    for (uint i = 0; i < ENS_N; ++i) {
        atoms.push_back(i);
    }

}


void MainMesh::updateContainments()
{

    for (MeshField* subField : subFields){
        subField->resetSubFields();
    }

    for (uint i = 0; i < ENS_N; ++i) {

        for (MeshField* subField : subFields){
            (void)subField->checkSubFields(i);
        }

    }

}

void MainMesh::dumpEventsToFile()
{
    storeActiveEvents();
    //    for (Event* event: allEvents) {
    //        if (event->shouldToFile()) {
    //            observables(*loopCounter, event->getId()) = event->getMeasurement();
    //        }
    //    }

    Event::saveEventMatrix(outputPath);
}

void MainMesh::eventLoop(uint N)
{

    Event::setN(N);
    Event::initializeEventMatrix();

    uint* loopCycle = new uint(0);
    Event::setLoopCyclePtr(loopCycle);

    initializeEvents();

    //    for (Event* event : allEvents) {
    //        cout << event->getPriority() << "  " << event->getType() << endl;
    //    }
    sortEvents();
    //    for (Event* event : allEvents) {
    //        cout << event->getPriority() << "  " << event->getType() << endl;
    //    }
    //    exit(1);

    while (*loopCycle < N) {

        //        for (uint i = allEvents.size()-1; i <= 0; ++i) {
        //            if (*loopCycle == allEvents.at(i)->getOffsetTime()) {
        //                allEvents.erase(allEvents.begin() + i);
        //            }
        //        }

        updateContainments();       //1. Find which atoms are in which meshes

        executeEvents();            //2. Let each mesh execute their events on these atoms

        //        for(Event* event : allEvents) {
        //            event->executeEvent();
        //        }

        dumpEvents();               //3. Let each event dump their output
        std::cout << std::endl;

        dumpEventsToFile();         //4. Dump the events to file if so is specified.

        resetEvents();              //4. Reset.

        *loopCycle = *loopCycle + 1;

    }

    Event::dumpEventMatrixData(*loopCycle-1);

}

void MainMesh::setOutputPath(std::string path)
{
    if (strcmp(&path.back(), "/") != 0){
        path = path + "/";
    }

    outputPath = path + "mdEventsOut.arma";
}

void MainMesh::sendToTop(Event &event)
{
    allEvents.push_back(&event);
}

void MainMesh::sortEvents()
{
    std::sort(allEvents.begin(), allEvents.end(), sortByPriority());
}



