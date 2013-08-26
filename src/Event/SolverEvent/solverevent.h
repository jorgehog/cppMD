#ifndef SOLVEREVENT_H
#define SOLVEREVENT_H

#include "../event.h"
#include "../../MeshField/MainMesh/mainmesh.h"
#include "../../MD/mdEvents/calculateForces/calculateforces.h"

class SolverEvent : public Event
{
protected:

    const double dt;
    const int N;

    double T;

    std::vector<Event*> solverSpecificEvents;
    MainMesh * mainMesh;

    void setMainMesh(MainMesh* mainMesh);

    virtual void initialize(){}

public:

    SolverEvent(double dt, int N);

    void execute();

    friend MainMesh::MainMesh(const mat &, Ensemble &, SolverEvent &);
    friend class TwoBodyForce;

};

#endif // SOLVEREVENT_H
