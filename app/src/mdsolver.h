#ifndef MDSOLVER_H
#define MDSOLVER_H

#include <ignis.h>
#include <math.h>

using namespace ignis;

class mdSolver : public Event
{
public:

    mdSolver(double T0, double dt);

private:

    double sqrtkT0;
    double dt;

    void initialize();
    void execute() {}

};

#endif // MDSOLVER_H
