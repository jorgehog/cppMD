#ifndef SOLVEREVENT_H
#define SOLVEREVENT_H

#include "../event.h"
#include "../../MeshField/MainMesh/mainmesh.h"

class SolverEvent : public Event
{
protected:

    const int N;
    const double dt;

    double T;


    std::vector<Event*> solverSpecificEvents;
    MainMesh * mainMesh;

    void setMainMesh(MainMesh* mainMesh);

    virtual void initialize(){}

public:

    SolverEvent(double dt, int N);

    void execute();

    friend MainMesh::MainMesh(const mat &, Ensemble &, SolverEvent &);

};

#endif // SOLVEREVENT_H
