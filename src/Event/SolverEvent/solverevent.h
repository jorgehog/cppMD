#ifndef SOLVEREVENT_H
#define SOLVEREVENT_H

#include "../event.h"

class SolverEvent : public Event
{
protected:

    const double dt;
    const int N;

    virtual void initialize(){}

public:

    SolverEvent(double dt, int N);

    void execute() {};

    friend class MainMesh;

};

#endif // SOLVEREVENT_H
