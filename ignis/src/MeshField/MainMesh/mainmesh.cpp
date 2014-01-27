
#include "mainmesh.h"

#include <assert.h>

#include "../../Event/event.h"
#include "../../Event/intrinsicevents.h"
#include "../../Ensemble/ensemble.h"

using namespace ignis;

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

    for (uint i = 0; i < IGNIS_N; ++i) {
        atoms.push_back(i);
    }

}


void MainMesh::updateContainments()
{

    for (MeshField* subField : subFields){
        subField->resetSubFields();
    }

    for (uint i = 0; i < IGNIS_N; ++i) {

        for (MeshField* subField : subFields){
            (void)subField->checkSubFields(i);
        }

    }

}

void MainMesh::dumpEventsToFile() const
{
    for (Event* event: allEvents) {
        event->storeEvent();
    }

    Event::saveEventMatrix(outputPath);
}


void MainMesh::eventLoop(uint N)
{

    addIntrinsicEvents();

    uint* loopCycle = new uint(0);

    Event::setNumberOfCycles(N);
    Event::setLoopCyclePtr(loopCycle);

    initializeEvents();

    sortEvents();

    //SPEEDUP: STORE OUTPUT, TOFILE EVENTS IN OWN ARRAY AND LOOP THIS ONLY INSTEAD OF IFTESTING.
    while (*loopCycle < N) {

        for (int i = allEvents.size()-1; i >= 0; --i) {
            if (*loopCycle == allEvents.at(i)->getOffsetTime()+1) {
                allEvents.erase(allEvents.begin() + i);
            }
        }

        updateContainments();       //1. Find which atoms are in which meshes

        executeEvents();            //2. Execute each active event.

        *loopCycle = *loopCycle + 1;

    }

    Event::dumpEventMatrixData(N-1);

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

void MainMesh::addIntrinsicEvents()
{

    Event *_stdout = new _dumpEvents(this);
    _stdout->setManualPriority();

    Event *_fileio = new _dumpEventsToFile(this);
    _fileio->setManualPriority();

    addEvent(*_stdout);
    addEvent(*_fileio);

}

void MainMesh::sortEvents()
{
    std::sort(allEvents.begin(),
              allEvents.end(),
              sortByPriority());
}

void MainMesh::executeEvents()
{

    for(Event* event : allEvents)
    {
        event->executeEvent();
    }

    for (Event* event : allEvents){
        event->reset();
    }

}

void MainMesh::dumpEvents() const
{
    for (Event* event : allEvents)
    {
        if (event->notSilent())
        {
            cout << event->dumpString() << endl;
        }
    }

    cout << endl;
}



