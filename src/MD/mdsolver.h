#ifndef MDSOLVER_H
#define MDSOLVER_H

#include "../Event/event.h"

#include <math.h>

class mdSolver : public Event
{
public:

    mdSolver(double T0, double dt) : Event("initMD"), sqrtkT0(sqrt(T0)), dt(dt) {}

private:

    double sqrtkT0;
    double dt;

    void initialize();
    void execute() {}

};

#endif // MDSOLVER_H
